package main

import (
	"context"
	"encoding/json"
	"flag"
	"fmt"
	"log"
	"net/http"
	"os"
	"sync"
	"time"

	//"github.com/patrickmn/go-cache"

	lumberjack "gopkg.in/natefinch/lumberjack.v2"
	v1 "k8s.io/api/core/v1"
	"k8s.io/apimachinery/pkg/api/meta"
	metav1 "k8s.io/apimachinery/pkg/apis/meta/v1"
	"k8s.io/apimachinery/pkg/watch"
	"k8s.io/client-go/kubernetes"
	"k8s.io/client-go/rest"
)

const POD_CHUNK_SIZE int64 = 1000

var (
	PodItemsCache = make(map[string]PodItem)
	//PodItemsCache = cache.New(cache.NoExpiration, cache.NoExpiration)
	ClientSet       *kubernetes.Clientset
	ResourceVersion string = ""
	rwLock                 = &sync.RWMutex{}
)
var (
	Logger = createLogger()
	// Log wrapper function
	Log = Logger.Printf
)

func createLogger() *log.Logger {
	var logfile *os.File
	logPath := "/var/opt/microsoft/docker-cimprov/log/api-proxy.log"

	if _, err := os.Stat(logPath); err == nil {
		fmt.Printf("File Exists. Opening file in append mode...\n")
		logfile, err = os.OpenFile(logPath, os.O_APPEND|os.O_WRONLY, 0600)
		if err != nil {
			//SendException(err.Error())
			fmt.Printf(err.Error())
		}
	}

	if _, err := os.Stat(logPath); os.IsNotExist(err) {
		fmt.Printf("File Doesnt Exist. Creating file...\n")
		logfile, err = os.Create(logPath)
		if err != nil {
			//SendException(err.Error())
			fmt.Printf(err.Error())
		}
	}

	logger := log.New(logfile, "", 0)

	logger.SetOutput(&lumberjack.Logger{
		Filename:   logPath,
		MaxSize:    10, //megabytes
		MaxBackups: 1,
		MaxAge:     28,   //days
		Compress:   true, // false by default
	})

	logger.SetFlags(log.Ltime | log.Lshortfile | log.LstdFlags)
	return logger
}

type PodItem struct {
	Metadata MetaData `json:"metadata"`
	Spec     Spec     `json:"spec"`
	Status   Status   `json:"status"`
}
type Spec struct {
	NodeName       string      `json:"nodeName"`
	Containers     []Container `json:"containers"`
	InitContainers []Container `json:"initContainers"`
}
type Status struct {
	StartTime             string            `json:"startTime,omitempty"`
	Reason                string            `json:"reason,omitempty"`
	PodIP                 string            `json:"podIP,omitempty"`
	Phase                 string            `json:"phase,omitempty"`
	Conditions            []PodCondition    `json:"conditions,omitempty"`
	InitContainerStatuses []ContainerStatus `json:"initContainerStatuses,omitempty"`
	ContainerStatuses     []ContainerStatus `json:"containerStatuses,omitempty"`
}

type PodCondition struct {
	PodConditionType   string `json:"type,omitempty"`
	Status             string `json:"status,omitempty"`
	LastTransitionTime string `json:"lastTransitionTime,omitempty"`
}
type ContainerStatus struct {
	ContainerID          string         `json:"containerID,omitempty"`
	Name                 string         `json:"name"`
	RestartCount         int32          `json:"restartCount"`
	State                ContainerState `json:"state,omitempty"`
	LastTerminationState ContainerState `json:"lastState,omitempty"`
}
type ContainerState struct {
	Waiting    *ContainerStateWaiting    `json:"waiting,omitempty"`
	Running    *ContainerStateRunning    `json:"running,omitempty"`
	Terminated *ContainerStateTerminated `json:"terminated,omitempty"`
}

// ContainerStateWaiting is a waiting state of a container.
type ContainerStateWaiting struct {
	Reason  string `json:"reason,omitempty"`
	Message string `json:"message,omitempty"`
}

// ContainerStateRunning is a running state of a container.
type ContainerStateRunning struct {
	StartedAt string `json:"startedAt,omitempty"`
}

