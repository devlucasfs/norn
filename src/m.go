package main

import (
	"os"
)

func main() {
	info, err := GetInfo()
	if err != nil {
		DefaultOutputs.Fatal(err)
	}

	LexerInit()
	if err := os.WriteFile(info.Output, Parser(info), 0644); err != nil {
		DefaultOutputs.Fatal(err)
	}
}
