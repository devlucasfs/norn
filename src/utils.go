package main

import (
	"reflect"
	"slices"
)

func IsSame[T any, U any]() bool {
	return reflect.TypeOf((*T)(nil)).Elem() == reflect.TypeOf((*U)(nil)).Elem()
}

func AssertExpect(file *CarlaFile, err error, kinds ...uint64) Token {
	data := file.Next()

	if slices.Contains(kinds, data.Kind) {
		return data
	}

	DefaultOutputs.Expected(data, err, kinds)
	return Token{}
}
