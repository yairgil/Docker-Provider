/*
This file is for commonly used methods in tests (like assert)
*/

package main

import (
	"os"
	"path"
	"runtime"
	"strconv"
	"testing"
)

func get_repo_root_dir() string {

	dir, err := os.Getwd()
	if err != nil {
		panic(err)
	}

	for {
		_, err := os.Stat(path.Join(dir, "ReleaseNotes.md"))
		if !os.IsNotExist(err) {
			return dir
		}

		dir = path.Join(dir, "..") // this actually removes the last directory in the path instead of appending /..

		if len(dir) <= 1 {
			panic("Not run in docker-provider repo")
		}
	}
}

func assert(t *testing.T, expression bool, error_message string) {
	if !expression {
		_, file, no, ok := runtime.Caller(1)
		if !ok {
			panic("couldn't get calling function (this is a test error, not a code error)")
		}
		t.Error("condition not satisfied at " + file + ":" + strconv.Itoa(no) + ": " + error_message)
	}
}
