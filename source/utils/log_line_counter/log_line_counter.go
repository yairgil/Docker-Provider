package main

import (
	"fmt"
	"os"
	"strings"

	"github.com/hpcloud/tail"
)

func main() {

	t, err := tail.TailFile(os.Args[1], tail.Config{Follow: true, MustExist: false, Poll: true})
	if err != nil {
		fmt.Printf("error opening file: %s", err.Error())
	}
	var linecount = 0
	for line := range t.Lines {
		if line.Err != nil {
			fmt.Printf("error reading file: %s", err.Error())
		}
		linecount += 1
		// fmt.Printf("read line %s, line count is now %d\n", line.Text, linecount)

		if strings.HasPrefix(line.Text, "eof") {
			fmt.Printf("read %d lines\n", linecount)
			os.Exit(linecount)
		}
	}

	os.Exit(linecount)
}
