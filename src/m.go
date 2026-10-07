package main

func main() {
	info, err := GetInfo()
	if err != nil {
		DefaultOutputs.Fatal(err)
	}

	LexerInit()
	Parser(info)
}
