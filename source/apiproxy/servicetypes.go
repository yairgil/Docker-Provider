package main

type ServiceItem struct {
	Metadata ServiceMetaData `json:"metadata"`
	Spec     ServiceSpec     `json:"spec"`
}

type ServiceMetaData struct {
	Name      string `json:"name"`
	Namespace string `json:"namespace"`
}

type ServiceSpec struct {
	Selector  map[string]string `json:"selector,omitempty"`
	ClusterIP string            `json:"clusterIP,omitempty"`
	Type      string            `json:"type,omitempty"`
}
