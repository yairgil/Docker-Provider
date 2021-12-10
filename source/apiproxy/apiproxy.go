package main

import (
	"context"
	"encoding/json"
	"fmt"
	"io"
	"net/http"
	"os"
	"sync"
	"time"

	v1 "k8s.io/api/core/v1"
	apierrors "k8s.io/apimachinery/pkg/api/errors"
	"k8s.io/apimachinery/pkg/api/meta"
	metav1 "k8s.io/apimachinery/pkg/apis/meta/v1"
	"k8s.io/apimachinery/pkg/watch"
	"k8s.io/client-go/kubernetes"
	"k8s.io/client-go/rest"
	"k8s.io/client-go/tools/clientcmd"
)

const (
	POD_CHUNK_SIZE     int64 = 1000
	SERVICE_CHUNK_SIZE int64 = 500
)

var (
	ClientSet kubernetes.Interface
)

var (
	PodItemsCache              = make(map[string]PodItem)
	PodsResourceVersion string = ""
	PodCacheRWLock             = &sync.RWMutex{}
)

var (
	ServiceItemsCache              = make(map[string]ServiceItem)
	ServicesResourceVersion string = ""
	ServiceCacheRWLock             = &sync.RWMutex{}
)

var (
	Logger = createLogger()
	// Log wrapper function
	Log = Logger.Printf
)

