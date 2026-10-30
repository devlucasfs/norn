package main

import (
	"errors"
	"fmt"
	"strconv"
	"strings"
)

type EnumValue struct {
	Value      int64
	Expression string
	IsExpr     bool
}

var Enums = map[string]map[string]EnumValue{}

func PUsingEnum(file *CarlaFile, data Token, id Token) {
	enumError := errors.New("An enum declaration needs parentheses.")

	AssertExpect(file, enumError, KLeftParen)

	values := make(map[string]EnumValue)
	var current int64

	for {
		token := AssertExpect(file, errors.New("Expected an identifier inside enum."), KIdentifier, KRightParen)

		if token.Kind == KRightParen {
			break
		}

		name := token.Lex
		value := EnumValue{Value: current}

		next := file.Next()

		if next.Kind == KSign && next.Lex == "=" {
			next = file.Next()

			if next.Lex == "expr" {
				AssertExpect(file, enumError, KLeftParen)

				var expression strings.Builder
				depth := 1

				for depth > 0 {
					part := file.Next()

					if part.Kind == KEOF {
						DefaultOutputs.Fatal("Unexpected EOF inside enum expression.")
					}

					if part.Kind == KLeftParen {
						depth++
					} else if part.Kind == KRightParen {
						depth--

						if depth == 0 {
							break
						}
					}

					if expression.Len() > 0 {
						expression.WriteByte(' ')
					}

					expression.WriteString(part.Lex)
				}

				value.Expression = expression.String()
				value.IsExpr = true

				next = file.Next()
			} else {
				if next.Kind != KInteger {
					DefaultOutputs.Fatal("Expected an integer or expr() as enum value.")
				}

				parsed, err := strconv.ParseInt(next.Lex, 0, 64)
				if err != nil {
					DefaultOutputs.Fatal(fmt.Sprintf("Invalid enum value: %s", next.Lex))
				}

				value.Value = parsed
				next = file.Next()
			}
		}

		if _, exists := values[name]; exists {
			DefaultOutputs.Fatal(fmt.Sprintf("Duplicate enum identifier: %s", name))
		}

		values[name] = value

		if value.IsExpr {
			current++
		} else {
			current = value.Value + 1
		}

		if next.Kind == KRightParen {
			break
		}

		if next.Kind != KComma {
			DefaultOutputs.Fatal("Expected ',' or ')' inside enum.")
		}
	}

	if _, exists := Enums[id.Lex]; exists {
		DefaultOutputs.Fatal(fmt.Sprintf("Enum '%s' already exists.", id.Lex))
	}

	Enums[id.Lex] = values

	semi := AssertExpect(file, errors.New(semicolonError), KSemi)
	semi.Col += 1

	file.Slice = append(file.Slice, ReplaceTokens{
		Start: data,
		End:   semi,
		Code:  "",
	})
}

func PEnumValue(file *CarlaFile, data Token) {
	values, exists := Enums[data.Lex]
	if !exists {
		return
	}

	dot := file.Next()

	if dot.Lex == "." {
		file.Back(dot)
		return
	}

	member := AssertExpect(file, nil, KIdentifier)

	value, exists := values[member.Lex]
	if !exists {
		return
	}

	member.Col += uint64(len(member.Lex))

	replacement := fmt.Sprintf("%d", value.Value)

	if value.IsExpr {
		replacement = value.Expression
	}

	file.Slice = append(file.Slice, ReplaceTokens{
		Start: data,
		End:   member,
		Code:  replacement,
	})
}
