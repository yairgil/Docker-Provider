package main

type PodItem struct {
	Metadata PodMetaData `json:"metadata" msgpack:"metadata"`
	Spec     PodSpec     `json:"spec" msgpack:"spec"`
	Status   PodStatus   `json:"status" msgpack:"status"`
}

type PodMetaData struct {
	//_msgpack struct{} `msgpack:",omitempty"`

	Name              string             `json:"name" msgpack:"name"`
	Namespace         string             `json:"namespace" msgpack:"namespace"`
	ResourceVersion   string             `json:"ResourceVersion" msgpack:"ResourceVersion"`
	CreationTimestamp string             `json:"creationTimestamp" msgpack:"creationTimestamp"`
	UID               string             `json:"uid" msgpack:"uid"`
	DeletionTimestamp string             `json:"deletionTimestamp,omitempty" msgpack:"deletionTimestamp,omitempty"`
	Annotations       *map[string]string `json:"annotations,omitempty" msgpack:"annotations,omitempty"`
	Labels            *map[string]string `json:"labels" msgpack:"labels"`
	OwnerReferences   []OwnerReference   `json:"ownerReferences,omitempty" msgpack:"ownerReferences,omitempty"`
}

type PodSpec struct {
	//_msgpack       struct{}    `msgpack:",omitempty"`
	NodeName       string      `json:"nodeName" msgpack:"nodeName"`
	Containers     []Container `json:"containers" msgpack:"containers"`
	InitContainers []Container `json:"initContainers" msgpack:"initContainers"`
}
type PodStatus struct {
	//_msgpack struct{} `msgpack:",omitempty"`

	StartTime             string            `json:"startTime,omitempty" msgpack:"startTime,omitempty"`
	Reason                string            `json:"reason,omitempty" msgpack:"reason,omitempty"`
	PodIP                 string            `json:"podIP,omitempty" msgpack:"podIP,omitempty"`
	Phase                 string            `json:"phase,omitempty" msgpack:"phase,omitempty"`
	Conditions            []PodCondition    `json:"conditions,omitempty" msgpack:"conditions,omitempty"`
	InitContainerStatuses []ContainerStatus `json:"initContainerStatuses,omitempty" msgpack:"initContainerStatuses,omitempty" `
	ContainerStatuses     []ContainerStatus `json:"containerStatuses,omitempty" msgpack:"containerStatuses,omitempty"`
}

type PodCondition struct {
	//_msgpack struct{} `msgpack:",omitempty"`

	PodConditionType   string `json:"type,omitempty" msgpack:"type,omitempty"`
	Status             string `json:"status,omitempty" msgpack:"status,omitempty" `
	LastTransitionTime string `json:"lastTransitionTime,omitempty" msgpack:"lastTransitionTime,omitempty"`
}
type ContainerStatus struct {
	//_msgpack             struct{}       `msgpack:",omitempty"`
	ContainerID          string         `json:"containerID,omitempty" msgpack:"containerID,omitempty"`
	Name                 string         `json:"name" msgpack:"name"`
	RestartCount         int32          `json:"restartCount" msgpack:"restartCount"`
	State                ContainerState `json:"state,omitempty" msgpack:"state,omitempty"`
	LastTerminationState ContainerState `json:"lastState,omitempty" msgpack:"lastState,omitempty"`
}
type ContainerState struct {
	//_msgpack   struct{}                  `msgpack:",omitempty"`
	Waiting    *ContainerStateWaiting    `json:"waiting,omitempty" msgpack:"waiting,omitempty"`
	Running    *ContainerStateRunning    `json:"running,omitempty" msgpack:"running,omitempty"`
	Terminated *ContainerStateTerminated `json:"terminated,omitempty" msgpack:"terminated,omitempty"`
}

// ContainerStateWaiting is a waiting state of a container.
type ContainerStateWaiting struct {
	//_msgpack struct{} `msgpack:",omitempty"`
	Reason  string `json:"reason,omitempty" msgpack:"reason,omitempty"`
	Message string `json:"message,omitempty" msgpack:"message,omitempty"`
}

// ContainerStateRunning is a running state of a container.
type ContainerStateRunning struct {
	//_msgpack  struct{} `msgpack:",omitempty"`
	StartedAt string `json:"startedAt,omitempty" msgpack:"startedAt,omitempty"`
}

// ContainerStateTerminated is a terminated state of a container.
type ContainerStateTerminated struct {
	//_msgpack    struct{} `msgpack:",omitempty"`
	ExitCode    int32  `json:"exitCode" msgpack:"exitCode"`
	Reason      string `json:"reason,omitempty" msgpack:"reason,omitempty"`
	Message     string `json:"message,omitempty" msgpack:"message,omitempty"`
	StartedAt   string `json:"startedAt,omitempty" msgpack:"startedAt,omitempty"`
	FinishedAt  string `json:"finishedAt,omitempty" msgpack:"finishedAt,omitempty"`
	ContainerID string `json:"containerID,omitempty" msgpack:"containerID,omitempty"`
}

type Container struct {
	//_msgpack  struct{}             `msgpack:",omitempty"`
	Name      string               `json:"name" msgpack:"name"`
	Resources ResourceRequirements `json:"resources,omitempty" msgpack:"resources,omitempty"`
}
type ResourceRequirements struct {
	//_msgpack struct{}          `msgpack:",omitempty"`
	Limits   map[string]string `json:"limits,omitempty" msgpack:"limits,omitempty"`
	Requests map[string]string `json:"requests,omitempty" msgpack:"requests,omitempty"`
}

type OwnerReference struct {
	Name string `json:"name" msgpack:"name"`
	Kind string `json:"kind" msgpack:"kind"`
}
