package main

import (
	"fmt"
	"os"
)

type Info = struct {
	Bin    string
	Main   string
	Output string
}

func GetInfo() (*Info, error) {
	args := os.Args
	len := len(args)

	if len < 3 {
		return nil, fmt.Errorf("Expected at least 4 arguments. Received %d", len)
	}

	content := Info{
		Bin:    args[0],
		Main:   args[2],
		Output: args[3],
	}

	return &content, nil
}