// ContainerStateTerminated is a terminated state of a container.
type ContainerStateTerminated struct {
	ExitCode    int32  `json:"exitCode"`
	Reason      string `json:"reason,omitempty"`
	Message     string `json:"message,omitempty"`
	StartedAt   string `json:"startedAt,omitempty"`
	FinishedAt  string `json:"finishedAt,omitempty"`
	ContainerID string `json:"containerID,omitempty"`
}

type Container struct {
	Name      string               `json:"name"`
	Resources ResourceRequirements `json:"resources,omitempty"`
}
type ResourceRequirements struct {
	Limits   map[string]string `json:"limits,omitempty"`
	Requests map[string]string `json:"requests,omitempty"`
}

type MetaData struct {
	Name              string             `json:"name"`
	Namespace         string             `json:"namespace"`
	ResourceVersion   string             `json:"resourceVersion"`
	CreationTimestamp string             `json:"creationTimestamp"`
	UID               string             `json:"uid"`
	DeletionTimestamp string             `json:"deletionTimestamp,omitempty"`
	Annotations       *map[string]string `json:"annotations,omitempty"`
	Labels            *map[string]string `json:"labels"`
	OwnerReferences   []OwnerReference   `json:"ownerReferences,omitempty"`
}
type OwnerReference struct {
	Name string `json:"name"`
	Kind string `json:"kind"`
}

