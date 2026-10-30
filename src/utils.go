package main

import (
	"fmt"
	"reflect"
	"slices"
)

func IsSame[T any, U any]() bool {
	return reflect.TypeOf((*T)(nil)).Elem() == reflect.TypeOf((*U)(nil)).Elem()
}

func expectLex(file *CarlaFile, lex string) Token {
    token := file.Next()

    if token.Lex != lex {
        DefaultOutputs.Fatal(fmt.Sprintf(
            "Expected '%s', got '%s' at line %d",
            lex, token.Lex, token.Line,
        ))
    }

    return token
}

func AssertExpect(file *CarlaFile, err error, kinds ...uint64) Token {
	data := file.Next()

	if slices.Contains(kinds, data.Kind) {
		return data
	}

	DefaultOutputs.Expected(data, err, kinds)
	return Token{}
}
