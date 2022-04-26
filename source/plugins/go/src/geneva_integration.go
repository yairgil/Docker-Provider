package main

import (
	"context"
	"encoding/json"
	"fmt"
	"time"
)

type GenevaConfigs struct {
	APIVersion string `json:"apiVersion"`
	Items      []struct {
		APIVersion string `json:"apiVersion"`
		Kind       string `json:"kind"`
		Metadata   struct {
			Name            string `json:"name"`
			Namespace       string `json:"namespace"`
			ResourceVersion string `json:"resourceVersion"`
			UID             string `json:"uid"`
		} `json:"metadata"`
		Spec struct {
			GenevaAccount         string `json:"genevaaccount"`
			GenevaEnvironmentType string `json:"genevaenvironmentType"`
			GenevaNamespace       string `json:"genevanamespace"`
		} `json:"spec"`
	} `json:"items"`
	Kind string `json:"kind"`
}

type GenevaAccountConfig struct {
	GenevaAccount         string
	GenevaNamespace       string
	GenevaEnvironmentType string
}

func updateGenevaTenantConfig() {
	for ; true; <-GenevaTenantConfigRefreshTicker.C {
		_genevaAccountConfigMap := make(map[string]GenevaAccountConfig)
		_k8sNamespaceGenevaAccountMap := make(map[string]string)
		var responseBytes []byte
		var errorMessage string
		var err error
		for retryCount := 0; retryCount < MaxRetries; retryCount++ {
			responseBytes, err = ClientSet.RESTClient().Get().AbsPath("/apis/azmon.container.insights/v1/genevaconfigs").DoRaw(context.TODO())
			if err != nil {
				time.Sleep(time.Duration((retryCount+1)*100) * time.Millisecond)
				continue
			}
			break
		}
		if responseBytes != nil {
			var genevaconfigs GenevaConfigs
			err = json.Unmarshal(responseBytes, &genevaconfigs)
			if err != nil {
				errorMessage = fmt.Sprintf("updateGenevaTenantConfig: Error unmarshalling the crdResponseBytes: %s", err.Error())
				Log(errorMessage)
			} else {
				for _, item := range genevaconfigs.Items {
					genevaAccountConfig := GenevaAccountConfig{
						GenevaAccount:         item.Spec.GenevaAccount,
						GenevaEnvironmentType: item.Spec.GenevaEnvironmentType,
						GenevaNamespace:       item.Spec.GenevaNamespace,
					}
					_genevaAccountConfigMap[item.Spec.GenevaAccount] = genevaAccountConfig
					_k8sNamespaceGenevaAccountMap[item.Metadata.Namespace] = item.Spec.GenevaAccount
				}
				Log("Locking to update geneva tenant account config")
				GenevaConfigUpdateMutex.Lock()
				K8SNamespaceGenevaAccountMap = _k8sNamespaceGenevaAccountMap
				GenevaAccountConfigMap = _genevaAccountConfigMap
				GenevaConfigUpdateMutex.Unlock()
				Log("Unlocking to update geneva tenant account config")
			}
		}
	}
}