func getOptimizedPodItem(pod *v1.Pod) PodItem {
	podItem := PodItem{
		Metadata: MetaData{
			Name:              pod.Name,
			Namespace:         pod.Namespace,
			ResourceVersion:   pod.ResourceVersion,
			CreationTimestamp: pod.CreationTimestamp.Format(time.RFC3339),
			UID:               string(pod.UID),
			Labels:            &pod.Labels,
		},
	}
	if pod.DeletionTimestamp != nil {
		podItem.Metadata.DeletionTimestamp = pod.DeletionTimestamp.Format(time.RFC3339)
	}
	if pod.Annotations != nil {
		podItem.Metadata.Annotations = &pod.Annotations
	}
	if pod.OwnerReferences != nil && len(pod.OwnerReferences) > 0 {
		podItem.Metadata.OwnerReferences = make([]OwnerReference, len(pod.OwnerReferences))
		for i, ownerRef := range pod.OwnerReferences {
			podItem.Metadata.OwnerReferences[i].Name = ownerRef.Name
			podItem.Metadata.OwnerReferences[i].Kind = ownerRef.Kind
		}
	}

	podItem.Spec.NodeName = pod.Spec.NodeName
	if pod.Spec.Containers != nil && len(pod.Spec.Containers) > 0 {
		podItem.Spec.Containers = make([]Container, len(pod.Spec.Containers))
		for i, container := range pod.Spec.Containers {
			podItem.Spec.Containers[i].Name = container.Name
			podItem.Spec.Containers[i].Resources.Requests = make(map[string]string)
			for k, v := range container.Resources.Requests {
				podItem.Spec.Containers[i].Resources.Requests[string(k)] = v.String()
			}
			podItem.Spec.Containers[i].Resources.Limits = make(map[string]string)
			for k, v := range container.Resources.Limits {
				podItem.Spec.Containers[i].Resources.Limits[string(k)] = v.String()
			}
		}
	}

	if pod.Spec.InitContainers != nil && len(pod.Spec.InitContainers) > 0 {
		podItem.Spec.InitContainers = make([]Container, len(pod.Spec.InitContainers))
		for i, container := range pod.Spec.InitContainers {
			podItem.Spec.InitContainers[i].Name = container.Name
			podItem.Spec.InitContainers[i].Resources.Requests = make(map[string]string)
			for k, v := range container.Resources.Requests {
				podItem.Spec.InitContainers[i].Resources.Requests[string(k)] = v.String()
			}
			podItem.Spec.Containers[i].Resources.Limits = make(map[string]string)
			for k, v := range container.Resources.Limits {
				podItem.Spec.Containers[i].Resources.Limits[string(k)] = v.String()
			}
		}
	}

	if pod.Status.StartTime != nil {
		podItem.Status.StartTime = pod.Status.StartTime.Format(time.RFC3339)
	}

	if pod.Status.Reason != "" {
		podItem.Status.Reason = pod.Status.Reason
	}

	if pod.Status.PodIP != "" {
		podItem.Status.PodIP = pod.Status.PodIP
	}

	if pod.Status.Phase != "" {
		podItem.Status.Phase = string(pod.Status.Phase)
	}

	if pod.Status.Conditions != nil && len(pod.Status.Conditions) > 0 {
		podItem.Status.Conditions = make([]PodCondition, len(pod.Status.Conditions))
		for i, condition := range pod.Status.Conditions {
			podItem.Status.Conditions[i].Status = string(condition.Status)
			podItem.Status.Conditions[i].PodConditionType = string(condition.Type)
			podItem.Status.Conditions[i].LastTransitionTime = condition.LastTransitionTime.Format(time.RFC3339)
		}
	}

	if pod.Status.InitContainerStatuses != nil && len(pod.Status.InitContainerStatuses) > 0 {
		podItem.Status.InitContainerStatuses = make([]ContainerStatus, len(pod.Status.InitContainerStatuses))
		for i, containerStatus := range pod.Status.InitContainerStatuses {
			podItem.Status.InitContainerStatuses[i].ContainerID = containerStatus.ContainerID
			podItem.Status.InitContainerStatuses[i].Name = containerStatus.Name
			podItem.Status.InitContainerStatuses[i].RestartCount = containerStatus.RestartCount
			if containerStatus.State.Waiting != nil {
				podItem.Status.InitContainerStatuses[i].State.Waiting = &ContainerStateWaiting{
					Message: containerStatus.State.Waiting.Message,
					Reason:  containerStatus.State.Waiting.Reason,
				}
			}
			if containerStatus.State.Running != nil {
				podItem.Status.InitContainerStatuses[i].State.Running = &ContainerStateRunning{
					StartedAt: containerStatus.State.Running.StartedAt.Format(time.RFC3339),
				}
			}
			if containerStatus.State.Terminated != nil {
				podItem.Status.InitContainerStatuses[i].State.Terminated = &ContainerStateTerminated{
					ContainerID: containerStatus.State.Terminated.ContainerID,
					ExitCode:    containerStatus.State.Terminated.ExitCode,
					FinishedAt:  containerStatus.State.Terminated.FinishedAt.Format(time.RFC3339),
					StartedAt:   containerStatus.State.Terminated.StartedAt.Format(time.RFC3339),
					Reason:      containerStatus.State.Terminated.Reason,
				}
			}

			if containerStatus.LastTerminationState.Waiting != nil {
				podItem.Status.InitContainerStatuses[i].LastTerminationState.Waiting = &ContainerStateWaiting{
					Message: containerStatus.LastTerminationState.Waiting.Message,
					Reason:  containerStatus.LastTerminationState.Waiting.Reason,
				}
			}
			if containerStatus.LastTerminationState.Running != nil {
				podItem.Status.InitContainerStatuses[i].LastTerminationState.Running = &ContainerStateRunning{
					StartedAt: containerStatus.LastTerminationState.Running.StartedAt.Format(time.RFC3339),
				}
			}
			if containerStatus.LastTerminationState.Terminated != nil {
				podItem.Status.InitContainerStatuses[i].LastTerminationState.Terminated = &ContainerStateTerminated{
					ContainerID: containerStatus.LastTerminationState.Terminated.ContainerID,
					ExitCode:    containerStatus.LastTerminationState.Terminated.ExitCode,
					FinishedAt:  containerStatus.LastTerminationState.Terminated.FinishedAt.Format(time.RFC3339),
					StartedAt:   containerStatus.LastTerminationState.Terminated.StartedAt.Format(time.RFC3339),
					Reason:      containerStatus.LastTerminationState.Terminated.Reason,
				}
			}

		}
	}
	if pod.Status.ContainerStatuses != nil && len(pod.Status.ContainerStatuses) > 0 {
		podItem.Status.ContainerStatuses = make([]ContainerStatus, len(pod.Status.ContainerStatuses))
		for i, containerStatus := range pod.Status.ContainerStatuses {
			podItem.Status.ContainerStatuses[i].ContainerID = containerStatus.ContainerID
			podItem.Status.ContainerStatuses[i].Name = containerStatus.Name
			podItem.Status.ContainerStatuses[i].RestartCount = containerStatus.RestartCount

			if containerStatus.State.Waiting != nil {
				podItem.Status.ContainerStatuses[i].State.Waiting = &ContainerStateWaiting{
					Message: containerStatus.State.Waiting.Message,
					Reason:  containerStatus.State.Waiting.Reason,
				}
			}
			if containerStatus.State.Running != nil {
				podItem.Status.ContainerStatuses[i].State.Running = &ContainerStateRunning{
					StartedAt: containerStatus.State.Running.StartedAt.Format(time.RFC3339),
				}
			}
			if containerStatus.State.Terminated != nil {
				podItem.Status.ContainerStatuses[i].State.Terminated = &ContainerStateTerminated{
					ContainerID: containerStatus.State.Terminated.ContainerID,
					ExitCode:    containerStatus.State.Terminated.ExitCode,
					FinishedAt:  containerStatus.State.Terminated.FinishedAt.Format(time.RFC3339),
					StartedAt:   containerStatus.State.Terminated.StartedAt.Format(time.RFC3339),
					Reason:      containerStatus.State.Terminated.Reason,
				}
			}

			if containerStatus.LastTerminationState.Waiting != nil {
				podItem.Status.ContainerStatuses[i].LastTerminationState.Waiting = &ContainerStateWaiting{
					Message: containerStatus.LastTerminationState.Waiting.Message,
					Reason:  containerStatus.LastTerminationState.Waiting.Reason,
				}
			}
			if containerStatus.LastTerminationState.Running != nil {
				podItem.Status.ContainerStatuses[i].LastTerminationState.Running = &ContainerStateRunning{
					StartedAt: containerStatus.LastTerminationState.Running.StartedAt.Format(time.RFC3339),
				}
			}
			if containerStatus.LastTerminationState.Terminated != nil {
				podItem.Status.ContainerStatuses[i].LastTerminationState.Terminated = &ContainerStateTerminated{
					ContainerID: containerStatus.LastTerminationState.Terminated.ContainerID,
					ExitCode:    containerStatus.LastTerminationState.Terminated.ExitCode,
					FinishedAt:  containerStatus.LastTerminationState.Terminated.FinishedAt.Format(time.RFC3339),
					StartedAt:   containerStatus.LastTerminationState.Terminated.StartedAt.Format(time.RFC3339),
					Reason:      containerStatus.LastTerminationState.Terminated.Reason,
				}
			}

		}
	}
	return podItem
}

