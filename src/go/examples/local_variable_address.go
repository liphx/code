package main

import "fmt"

// function returns address of local variable
func get_local() *int {
	n := 42
	return &n
}

func main() {
	fmt.Println(*get_local())
}
