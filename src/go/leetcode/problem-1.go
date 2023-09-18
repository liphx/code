package main

import "fmt"

func twoSum(nums []int, target int) []int {
	data := make(map[int]int)
	for i, x := range nums {
		if p, ok := data[target-x]; ok {
			return []int{p, i}
		}
		data[x] = i
	}
	return nil
}

func main() {
	fmt.Println(twoSum([]int{2, 7, 9, 11}, 9))
}