func watchPods() {
	for {
		if ResourceVersion == "" {
			var podsList *v1.PodList
			var err error
			podsList, err = ClientSet.CoreV1().Pods("").List(context.TODO(), metav1.ListOptions{Limit: POD_CHUNK_SIZE})
			if err != nil {
				Log("get pods failed with an error : %s", err.Error())
				time.Sleep(time.Second * 5) // avoid bombading the API server if its broken. // TODO - implement exponential backoff
				ResourceVersion = ""
				continue
			}
			Log("pod count : %d \n", len(podsList.Items))
			for _, pod := range podsList.Items {
				//PodItemsCache.Set(string(pod.UID), getOptimizedPodItem(&pod), cache.NoExpiration)
				rwLock.Lock()
				PodItemsCache[string(pod.UID)] = getOptimizedPodItem(&pod)
				rwLock.Unlock()
			}
			ResourceVersion = podsList.ResourceVersion
			continueToken := podsList.Continue
			podsList = nil
			for continueToken != "" {
				podsList, err = ClientSet.CoreV1().Pods("").List(context.TODO(), metav1.ListOptions{Limit: POD_CHUNK_SIZE, Continue: continueToken})
				if err != nil {
					Log("get pods failed with an error : %s", err.Error())
					continueToken = ""
					podsList = nil
				} else {
					Log("pod count : %d \n", len(podsList.Items))
					for _, pod := range podsList.Items {
						//PodItemsCache.Set(string(pod.UID), getOptimizedPodItem(&pod), cache.NoExpiration)
						rwLock.Lock()
						PodItemsCache[string(pod.UID)] = getOptimizedPodItem(&pod)
						rwLock.Unlock()
					}
					continueToken = podsList.Continue
					podsList = nil
				}
			}
			Log("continue: %s \n", continueToken)
			Log("resource version: %s \n", ResourceVersion)
		}
		Log("Total Pod count: %d \n", len(PodItemsCache))

		listOptions := metav1.ListOptions{AllowWatchBookmarks: true}
		if ResourceVersion != "" {
			listOptions.ResourceVersion = ResourceVersion
		}
		Log("Establishing watch connection @  %s with ResourceVersion: %s \n", time.Now().UTC().String(), ResourceVersion)
		fmt.Printf("Establishing watch connection @  %s with ResourceVersion: %s \n", time.Now().UTC().String(), ResourceVersion)
		watcher, err := ClientSet.CoreV1().Pods(v1.NamespaceAll).Watch(context.Background(), listOptions)
		if err != nil {
			Log("watch connection failed and reconnecting Watch after List: %s \n", err.Error())
			time.Sleep(time.Millisecond * 500)
			ResourceVersion = "" // validate, is this right thing to do considering performance reasons
			continue
		}

		for event := range watcher.ResultChan() {
			meta, err := meta.Accessor(event.Object)
			if err != nil {
				Log("%s: unable to understand watch event", event.Type)
				continue
			}
			ResourceVersion = meta.GetResourceVersion()
			Log("Event receive version : %s @ %s \n", ResourceVersion, time.Now().UTC().String())
			fmt.Printf("Event receive version : %s @ %s \n", ResourceVersion, time.Now().UTC().String())
			switch event.Type {
			case watch.Bookmark:
				Log("BookMark event received \n")
			case watch.Error:
				Log("Error event received \n")
			case watch.Added:
				pod := event.Object.(*v1.Pod)
				rwLock.Lock()
				PodItemsCache[string(pod.UID)] = getOptimizedPodItem(pod)
				rwLock.Unlock()
				Log("Pod %s/%s added \n", pod.ObjectMeta.Namespace, pod.ObjectMeta.Name)
			case watch.Modified:
				pod := event.Object.(*v1.Pod)
				rwLock.Lock()
				PodItemsCache[string(pod.UID)] = getOptimizedPodItem(pod)
				rwLock.Unlock()
				Log("Pod %s/%s modified \n", pod.ObjectMeta.Namespace, pod.ObjectMeta.Name)
			case watch.Deleted:
				pod := event.Object.(*v1.Pod)
				rwLock.Lock()
				delete(PodItemsCache, string(pod.UID))
				rwLock.Unlock()
				Log("Pod %s/%s deleted \n", pod.ObjectMeta.Namespace, pod.ObjectMeta.Name)
			}
		}
	}
}

