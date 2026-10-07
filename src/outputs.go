package main

import (
	"fmt"
	"os"
)

var TokenKindNames = map[uint64]string{
	KUnknown:     "unknown",
	KLeftParen:   "(",
	KRightParen:  ")",
	KSemi:        ";",
	KComma:       "comma",
	KDettach:     "..",
	KSign:        "=",
	KGenericSign: ":=",
	KString:      "a string",
	KInteger:     "an integer",
	KFloat:       "a float",
	KNil:         "nil",
	KBool:        "a bool",
	KConst:       "const",
	KIdentifier:  "an identifier",
	KEOF:         "EOF",
}

type defaultOutputs struct{}

var DefaultOutputs defaultOutputs

func (defaultOutputs) Fatal(content any) {
	switch v := content.(type) {
	case error:
		fmt.Println("[Norn] " + v.Error())
	case string:
		fmt.Println("[Norn] " + v)
	}

	os.Exit(FatalOutputCode)
}

func (defaultOutputs) Expected(found Token, extra error, expected []uint64) {
	msg := "No extra context"
	if extra != nil {
		msg = extra.Error()
	}

	lastI := len(expected) - 1
	var console string
	for i, kind := range expected {
		if i == lastI && len(expected) > 1 {
			console += " or " + TokenKindNames[kind]
			break
		} else if i > 0 {
			console += ", "
		}

		console += TokenKindNames[kind]
	}

	suffix := ""
	if found.Kind == KIdentifier {
		suffix = fmt.Sprintf("(%s)", found.Lex)
	}

	DefaultOutputs.Fatal(
		fmt.Errorf(
			"Was expected %s but was found %s%s in line %d col %d.\n~> %s\n",
			console,
			TokenKindNames[found.Kind],
			suffix,
			found.Line+1, found.Col+1,
			msg,
		),
	)

	os.Exit(FatalOutputCode)
}
