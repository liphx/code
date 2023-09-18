package main

import "fmt"
import "os"

func main() {
	i := 1
	for i < len(os.Args) {
		if i != 1 {
			fmt.Print(" ")
		}
		fmt.Print(os.Args[i])
		i += 1
	}
	fmt.Println()
}
