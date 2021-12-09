package main

type PodItem struct {
	Metadata PodMetaData `json:"metadata"`
	Spec     PodSpec     `json:"spec"`
	Status   PodStatus   `json:"status"`
}

type PodMetaData struct {
	Name              string             `json:"name"`
	Namespace         string             `json:"namespace"`
	ResourceVersion   string             `json:"ResourceVersion"`
	CreationTimestamp string             `json:"creationTimestamp"`
	UID               string             `json:"uid"`
	DeletionTimestamp string             `json:"deletionTimestamp,omitempty"`
	Annotations       *map[string]string `json:"annotations,omitempty"`
	Labels            *map[string]string `json:"labels"`
	OwnerReferences   []OwnerReference   `json:"ownerReferences,omitempty"`
}

type PodSpec struct {
	NodeName       string      `json:"nodeName"`
	Containers     []Container `json:"containers"`
	InitContainers []Container `json:"initContainers"`
}
type PodStatus struct {
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

type OwnerReference struct {
	Name string `json:"name"`
	Kind string `json:"kind"`
}