func listAndWatchPods() {
	for {
		if PodsResourceVersion == "" {
			startTime := time.Now()
			var podList *v1.PodList
			var err error
			podList, err = ClientSet.CoreV1().Pods("").List(context.TODO(), metav1.ListOptions{Limit: POD_CHUNK_SIZE})
			if err != nil {
				Log("apiproxy::listAndWatchPods::get pods failed with an error : %s", err.Error())
				time.Sleep(time.Second * 5) // avoid bombading the API server if its broken. // TODO - implement exponential backoff
				continue
			}
			Log("apiproxy::listAndWatchPods::pod count : %d \n", len(podList.Items))
			for _, pod := range podList.Items {
				PodCacheRWLock.Lock()
				PodItemsCache[string(pod.UID)] = getOptimizedPodItem(&pod)
				PodCacheRWLock.Unlock()
			}
			PodsResourceVersion = podList.ResourceVersion
			continueToken := podList.Continue
			podList = nil
			for continueToken != "" {
				podList, err = ClientSet.CoreV1().Pods("").List(context.TODO(), metav1.ListOptions{Limit: POD_CHUNK_SIZE, Continue: continueToken})
				if err != nil {
					Log("apiproxy::listAndWatchPods::get pods failed with an error : %s", err.Error())
					continueToken = ""
					podList = nil
					//TODO - do we need retry incase of chunked api call failure??
				} else {
					Log("apiproxy::listAndWatchPods::pod count : %d \n", len(podList.Items))
					for _, pod := range podList.Items {
						PodCacheRWLock.Lock()
						PodItemsCache[string(pod.UID)] = getOptimizedPodItem(&pod)
						PodCacheRWLock.Unlock()
					}
					continueToken = podList.Continue
					podList = nil
				}
			}
			elapsedTime := time.Since(startTime)
			Log("apiproxy::listAndWatchPods::List Pods API E2E Latency(seconds): %.2f \n", elapsedTime.Seconds())
			Log("apiproxy::listAndWatchPods::continue: %s \n", continueToken)
			Log("apiproxy::listAndWatchPods::resource version: %s \n", PodsResourceVersion)
		}
		Log("apiproxy::listAndWatchPods::Total Pod count: %d \n", len(PodItemsCache))

		listOptions := metav1.ListOptions{AllowWatchBookmarks: true}
		if PodsResourceVersion != "" {
			listOptions.ResourceVersion = PodsResourceVersion
		}
		Log("apiproxy::listAndWatchPods::Establishing watch connection with ResourceVersion: %s \n", PodsResourceVersion)
		watcher, err := ClientSet.CoreV1().Pods(v1.NamespaceAll).Watch(context.Background(), listOptions)
		if err != nil {
			Log("apiproxy::listAndWatchPods::Failed to establish watch connection with ResourceVersion: %s \n", PodsResourceVersion)
			switch err {
			case io.EOF:
				Log("apiproxy::listAndWatchPods::watch connection closed normally: %s \n", err.Error())
			case io.ErrUnexpectedEOF:
				Log("apiproxy::listAndWatchPods::Watch closed with unexpected EOF: %s \n", err.Error())
			default:
				Log("apiproxy::listAndWatchPods::watch connection failed with an error: %s \n", err.Error())
			}
			time.Sleep(time.Millisecond * 500)
		} else if watcher != nil {
			for event := range watcher.ResultChan() {
				switch event.Type {
				case watch.Added, watch.Modified, watch.Deleted, watch.Bookmark:
					meta, err := meta.Accessor(event.Object)
					if err != nil {
						Log("apiproxy::listAndWatchPods::unable to understand watch event: %s", event.Type)
						//setting ResourceVersion to "" enforce List Again
						PodsResourceVersion = ""
						// We have to abort here because this might cause lastResourceVersion inconsistency by skipping a potential RV with valid data!
						watcher.Stop()
					} else {
						PodsResourceVersion = meta.GetResourceVersion()
						Log("apiproxy::listAndWatchPods::Event Type: %s  and Received Resource Version: %s \n", event.Type, PodsResourceVersion)
						if event.Type != watch.Bookmark {
							pod := event.Object.(*v1.Pod)
							if event.Type == watch.Deleted {
								PodCacheRWLock.Lock()
								delete(PodItemsCache, string(pod.UID))
								PodCacheRWLock.Unlock()
							} else {
								PodCacheRWLock.Lock()
								PodItemsCache[string(pod.UID)] = getOptimizedPodItem(pod)
								PodCacheRWLock.Unlock()
							}
						}
					}
				case watch.Error:
					Log("apiproxy::listAndWatchPods::Error event received \n")
					errObject := apierrors.FromObject(event.Object)
					statusErr, ok := errObject.(*apierrors.StatusError)
					if !ok {
						Log("apiproxy::listAndWatchPods::Received an error which is not *metav1.Status but %#+v", event.Object)
						//retry unknown error
						watcher.Stop()
					} else {
						status := statusErr.ErrStatus
						retryAfterSeconds := int32(1)
						statusDelay := time.Duration(retryAfterSeconds) * time.Second
						if status.Details != nil {
							retryAfterSeconds = status.Details.RetryAfterSeconds
							statusDelay = time.Duration(retryAfterSeconds) * time.Second
						}
						Log("apiproxy::listAndWatchPods::Received an error with  error status code : %d", status.Code)
						switch status.Code {
						case http.StatusGone:
							//setting ResourceVersion to "" enforce List Again
							PodsResourceVersion = ""
							watcher.Stop()
							time.Sleep(statusDelay)
						case http.StatusGatewayTimeout, http.StatusInternalServerError:
							watcher.Stop()
							time.Sleep(statusDelay)
						default:
							watcher.Stop()
							time.Sleep(statusDelay)
						}
						Log("apiproxy::listAndWatchPods:: Stopping watcher and retrying after delay in seconds: %d", retryAfterSeconds)
					}
				default:
					Log("Failed to recognize Event type %q", event.Type)
					//setting ResourceVersion to "" enforce List Again
					PodsResourceVersion = ""
					// We have to abort here because this might cause lastResourceVersion inconsistency by skipping a potential RV with valid data!
					watcher.Stop()
				}
			}
		} else {
			Log("apiproxy::listAndWatchPods::Watch returned nil watcher for watch connection with ResourceVersion: %s \n", PodsResourceVersion)
		}
	}
}

