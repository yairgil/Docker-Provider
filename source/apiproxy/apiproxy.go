package main

import (
	"context"
	"encoding/json"
	"fmt"
	"net/http"
	"os"
	"sync"
	"time"

	v1 "k8s.io/api/core/v1"
	"k8s.io/apimachinery/pkg/api/meta"
	metav1 "k8s.io/apimachinery/pkg/apis/meta/v1"
	"k8s.io/apimachinery/pkg/watch"
	"k8s.io/client-go/kubernetes"
	"k8s.io/client-go/rest"
	"k8s.io/client-go/tools/clientcmd"
)

const (
	POD_CHUNK_SIZE int64 = 1000
)

var (
	PodItemsCache       = make(map[string]PodItem)
	ClientSet           kubernetes.Interface
	PodsResourceVersion string = ""
	podsCacheRWLock            = &sync.RWMutex{}
)
var (
	Logger = createLogger()
	// Log wrapper function
	Log = Logger.Printf
)

func listAndWatchPods() {
	for {
		if PodsResourceVersion == "" {
			var podsList *v1.PodList
			var err error
			podsList, err = ClientSet.CoreV1().Pods("").List(context.TODO(), metav1.ListOptions{Limit: POD_CHUNK_SIZE})
			if err != nil {
				Log("apiproxy::listAndWatchPods::get pods failed with an error : %s", err.Error())
				time.Sleep(time.Second * 5) // avoid bombading the API server if its broken. // TODO - implement exponential backoff
				continue
			}
			Log("apiproxy::listAndWatchPods::pod count : %d \n", len(podsList.Items))
			for _, pod := range podsList.Items {
				podsCacheRWLock.Lock()
				PodItemsCache[string(pod.UID)] = getOptimizedPodItem(&pod)
				podsCacheRWLock.Unlock()
			}
			PodsResourceVersion = podsList.ResourceVersion
			continueToken := podsList.Continue
			podsList = nil
			for continueToken != "" {
				podsList, err = ClientSet.CoreV1().Pods("").List(context.TODO(), metav1.ListOptions{Limit: POD_CHUNK_SIZE, Continue: continueToken})
				if err != nil {
					Log("apiproxy::listAndWatchPods::get pods failed with an error : %s", err.Error())
					continueToken = ""
					podsList = nil
				} else {
					Log("apiproxy::listAndWatchPods::pod count : %d \n", len(podsList.Items))
					for _, pod := range podsList.Items {
						podsCacheRWLock.Lock()
						PodItemsCache[string(pod.UID)] = getOptimizedPodItem(&pod)
						podsCacheRWLock.Unlock()
					}
					continueToken = podsList.Continue
					podsList = nil
				}
			}
			Log("apiproxy::listAndWatchPods::continue: %s \n", continueToken)
			Log("apiproxy::listAndWatchPods::resource version: %s \n", PodsResourceVersion)
		}
		Log("apiproxy::listAndWatchPods::Total Pod count: %d \n", len(PodItemsCache))

		listOptions := metav1.ListOptions{AllowWatchBookmarks: true}
		if PodsResourceVersion != "" {
			listOptions.ResourceVersion = PodsResourceVersion
		}
		Log("apiproxy::listAndWatchPods::Establishing watch connection @  %s with ResourceVersion: %s \n", time.Now().UTC().String(), PodsResourceVersion)
		watcher, err := ClientSet.CoreV1().Pods(v1.NamespaceAll).Watch(context.Background(), listOptions)
		if err != nil {
			Log("apiproxy::listAndWatchPods::watch connection failed and reconnecting Watch after List: %s \n", err.Error())
			time.Sleep(time.Millisecond * 500)
			PodsResourceVersion = "" // validate, is this right thing to do considering performance reasons
			continue
		}

		for event := range watcher.ResultChan() {
			meta, err := meta.Accessor(event.Object)
			if err != nil {
				Log("apiproxy::listAndWatchPods::unable to understand watch event: %s", event.Type)
				continue
			}
			PodsResourceVersion = meta.GetResourceVersion()
			Log("apiproxy::listAndWatchPods::Event receive version : %s @ %s \n", PodsResourceVersion, time.Now().UTC().String())
			switch event.Type {
			case watch.Bookmark:
				Log("apiproxy::listAndWatchPods::BookMark event received \n")
			case watch.Error:
				Log("apiproxy::listAndWatchPods::Error event received \n")
			case watch.Added:
				pod := event.Object.(*v1.Pod)
				podsCacheRWLock.Lock()
				PodItemsCache[string(pod.UID)] = getOptimizedPodItem(pod)
				podsCacheRWLock.Unlock()
				Log("apiproxy::listAndWatchPods::Pod %s/%s added \n", pod.ObjectMeta.Namespace, pod.ObjectMeta.Name)
			case watch.Modified:
				pod := event.Object.(*v1.Pod)
				podsCacheRWLock.Lock()
				PodItemsCache[string(pod.UID)] = getOptimizedPodItem(pod)
				podsCacheRWLock.Unlock()
				Log("apiproxy::listAndWatchPods::Pod %s/%s modified \n", pod.ObjectMeta.Namespace, pod.ObjectMeta.Name)
			case watch.Deleted:
				pod := event.Object.(*v1.Pod)
				podsCacheRWLock.Lock()
				delete(PodItemsCache, string(pod.UID))
				podsCacheRWLock.Unlock()
				Log("apiproxy::listAndWatchPods::Pod %s/%s deleted \n", pod.ObjectMeta.Namespace, pod.ObjectMeta.Name)
			}
		}
	}
}

func pods(w http.ResponseWriter, r *http.Request) {
	switch r.Method {
	case "GET":
		fmt.Printf("get Pods invoked :%s \n", time.Now().UTC().String())

		podsResponse := make(map[string][]PodItem)
		podsCacheRWLock.RLock()
		Log("get Pods invoked with Pod count :%d \n", len(PodItemsCache))
		podsResponse["items"] = make([]PodItem, 0, len(PodItemsCache))
		for _, podItem := range PodItemsCache {
			podsResponse["items"] = append(podsResponse["items"], podItem)
		}
		podsCacheRWLock.RUnlock()
		j, _ := json.Marshal(podsResponse)
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

	var kubeconfigPath = "/home/sshadmin/testakscluster/testakscluster" // path MUST be empty for incluster scenario

	ClientSet, err = createClient(kubeconfigPath)
	if err != nil {
		Log("create Kubernetes client failed with an error :%s", err.Error())
		os.Exit(1)
	}

	go listAndWatchPods()

	http.HandleFunc("/pods", pods)
	//http.HandleFunc("/nodes", nodes)

	Log("Starting Kuberenets Proxy Server")
	http.ListenAndServe(":8080", nil)
}
