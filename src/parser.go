package main

import (
	"errors"
	"fmt"
)

const semicolonError = "An expression need be finished with a ;"

var Ginfo *Info

func ConstImport(file *CarlaFile, identifier, _const Token) {
	parenError := "@import is an builtin statement who needs parenthesis to be used, as a function"
	AssertExpect(file, errors.New(parenError), KLeftParen)
	str := AssertExpect(file, errors.New(parenError), KString)
	AssertExpect(file, errors.New(parenError), KRightParen)
	semi := AssertExpect(file, errors.New(semicolonError), KSemi)

	ConstImportNamespace(file, identifier.Lex, str.Lex, _const, semi)
}

func PConst(file *CarlaFile, data Token) {
	namespace := AssertExpect(file, errors.New("A const need an identifier."), KIdentifier)
	sign := AssertExpect(file, nil, KGenericSign, KSign)

	switch sign.Kind {
	case KGenericSign:
		id := AssertExpect(file, nil, KIdentifier, KString, KInteger, KBool, KNil, KFloat)
		if id.Kind != KIdentifier {
			return
		}

		if id.Lex == "@import" {
			ConstImport(file, namespace, data)
		}
	}
}

func PDettach(file *CarlaFile, data Token) {
	id := AssertExpect(file, nil, KIdentifier, KString, KInteger, KBool, KNil, KFloat)
	if id.Kind != KIdentifier {
		return
	}

	if id.Lex == "@import" {
		parenError := "@import is an builtin statement who needs parenthesis to be used, as a function"
		AssertExpect(file, errors.New(parenError), KLeftParen)
		str := AssertExpect(file, errors.New(parenError), KString)
		AssertExpect(file, errors.New(parenError), KRightParen)
		semi := AssertExpect(file, errors.New(semicolonError), KSemi)

		DettachImport(file, str.Lex, data, semi)
	}
}

func Runner(file *CarlaFile) {
	for {
		data := file.Next()
		if data.Kind == KEOF {
			break
		}

		switch data.Kind {
		case KConst:
			PConst(file, data)
		case KDettach:
			PDettach(file, data)
		}

		if len(file.Slice) > 0 {
			break
		}
	}
}

func Parser(info *Info) {
	Ginfo = info

	file := NewCarlaFile(info.Main)

	for {
		Runner(&file)

		if len(file.Slice) == 0 {
			break
		}

		file.Content = ReplaceAll(file.Content, file.Slice)
		file.Slice = nil

		file.Position = 0
		file.Line = 0
		file.Length = uint64(len(file.Content))
	}

	fmt.Println(string(file.Content))
}
