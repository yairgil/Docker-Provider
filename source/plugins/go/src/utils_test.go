package main

import (
	"errors"
	"fmt"
	"os"
	"path/filepath"
	"testing"
)

func Test_isValidUrl(t *testing.T) {
	type test_struct struct {
		isValid bool
		url     string
	}

	tests := []test_struct{
		{true, "https://www.microsoft.com"},
		{true, "http://abc.xyz"},
		{true, "https://www.microsoft.com/tests"},
		{false, "()"},
		{false, "https//www.microsoft.com"},
		{false, "https:/www.microsoft.com"},
		{false, "https:/www.microsoft.com*"},
		{false, ""},
	}

	for _, tt := range tests {
		t.Run(tt.url, func(t *testing.T) {
			got := isValidUrl(tt.url)
			if got != tt.isValid {
				t.Errorf("isValidUrl(%s) = %t, want %t", tt.url, got, tt.isValid)
			}
		})
	}
}

func Test_ReadFileContents(t *testing.T) {
	type mock_struct struct {
		expectedFilePath string
		fileContents     []byte
		err              error
	}
	type test_struct struct {
		testname       string
		calledFilePath string
		subcall_spec   mock_struct
		output         string
		err            bool
	}

	tests := []test_struct{
		{"normal", "foobar.txt", mock_struct{"foobar.txt", []byte("asdf"), nil}, "asdf", false},
		{"extra whitespace", "foobar.txt ", mock_struct{"foobar.txt", []byte("asdf   \t"), nil}, "asdf", false},
		{"empty filename", "", mock_struct{"", []byte(""), nil}, "", true},
		{"file doesn't exist", "asdf.txt", mock_struct{"asdf", []byte(""), errors.New("this error doesn't matter much")}, "", true},
	}

	for _, tt := range tests {
		t.Run(string(tt.testname), func(t *testing.T) {

			readfileFunc := func(filename string) ([]byte, error) {
				if filename == tt.subcall_spec.expectedFilePath {
					return tt.subcall_spec.fileContents, nil
				}
				return []byte(""), errors.New("file not found")
			}

			got, err := ReadFileContentsImpl(tt.calledFilePath, readfileFunc)

			if got != tt.output || !(tt.err == (err != nil)) {
				t.Errorf("ReadFileContents(%v) = (%v, %v), want (%v, %v)", tt.calledFilePath, got, err, tt.output, tt.err)
				if got != tt.output {
					t.Errorf("output strings are not equal")
				}
				if tt.err == (err != nil) {
					t.Errorf("errors are not equal")
				}
			}
		})
	}
}

func Test_GetSizeOfAllFilesInDir(t *testing.T) {
	pwd, err := os.Getwd()
	if err != nil {
		panic(err) // HAHAHAHAHAHAHAHAHAHA
	}

	// if we upgrade to golang 1.15 then the testing lib will automatically create and clean up a temprorary dir for us
	testdir := filepath.Join(pwd, "TempTestyboiDir")
	os.Mkdir(testdir, 0777)

	defer func() {
		err := os.RemoveAll(testdir)
		if err != nil {
			t.Error("could not remove temp dir, future test runs will fail:", err)
		}
	}()

	first_result := GetSizeOfAllFilesInDir(testdir)
	if len(first_result) != 0 {
		t.Errorf("GetSizeOfAllFilesInDir returned incorrect result for empty dir: %v", first_result)
	}

	file, err := os.Create(filepath.Join(testdir, "a.txt"))
	if err != nil {
		t.Error(err)
	}
	linesToWrite := []string{"aaaaaaaaaa"}
	for _, line := range linesToWrite {
		file.WriteString(line)
	}
	file.Close()

	second_result := GetSizeOfAllFilesInDir(testdir)
	if len(second_result) != 1 {
		t.Error("GetSizeOfAllFilesInDir returned incorrect result for dir with one file:", second_result)
	}
	if second_result["a.txt"] != 10 {
		t.Error("GetSizeOfAllFilesInDir returned incorrect result for dir with one file:", second_result)
	}

	os.Mkdir(filepath.Join(testdir, "asdf"), 0777)
	file, err = os.Create(filepath.Join(testdir, "asdf", "b.txt"))
	if err != nil {
		t.Error(err)
	}
	linesToWrite = []string{"aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"}
	for _, line := range linesToWrite {
		file.WriteString(line)
	}
	file.Close()

	third_result := GetSizeOfAllFilesInDir(testdir)
	if len(third_result) != 2 {
		t.Error("GetSizeOfAllFilesInDir returned incorrect result for dir with two files:", third_result)
	}
	if third_result["a.txt"] != 10 {
		t.Error("GetSizeOfAllFilesInDir returned incorrect result for dir with two files:", third_result)
	}
	if third_result[filepath.Join("asdf", "b.txt")] != 41+1 {
		t.Error("GetSizeOfAllFilesInDir returned incorrect result for dir with two files:", third_result)
	}
}

func Test_Make_QuickDeleteSlice(t *testing.T) {
	qs := Make_QuickDeleteSlice()
	if qs.container_identifiers == nil {
		t.Error("qs.container_identifiers == nil")
	}

	if qs.free_list == nil {
		t.Error("qs.free_list == nil")
	}

	if qs.log_counts == nil {
		t.Error("qs.log_counts == nil")
	}
}

func Test_QuickDeleteSliceStuff(t *testing.T) {
	qs := Make_QuickDeleteSlice()
	first_index := qs.insert_new_container("never")
	second_index := qs.insert_new_container("gonna")
	third_index := qs.insert_new_container("give")

	fmt.Print(qs.log_counts)
	fmt.Print(qs.container_identifiers)
	fmt.Print(qs.free_list)

	if qs.container_identifiers[first_index] != "never" {
		t.Error("qs.container_identifiers[first_index] != \"never\"")
	}
	if qs.container_identifiers[second_index] != "gonna" {
		t.Error("qs.container_identifiers[second_index] != \"gonna\"")
	}
	if qs.container_identifiers[third_index] != "give" {
		t.Error("qs.container_identifiers[third_index] != \"give\"")
	}

	qs.remove_index(second_index)
	if qs.container_identifiers[second_index] != "" {
		t.Error("qs.container_identifiers[second_index] != \"\"")
	}

	fourth_index := qs.insert_new_container("up")
	if qs.container_identifiers[fourth_index] != "up" {
		t.Error("qs.container_identifiers[fourth_index] != \"up\"")
	}

	qs.remove_index(first_index)
	if qs.container_identifiers[first_index] != "" {
		t.Error("qs.container_identifiers[first_index] != \"\"")
	}

	if qs.container_identifiers[third_index] != "give" {
		t.Error("qs.container_identifiers[third_index] != \"give\"  (second)")
	}
}

func Test_slice_contains(t *testing.T) {
	a := []string{}
	if slice_contains(a, "asdf") {
		t.Errorf("!slice_contains(%v, \"asdf\")", a)
	}

	b := []string{"asdf", "asdfasdf", "asdfasdfasdf"}
	if !slice_contains(b, "asdf") {
		t.Errorf("!slice_contains(%v, \"asdf\")", b)
	}
	if !slice_contains(b, "asdfasdf") {
		t.Errorf("!slice_contains(%v, \"asdf\")", b)
	}
	if !slice_contains(b, "asdfasdfasdf") {
		t.Errorf("!slice_contains(%v, \"asdf\")", b)
	}
	if slice_contains(b, "foobar") {
		t.Errorf("!slice_contains(%v, \"asdf\")", b)
	}
	if slice_contains(b, "") {
		t.Errorf("!slice_contains(%v, \"asdf\")", b)
	}

}