func listAndWatchServices() {
	for {
		if ServicesResourceVersion == "" {
			var serviceList *v1.ServiceList
			var err error
			serviceList, err = ClientSet.CoreV1().Services("").List(context.TODO(), metav1.ListOptions{Limit: SERVICE_CHUNK_SIZE})
			if err != nil {
				Log("apiproxy::listAndWatchServices::get services failed with an error : %s", err.Error())
				time.Sleep(time.Second * 5) // avoid bombading the API server if its broken. // TODO - implement exponential backoff
				continue
			}
			Log("apiproxy::listAndWatchServices::service count : %d \n", len(serviceList.Items))
			for _, item := range serviceList.Items {
				ServiceCacheRWLock.Lock()
				ServiceItemsCache[string(item.UID)] = getOptimizedServiceItem(&item)
				ServiceCacheRWLock.Unlock()
			}
			ServicesResourceVersion = serviceList.ResourceVersion
			continueToken := serviceList.Continue
			serviceList = nil
			for continueToken != "" {
				serviceList, err = ClientSet.CoreV1().Services("").List(context.TODO(), metav1.ListOptions{Limit: SERVICE_CHUNK_SIZE, Continue: continueToken})
				if err != nil {
					Log("apiproxy::listAndWatchServices::get services failed with an error: %s", err.Error())
					continueToken = ""
					serviceList = nil
				} else {
					Log("apiproxy::listAndWatchServices::services count : %d \n", len(serviceList.Items))
					for _, item := range serviceList.Items {
						ServiceCacheRWLock.Lock()
						ServiceItemsCache[string(item.UID)] = getOptimizedServiceItem(&item)
						ServiceCacheRWLock.Unlock()
					}
					continueToken = serviceList.Continue
					serviceList = nil
				}
			}
			Log("apiproxy::listAndWatchServices::continue: %s \n", continueToken)
			Log("apiproxy::listAndWatchServices::resource version: %s \n", ServicesResourceVersion)
		}
		Log("apiproxy::listAndWatchServices::Total Service count: %d \n", len(ServiceItemsCache))

		listOptions := metav1.ListOptions{AllowWatchBookmarks: true}
		if ServicesResourceVersion != "" {
			listOptions.ResourceVersion = ServicesResourceVersion
		}
		Log("apiproxy::listAndWatchServices::Establishing watch connection with ResourceVersion: %s \n", ServicesResourceVersion)
		watcher, err := ClientSet.CoreV1().Services(v1.NamespaceAll).Watch(context.Background(), listOptions)
		if err != nil {
			Log("apiproxy::listAndWatchServices::watch connection failed and reconnecting Watch after List: %s \n", err.Error())
			time.Sleep(time.Millisecond * 500)
			ServicesResourceVersion = "" // validate, is this right thing to do considering performance reasons
			continue
		}

		for event := range watcher.ResultChan() {
			meta, err := meta.Accessor(event.Object)
			if err != nil {
				Log("apiproxy::listAndWatchServices::unable to understand watch event: %s", event.Type)
				continue
			}
			ServicesResourceVersion = meta.GetResourceVersion()
			Log("apiproxy::listAndWatchServices::Event received with ResourceVersion: %s \n", ServicesResourceVersion)
			switch event.Type {
			case watch.Bookmark:
				Log("apiproxy::listAndWatchServices::BookMark event received \n")
			case watch.Error:
				Log("apiproxy::listAndWatchServices::Error event received \n")
			case watch.Added:
				service := event.Object.(*v1.Service)
				ServiceCacheRWLock.Lock()
				ServiceItemsCache[string(service.UID)] = getOptimizedServiceItem(service)
				ServiceCacheRWLock.Unlock()
				Log("apiproxy::listAndWatchServices::Service %s/%s added \n", service.ObjectMeta.Namespace, service.ObjectMeta.Name)
			case watch.Modified:
				service := event.Object.(*v1.Service)
				ServiceCacheRWLock.Lock()
				ServiceItemsCache[string(service.UID)] = getOptimizedServiceItem(service)
				ServiceCacheRWLock.Unlock()
				Log("apiproxy::listAndWatchServices::Service %s/%s modified \n", service.ObjectMeta.Namespace, service.ObjectMeta.Name)
			case watch.Deleted:
				service := event.Object.(*v1.Service)
				ServiceCacheRWLock.Lock()
				delete(ServiceItemsCache, string(service.UID))
				ServiceCacheRWLock.Unlock()
				Log("apiproxy::listAndWatchServices::Service %s/%s deleted \n", service.ObjectMeta.Namespace, service.ObjectMeta.Name)
			}
		}
	}
}

