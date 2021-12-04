package main

import (
	"encoding/json"
	"flag"
	"fmt"
	"log"
	"net/http"
	"os"
	"time"

	lumberjack "gopkg.in/natefinch/lumberjack.v2"
	v1 "k8s.io/api/core/v1"
	"k8s.io/apimachinery/pkg/labels"
	"k8s.io/client-go/informers"
	coreinformers "k8s.io/client-go/informers/core/v1"
	"k8s.io/client-go/kubernetes"
	"k8s.io/client-go/rest"

	//"k8s.io/client-go/pkg/api/v1"

	"k8s.io/client-go/tools/cache"
)

var kubeconfig string

// func init() {
// 	flag.StringVar(&kubeconfig, "kubeconfig", "/home/sshadmin/learn-go/gangams-aks-hyperscale-test", "absolute path to the kubeconfig file")
// }

func Run(stopCh chan struct{}) error {
	// Starts all the shared informers that have been created by the factory so
	// far.
	Log("Starting informerFactory")
	informerFactory.Start(stopCh)
	// wait for the initial synchronization of the local cache.
	//if !cache.WaitForCacheSync(stopCh, podInformer.Informer().HasSynced, nodeInformer.Informer().HasSynced) {
	if !cache.WaitForCacheSync(stopCh, podInformer.Informer().HasSynced) {
		errorMessage := "Failed to sync"
		Log(errorMessage)
		return fmt.Errorf(errorMessage)
	}
	Log("Successfully synced the cache")

	return nil
}

