package main

import (
	"errors"
	"os"
	"path/filepath"
	"runtime"
	"strconv"
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

func Test_Make_AddressableMap(t *testing.T) {
	qs := Make_AddressableMap()
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

func Test_AddressableMapStuff(t *testing.T) {
	qs := Make_AddressableMap()
	first_index, first_new := qs.get("never")
	*first_index = 1
	second_index, second_new := qs.get("gonna")
	*second_index = 2
	third_index, third_new := qs.get("give")
	*third_index = 3

	a, b := qs.get("never")
	assert(t, first_new, "first_new == true")
	assert(t, b == false, "b == false")
	assert(t, *a == 1, "a != 1")

	a, b = qs.get("gonna")
	assert(t, second_new, "second_new == true")
	assert(t, b == false, "b == false")
	assert(t, *a == 2, "a != 2")

	a, b = qs.get("give")
	assert(t, third_new, "third_new == true")
	assert(t, b == false, "b == false")
	assert(t, *a == 3, "a != 3")

	qs.delete("gonna")
	if qs.container_identifiers[1] != "" {
		t.Error("qs.container_identifiers[1] != \"\"")
	}
	assert(t, len(qs.free_list) == 1, `len(qs.free_list) == 1`)

	fourth_index, fourth_new := qs.get("up")
	*fourth_index = 4
	a, b = qs.get("up")
	assert(t, fourth_new, "fourth_new != true")
	assert(t, b == false, "b == false")
	assert(t, *a == 4, "a != 4")

	assert(t, qs.container_identifiers[0] == "never", `qs.container_identifiers[0] == "never"`)
	assert(t, qs.container_identifiers[1] == "up", `qs.container_identifiers[1] == "up"`)
	assert(t, qs.container_identifiers[2] == "give", `qs.container_identifiers[2] == "give"`)

	assert(t, qs.log_counts[0] == 1, `qs.log_counts[0] == 1`)
	assert(t, qs.log_counts[1] == 4, `qs.log_counts[1] == 4`)
	assert(t, qs.log_counts[2] == 3, `qs.log_counts[2] == 3`)

	assert(t, len(qs.free_list) == 0, `len(qs.free_list) == 0`)

	qs.delete("never")
	assert(t, qs.container_identifiers[0] == "", `qs.container_identifiers[0] != ""`)
	assert(t, qs.container_identifiers[1] == "up", `qs.container_identifiers[1] != "up"`)
	assert(t, qs.container_identifiers[2] == "give", `qs.container_identifiers[2] != "give"`)
	assert(t, len(qs.free_list) == 1, `len(qs.free_list) == 1`)

	qs.delete("up")
	assert(t, len(qs.free_list) == 2, `len(qs.free_list) == 2`)

	qs.delete("give")
	assert(t, len(qs.free_list) == 3, `len(qs.free_list) == give`)
	assert(t, len(qs.string_to_arr_index) == 0, `len(qs.string_to_arr_index) == 0`)
}

func Test_duplicate_addressable_map(t *testing.T) {
	qs1 := Make_AddressableMap()
	a, b := qs1.export_values()
	assert(t, len(a) == 0, `len(a) == 0`)
	assert(t, len(b) == 0, `len(b) == 0`)

	qs2 := Make_AddressableMap()
	first_index, _ := qs2.get("never")
	*first_index = 1
	second_index, _ := qs2.get("gonna")
	*second_index = 2
	third_index, _ := qs2.get("give")
	*third_index = 3
	qs2.delete("gonna")

	get_index := func(str_slice []string, target_val string) int {
		for ind, val := range str_slice {
			if val == target_val {
				return ind
			}
		}
		panic("string not found in slice")
	}

	a, b = qs2.export_values()
	assert(t, len(b) == 2, `qs2_copy.len(b) == 2`)
	assert(t, b[get_index(a, "never")] == 1, `b[get_index(a, "never")] == 1`)
	assert(t, b[get_index(a, "give")] == 3, `b[get_index(a, "give")] == 3`)
}

func Test_slice_contains_str(t *testing.T) {
	a := []string{}
	if slice_contains_str(a, "asdf") {
		t.Errorf("!slice_contains_str(%v, \"asdf\")", a)
	}

	b := []string{"asdf", "asdfasdf", "asdfasdfasdf"}
	if !slice_contains_str(b, "asdf") {
		t.Errorf("!slice_contains_str(%v, \"asdf\")", b)
	}
	if !slice_contains_str(b, "asdfasdf") {
		t.Errorf("!slice_contains_str(%v, \"asdf\")", b)
	}
	if !slice_contains_str(b, "asdfasdfasdf") {
		t.Errorf("!slice_contains_str(%v, \"asdf\")", b)
	}
	if slice_contains_str(b, "foobar") {
		t.Errorf("!slice_contains_str(%v, \"asdf\")", b)
	}
	if slice_contains_str(b, "") {
		t.Errorf("!slice_contains_str(%v, \"asdf\")", b)
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