func pods(w http.ResponseWriter, r *http.Request) {
	switch r.Method {
	case "GET":
		fmt.Printf("get Pods invoked :%s \n", time.Now().UTC().String())

		podsResponse := make(map[string][]PodItem)
		rwLock.RLock()
		Log("get Pods invoked with Pod count :%d \n", len(PodItemsCache))
		podsResponse["items"] = make([]PodItem, 0, len(PodItemsCache))
		for _, podItem := range PodItemsCache {
			podsResponse["items"] = append(podsResponse["items"], podItem)
		}
		rwLock.RUnlock()
		j, _ := json.Marshal(podsResponse)
		w.Write(j)
	default:
		w.WriteHeader(http.StatusMethodNotAllowed)
		fmt.Fprintf(w, "Not supported method")
	}
}

func main() {
	flag.Parse()

	config, err := rest.InClusterConfig()
	if err != nil {
		Log(err.Error())
		panic(err.Error())
	}

	ClientSet, err = kubernetes.NewForConfig(config)
	if err != nil {
		log.Fatal(err.Error())
	}
	go watchPods()

	http.HandleFunc("/pods", pods)
	//http.HandleFunc("/nodes", nodes)

	Log("Starting Kuberenets Proxy Server")
	http.ListenAndServe(":8080", nil)
}
