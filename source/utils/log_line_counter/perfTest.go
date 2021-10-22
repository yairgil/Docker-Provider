// package main

// import (
// 	"fmt"
// 	"os"
// 	"path/filepath"
// 	"strings"
// )

// var seen_files map[string]string

// func main() {
// 	// var files []string

// 	root := "/var/log/pods"
// 	err := filepath.Walk(root, func(path string, info os.FileInfo, err error) error {
// 		if !info.IsDir() && strings.HasPrefix(info.Name(), "0.log.") {
// 			// files = append(files, path)

// 			// TODO: does path include the file name?

// 			previous_rotated_file, already_seen := seen_files[path]
// 			// if the previous log file has a different name
// 			if !already_seen && previous_rotated_file == info.Name() {
// 				seen_files[path] = info.Name()
// 				//TODO: count number of lines in file (maybe in goroutine?)
// 			}
// 		}
// 		return nil
// 	})

// 	if err != nil {
// 		panic(err)
// 	}

// 	for _, file := range seen_files {
// 		fmt.Println(file)
// 	}

// 	//TODO: go through seen_files every once in a while and remove old files
// 	// (need to decide when to do this, should only do it when there's a newer rotated log file from the same container).
// }
