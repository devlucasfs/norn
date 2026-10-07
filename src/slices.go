package main

func PositionOffset(content []byte, pos Token) int {
	if pos.Line == 0 {
		return int(pos.Col)
	}

	line := uint64(0)
	offset := 0

	for offset < len(content) {
		if line == pos.Line {
			return offset + int(pos.Col)
		}

		if content[offset] == '\n' {
			line++
		}

		offset++
	}

	if line == pos.Line {
		return offset + int(pos.Col)
	}

	return len(content)
}

func ReplaceSlice(content []byte, replace ReplaceTokens) []byte {
	start := PositionOffset(content, replace.Start)
	end := PositionOffset(content, replace.End)

	if start < 0 {
		start = 0
	}

	if end < start {
		end = start
	}

	if start > len(content) {
		start = len(content)
	}

	if end > len(content) {
		end = len(content)
	}

	result := make([]byte, 0, len(content)-(end-start)+len(replace.Code))

	result = append(result, content[:start]...)
	result = append(result, replace.Code...)
	result = append(result, content[end:]...)

	return result
}

func ReplaceAll(content []byte, replacements []ReplaceTokens) []byte {
	for i := len(replacements) - 1; i >= 0; i-- {
		content = ReplaceSlice(content, replacements[i])
	}

	return content
}