var (
	informerFactory informers.SharedInformerFactory
	podInformer     coreinformers.PodInformer

//	nodeInformer    coreinformers.NodeInformer
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

func getOptimizedPodItems(pods []*v1.Pod) {
	for _, pod := range pods {
		if pod.ManagedFields != nil {
			pod.ManagedFields = nil
		}
		if pod.GenerateName != "" {
			pod.GenerateName = ""
		}
		if pod.Spec.Volumes != nil {
			pod.Spec.Volumes = nil
		}
		if pod.Spec.Affinity != nil {
			pod.Spec.Affinity = nil
		}
		if pod.Spec.Tolerations != nil {
			pod.Spec.Tolerations = nil
		}
		if pod.Spec.SecurityContext != nil {
			pod.Spec.SecurityContext = nil
		}
		if pod.Spec.RestartPolicy != "" {
			pod.Spec.RestartPolicy = ""
		}
		if pod.Spec.TerminationGracePeriodSeconds != nil {
			pod.Spec.TerminationGracePeriodSeconds = nil
		}
		if pod.Spec.DNSPolicy != "" {
			pod.Spec.DNSPolicy = ""
		}
		if pod.Spec.ServiceAccountName != "" {
			pod.Spec.ServiceAccountName = ""
		}
		if pod.Spec.PriorityClassName != "" {
			pod.Spec.PriorityClassName = ""
		}
		if pod.Spec.SchedulerName != "" {
			pod.Spec.SchedulerName = ""
		}
		if pod.Spec.Priority != nil {
			pod.Spec.Priority = nil
		}
		if pod.Spec.EnableServiceLinks != nil {
			pod.Spec.EnableServiceLinks = nil
		}
		if pod.Spec.PreemptionPolicy != nil {
			pod.Spec.PreemptionPolicy = nil
		}

		if pod.Spec.Containers != nil && len(pod.Spec.Containers) > 0 {
			for index := 0; index < len(pod.Spec.Containers); index++ {
				if pod.Spec.Containers[index].SecurityContext != nil {
					pod.Spec.Containers[index].SecurityContext = nil
				}
				if pod.Spec.Containers[index].Command != nil {
					pod.Spec.Containers[index].Command = nil
				}
				if pod.Spec.Containers[index].Env != nil {
					pod.Spec.Containers[index].Env = nil
				}
				if pod.Spec.Containers[index].LivenessProbe != nil {
					pod.Spec.Containers[index].LivenessProbe = nil
				}
				if pod.Spec.Containers[index].VolumeMounts != nil {
					pod.Spec.Containers[index].VolumeMounts = nil
				}
				if pod.Spec.Containers[index].TerminationMessagePath != "" {
					pod.Spec.Containers[index].TerminationMessagePath = ""
				}
				if pod.Spec.Containers[index].TerminationMessagePolicy != "" {
					pod.Spec.Containers[index].TerminationMessagePolicy = ""
				}
				if pod.Spec.Containers[index].ImagePullPolicy != "" {
					pod.Spec.Containers[index].ImagePullPolicy = ""
				}

			}
		}
		if pod.Status.PodIPs != nil {
			pod.Status.PodIPs = nil
		}
		if pod.Status.QOSClass != "" {
			pod.Status.QOSClass = ""
		}
	}
}

func pods(w http.ResponseWriter, r *http.Request) {
	switch r.Method {
	case "GET":
		Log("get Pods invoked")
		pods, err := podInformer.Lister().List(labels.Everything())
		if err != nil {
			errorMessage := fmt.Sprintf("error on getting pods list: %s", err.Error())
			w.WriteHeader(http.StatusInternalServerError)
			fmt.Fprintf(w, errorMessage)
		} else {
			getOptimizedPodItems(pods)
			j, _ := json.Marshal(pods)
			w.Write(j)
		}
	default:
		w.WriteHeader(http.StatusMethodNotAllowed)
		fmt.Fprintf(w, "Not supported method")
	}
}

// func nodes(w http.ResponseWriter, r *http.Request) {
// 	switch r.Method {
// 	case "GET":
// 		Log("get nodes invoked @ %s \n", time.Now().UTC().String())
// 		nodes, err := nodeInformer.Lister().List(labels.Everything())
// 		if err != nil {
// 			Log("error on getting nodes list")
// 		}
// 		if err != nil {
// 			errorMessage := fmt.Sprintf("error on getting nodes list: %s", err.Error())
// 			w.WriteHeader(http.StatusInternalServerError)
// 			fmt.Fprintf(w, errorMessage)
// 		} else {
// 			j, _ := json.Marshal(nodes)
// 			w.Write(j)
// 		}
// 	default:
// 		w.WriteHeader(http.StatusMethodNotAllowed)
// 		fmt.Fprintf(w, "Not supported method")
// 	}
// }

func main() {
	flag.Parse()
	createLogger()

	// config, err := clientcmd.BuildConfigFromFlags("", kubeconfig)
	// if err != nil {
	// 	Log(err.Error())
	// }

	config, err := rest.InClusterConfig()
	if err != nil {
		Log(err.Error())
		panic(err.Error())
	}

	clientset, err := kubernetes.NewForConfig(config)
	if err != nil {
		Log(err.Error())
	}
	Log("Successfully created clientSet")

	informerFactory = informers.NewSharedInformerFactory(clientset, time.Hour*24)
	Log("Successfully created informerFactory")

	podInformer = informerFactory.Core().V1().Pods()
	Log("Successfully created podInformer")

	// nodeInformer = informerFactory.Core().V1().Nodes()
	// Log("Successfully created nodeInformer")

	podInformer.Informer().AddEventHandler(
		// Your custom resource event handlers.
		cache.ResourceEventHandlerFuncs{
			// Called on creation
			AddFunc: func(obj interface{}) {},
			// Called on resource update and every resyncPeriod on existing resources.
			UpdateFunc: func(old, new interface{}) {},
			// Called on resource deletion.
			DeleteFunc: func(obj interface{}) {},
		},
	)
	// nodeInformer.Informer().AddEventHandler(
	// 	// Your custom resource event handlers.
	// 	cache.ResourceEventHandlerFuncs{
	// 		// Called on creation
	// 		AddFunc: func(obj interface{}) {},
	// 		// Called on resource update and every resyncPeriod on existing resources.
	// 		UpdateFunc: func(old, new interface{}) {},
	// 		// Called on resource deletion.
	// 		DeleteFunc: func(obj interface{}) {},
	// 	},
	// )
	stop := make(chan struct{})
	defer close(stop)

	if Run(stop) == nil {
		Log("Successfully started all the shared informers")
	} else {
		Log("Either starting of the shared informers or cache sync failed")
	}

	http.HandleFunc("/pods", pods)
	//http.HandleFunc("/nodes", nodes)

	Log("Starting Kuberenets API Proxy Server")
	http.ListenAndServe(":8080", nil)
}
