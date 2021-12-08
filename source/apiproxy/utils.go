package main

import (
	"fmt"
	"log"
	"os"
	"time"

	"gopkg.in/natefinch/lumberjack.v2"
	v1 "k8s.io/api/core/v1"
)

func createLogger() *log.Logger {
	var logfile *os.File
	logPath := "apiproxy.log"

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
