package main

import (
	"fmt"
	"os"
	"path/filepath"
	"regexp"
	"slices"
	"strings"
)

var alreadyDidImport []string

func ValidatePath(filename string) string {
	/* If the file path starts with "./" we
	 * need to check import the file from the
	 * same parent directory as the main file. */
	if len(filename) >= 2 && filename[:2] == "./" {
		/* Get the parent directory of the main file
		 * and then concat it with the OS path separator
		 * and the import relative path */
		directory := filepath.Dir(Ginfo.Main)
		relative := "./" + filepath.Join(directory, filename)

		/* Transform the relative path in a absolute path
		 * and return a error if fail */
		path, err := filepath.Abs(relative)
		if err != nil {
			DefaultOutputs.Fatal(
				fmt.Errorf("%s isn't a valid file path", filename),
			)
		}

		/* Check if the file exist in the parent directory
		 * of the main file */
		if _, err := os.ReadFile(path); err != nil {
			DefaultOutputs.Fatal(
				fmt.Errorf("%s isn't a existent file : %s", relative, path),
			)
		}

		/* Return the absolute path of the file. */
		return path
	}

	/* If you did import the file as a library,
	 * it's needed to check the path of Carla libraries
	 * and then make check if the library really exists */
	home := os.Getenv("HOME")
	path := filepath.Join(home, ".carla", "libs", filename)

	/* If doesn't exist, return a error */
	if _, err := os.ReadFile(path); err != nil {
		DefaultOutputs.Fatal(
			fmt.Errorf("%s isn't a existent library", path),
		)
	}

	/* Return the aboslute path if the library */
	return path
}

func GetFileContent(path string, bypass bool) string {
	/* Check if the file was already imported as
	 * pragma once. If yes, do not import any code
	 * Ignore this normal rule if isn't a dettach */
	if slices.Contains(alreadyDidImport, path) && !bypass {
		return ""
	}

	/* Read the file who did u import and then
	 * cast it from []bytes to string */
	content, _ := os.ReadFile(path)
	asciz := string(content)

	/* Split all the file in lines, using linefeed
	 * as the split character */
	lines := strings.Split(asciz, "\n")

	/* Compile the Regex to "@pragma always" in
	 * Perl Syntax to check check if file is pragma
	 * always. */
	regex := regexp.MustCompile(`^@pragma(\s+)always$`)

	/* If the first line if the file match with
	 * the Regex, we need to remove the first line */
	if regex.MatchString(lines[0]) {
		return strings.Join(lines[1:], "\n")
	}

	/* Add file in the "already did import" list
	 * 'cuz file isnt pragma always.
	 * It is pragma once. But only if isn't a dettach */
	if !bypass {
		alreadyDidImport = append(alreadyDidImport, path)
	}

	/* Return the file content */
	return asciz
}

func ConstImportNamespace(file *CarlaFile, namespace, str string, _const, semi Token) {
	/* Get the absolute path of the imported
	 * library or file. */
	path := ValidatePath(str[1 : len(str)-1])

	/* Add in file content the file position
	 * stack push and pop */
	line := "@pushfile \"" + path + "\"\n"
	line += "const " + namespace + " = namespace {\n"
	line += GetFileContent(path, true)
	line += "}\n"
	line += "@popfile"

	semi.Col += 1
	file.Slice = append(file.Slice, ReplaceTokens{
		Start: _const,
		End:   semi,
		Code:  line,
	})
}

func DettachImport(file *CarlaFile, str string, dettach, semi Token) {
	/* Get the absolute path of the imported
	 * library or file. */
	path := ValidatePath(str[1 : len(str)-1])

	/* Add in file content the file position
	 * stack push and pop */
	content := GetFileContent(path, false)
	line := "@pushfile \"" + path + "\"\n"
	line += content
	line += "@popfile"

	/* An dettach can't be bypassed! */
	if len(content) == 0 {
		line = ""
	}

	semi.Col += 1
	file.Slice = append(file.Slice, ReplaceTokens{
		Start: dettach,
		End:   semi,
		Code:  line,
	})
}
