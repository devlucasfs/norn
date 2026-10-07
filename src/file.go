package main

import (
	"fmt"
	"os"
)

type ReplaceTokens struct {
	Start Token
	End   Token
	Code  string
}

type CarlaFile struct {
	Slice []ReplaceTokens

	Position uint64
	Length   uint64
	Line     uint64
	Content  []byte
	back     []Token
}

func NewCarlaFile[T any](data T) CarlaFile {
	var _default CarlaFile

	if IsSame[T, string]() {
		path := any(data).(string)
		content, err := os.ReadFile(path)

		if err != nil {
			DefaultOutputs.Fatal(fmt.Sprintf("Can't read `%s` as a file.", path))
		}

		return CarlaFile{
			Position: 0,
			Length:   uint64(len(content)),
			Content:  content,
		}
	}

	return _default
}
