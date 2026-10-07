package main

import (
	"strconv"
	"unicode"
	"unicode/utf8"
)

const (
	KUnknown uint64 = iota
	KLeftParen
	KRightParen
	KSemi
	KComma
	KDettach
	KSign
	KGenericSign
	KString
	KInteger
	KFloat
	KNil
	KBool
	KConst
	KIdentifier
	KEOF
)

const (
	KNone uint64 = iota
	KOperator
	KWords
	KNumeric
)

type Token struct {
	Kind uint64
	Line uint64
	Col  uint64
	Lex  string
}

type TokenLex struct {
	Data []byte
	Kind uint64
}

func newTokenLex(data string, kind uint64) TokenLex {
	return TokenLex{
		Data: []byte(data),
		Kind: kind,
	}
}

var Tokens []TokenLex

func LexerInit() {
	Tokens = []TokenLex{
		newTokenLex("(", KLeftParen),
		newTokenLex(")", KRightParen),
		newTokenLex(";", KSemi),
		newTokenLex(",", KComma),
		newTokenLex("..", KDettach),
		newTokenLex("=", KSign),
		newTokenLex(":=", KGenericSign),
		newTokenLex("const", KConst),
		newTokenLex("false", KBool),
		newTokenLex("true", KBool),
		newTokenLex("nil", KNil),
	}
}

func getCol(file *CarlaFile, start uint64) uint64 {
	col := uint64(0)

	for i := start; i > 0; i-- {
		if file.Content[i-1] == '\n' {
			break
		}

		col++
	}

	return col
}

func tokenFromBuffer(file *CarlaFile, buff string, start uint64) Token {
	if len(buff) == 0 {
		return Token{}
	}

	for _, token := range Tokens {
		if string(token.Data) == buff {
			return Token{
				Kind: token.Kind,
				Line: file.Line,
				Col:  getCol(file, start),
				Lex:  buff,
			}
		}
	}

	kind := KIdentifier

	if _, err := strconv.ParseInt(buff, 10, 64); err == nil {
		kind = KInteger
	} else if _, err := strconv.ParseFloat(buff, 64); err == nil {
		kind = KFloat
	}

	return Token{
		Kind: kind,
		Line: file.Line,
		Col:  getCol(file, start),
		Lex:  buff,
	}
}

func isOperatorChar(char rune) bool {
	switch char {
	case '=', '+', '-', '*', '/', '%',
		'<', '>', '!', '&', '|', '^',
		'~', '.', ':':
		return true
	}

	return false
}

func isWordChar(char rune) bool {
	return unicode.IsLetter(char) ||
		unicode.IsDigit(char) ||
		char == '_' ||
		char == '@'
}

func (file *CarlaFile) Back(token Token) {
	file.back = append(file.back, token)
}

func (file *CarlaFile) Next() Token {
	if len(file.back) > 0 {
		result := file.back[len(file.back)-1]
		file.back = file.back[:len(file.back)-1]
		return result
	}

	for file.Position < file.Length {
		char, size := utf8.DecodeRune(file.Content[file.Position:])

		if char == utf8.RuneError && size == 1 {
			size = 1
		}

		if char == ' ' || char == '\t' || char == '\r' {
			file.Position += uint64(size)
			continue
		}

		if char == '\n' {
			file.Position += uint64(size)
			file.Line++
			continue
		}

		start := file.Position

		// Identificadores e palavras
		if unicode.IsLetter(char) || char == '_' || char == '@' {
			file.Position += uint64(size)

			for file.Position < file.Length {
				char, size = utf8.DecodeRune(file.Content[file.Position:])

				if !isWordChar(char) {
					break
				}

				file.Position += uint64(size)
			}

			return tokenFromBuffer(
				file,
				string(file.Content[start:file.Position]),
				start,
			)
		}

		// Números
		if unicode.IsDigit(char) {
			hasDot := false

			file.Position += uint64(size)

			for file.Position < file.Length {
				char, size = utf8.DecodeRune(file.Content[file.Position:])

				if unicode.IsDigit(char) {
					file.Position += uint64(size)
					continue
				}

				if char == '.' {
					if hasDot {
						break
					}

					hasDot = true
					file.Position += uint64(size)
					continue
				}

				break
			}

			return tokenFromBuffer(
				file,
				string(file.Content[start:file.Position]),
				start,
			)
		}

		// Strings
		if char == '"' {
			file.Position += uint64(size)

			for file.Position < file.Length {
				char, size = utf8.DecodeRune(file.Content[file.Position:])

				if char == '\\' {
					file.Position += uint64(size)

					if file.Position < file.Length {
						_, escapedSize := utf8.DecodeRune(
							file.Content[file.Position:],
						)

						file.Position += uint64(escapedSize)
					}

					continue
				}

				if char == '"' {
					file.Position += uint64(size)
					break
				}

				file.Position += uint64(size)
			}

			return Token{
				Kind: KString,
				Line: file.Line,
				Col:  getCol(file, start),
				Lex:  string(file.Content[start:file.Position]),
			}
		}

		// Delimitadores
		if char == '(' {
			file.Position += uint64(size)

			return Token{
				Kind: KLeftParen,
				Line: file.Line,
				Col:  getCol(file, start),
				Lex:  "(",
			}
		}

		if char == ')' {
			file.Position += uint64(size)

			return Token{
				Kind: KRightParen,
				Line: file.Line,
				Col:  getCol(file, start),
				Lex:  ")",
			}
		}

		if char == ';' {
			file.Position += uint64(size)

			return Token{
				Kind: KSemi,
				Line: file.Line,
				Col:  getCol(file, start),
				Lex:  ";",
			}
		}

		if char == ',' {
			file.Position += uint64(size)

			return Token{
				Kind: KComma,
				Line: file.Line,
				Col:  getCol(file, start),
				Lex:  ",",
			}
		}

		// Operadores
		if isOperatorChar(char) {
			file.Position += uint64(size)

			if char == '.' {
				if file.Position < file.Length {
					next, nextSize := utf8.DecodeRune(file.Content[file.Position:])

					if next == '.' {
						file.Position += uint64(nextSize)

						return Token{
							Kind: KDettach,
							Line: file.Line,
							Col:  getCol(file, start),
							Lex:  "..",
						}
					}
				}

				return Token{
					Kind: KUnknown,
					Line: file.Line,
					Col:  getCol(file, start),
					Lex:  ".",
				}
			}

			for file.Position < file.Length {
				char, size = utf8.DecodeRune(file.Content[file.Position:])

				if !isOperatorChar(char) {
					break
				}

				file.Position += uint64(size)
			}

			return tokenFromBuffer(
				file,
				string(file.Content[start:file.Position]),
				start,
			)
		}

		// Desconhecido
		file.Position += uint64(size)

		return Token{
			Kind: KUnknown,
			Line: file.Line,
			Col:  getCol(file, start),
			Lex:  string(file.Content[start:file.Position]),
		}
	}

	return Token{
		Kind: KEOF,
		Line: file.Line,
		Col:  0,
		Lex:  "",
	}
}
