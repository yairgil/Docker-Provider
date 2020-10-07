package main

import (
	"bufio"
	"log"
	"os"
	"testing"
)

func TestSum(t *testing.T) {
	var jsonMaps []map[interface{}]interface{}

	file, err := os.Open("/var/log/kube-audit")
	if err != nil {
		log.Fatal(err)
	}
	defer file.Close()

	scanner := bufio.NewScanner(file)
	for scanner.Scan() {
		line := scanner.Text()
		jsonMap := make(map[interface{}]interface{})
		jsonMap["AuditLog"] = line
		// err := json.Unmarshal([]byte(line), &jsonMap)
		// if err != nil {
		// 	panic(err)
		// }
		jsonMaps = append(jsonMaps, jsonMap)
	}

	// jsonMapsParsed := make([]map[interface{}]interface{}, len(jsonMaps))
	// for i, v := range jsonMaps {
	// 	jsonMapParsed := make(map[interface{}]interface{})
	// 	for i2, v2 := range v {
	// 		jsonMapParsed[i2] = v2
	// 	}
	// 	jsonMapsParsed[i] = jsonMapParsed
	// }
	PostDataHelper(jsonMaps)
	if err := scanner.Err(); err != nil {
		log.Fatal(err)
	}
	// if total != 10 {
	// 	t.Errorf("Sum was incorrect, got: %d, want: %d.", total, 10)
	// }
}

// func TestConvertToStringMap(t *testing.T) {
// 	input1 := map[interface{}]interface{}{"1": "1", 1: 2, false: 3}
// 	ConvertToStringMap(input1)
// }

func TestX(t *testing.T) {
	// (map[interface {}]interface {}) (len=18) {
	// 	(string) (len=4) "kind": ([]uint8) (len=5 cap=5) {
	// 	 00000000  45 76 65 6e 74                                    |Event|
	// 	},
	// 	(string) (len=10) "apiVersion": ([]uint8) (len=15 cap=15) {
	// 	 00000000  61 75 64 69 74 2e 6b 38  73 2e 69 6f 2f 76 31     |audit.k8s.io/v1|
	// 	},
	input1 := map[interface{}]interface{}{"kind": []uint8{69, 118, 101, 110, 116}, "apiVersion": []uint8{97, 117, 100, 105, 116, 46, 107, 56, 115, 46, 105, 111, 47, 118, 49}, "user": []uint8{69, 118, 101, 110, 116}}
	inputLst := []map[interface{}]interface{}{input1}
	PostDataHelper(inputLst)
}