func pods(w http.ResponseWriter, r *http.Request) {
	switch r.Method {
	case "GET":
		fmt.Printf("apiproxy::pods::get Pods invoked :%s \n", time.Now().UTC().String())

		podsResponse := make(map[string][]PodItem)
		PodCacheRWLock.RLock()
		Log("apiproxy::pods::get Pods invoked with Pod count :%d \n", len(PodItemsCache))
		podsResponse["items"] = make([]PodItem, 0, len(PodItemsCache))
		for _, podItem := range PodItemsCache {
			podsResponse["items"] = append(podsResponse["items"], podItem)
		}
		PodCacheRWLock.RUnlock()
		start := time.Now()
		j, _ := json.Marshal(podsResponse)
		elapsed := time.Since(start)
		Log("apiproxy::pods::Time taken for JSON serilization (milliseconds) :%.2f \n", elapsed.Milliseconds())
		w.Write(j)
	default:
		w.WriteHeader(http.StatusMethodNotAllowed)
		fmt.Fprintf(w, "Not supported method")
	}
}

func services(w http.ResponseWriter, r *http.Request) {
	switch r.Method {
	case "GET":
		fmt.Printf("get services invoked :%s \n", time.Now().UTC().String())
		servicesResponse := make(map[string][]ServiceItem)
		ServiceCacheRWLock.RLock()
		Log("get services invoked with service count :%d \n", len(ServiceItemsCache))
		servicesResponse["items"] = make([]ServiceItem, 0, len(ServiceItemsCache))
		for _, serviceItem := range ServiceItemsCache {
			servicesResponse["items"] = append(servicesResponse["items"], serviceItem)
		}
		ServiceCacheRWLock.RUnlock()
		j, _ := json.Marshal(servicesResponse)
		w.Write(j)
	default:
		w.WriteHeader(http.StatusMethodNotAllowed)
		fmt.Fprintf(w, "Not supported method")
	}
}

func createClient(kubeconfigPath string) (kubernetes.Interface, error) {
	var kubeconfig *rest.Config

	if kubeconfigPath != "" {
		config, err := clientcmd.BuildConfigFromFlags("", kubeconfigPath)
		if err != nil {
			return nil, fmt.Errorf("unable to load kubeconfig from %s: %v", kubeconfigPath, err)
		}
		kubeconfig = config
	} else {
		config, err := rest.InClusterConfig()
		if err != nil {
			return nil, fmt.Errorf("unable to load in-cluster config: %v", err)
		}
		kubeconfig = config
	}

	client, err := kubernetes.NewForConfig(kubeconfig)
	if err != nil {
		return nil, fmt.Errorf("unable to create a client: %v", err)
	}

	return client, nil
}

func main() {
	var err error

	// path MUST be empty for incluster scenario
	var kubeconfigPath = "/home/sshadmin/gangams-aks-hyperscale-test/gangams-aks-hyperscale-test"

	ClientSet, err = createClient(kubeconfigPath)
	if err != nil {
		Log("create Kubernetes client failed with an error :%s", err.Error())
		os.Exit(1)
	}

	//go listAndWatchServices()
	go listAndWatchPods()

	http.HandleFunc("/pods", pods)
	//http.HandleFunc("/services", services)

	Log("Starting Kuberenets Proxy Server")
	http.ListenAndServe(":8080", nil)
}
