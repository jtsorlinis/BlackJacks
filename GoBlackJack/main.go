package main

import (
	"fmt"
	"time"
)

func main() {
	start := time.Now()
	cp := NewCardPile(5)
	println(cp.Print())
	for i := 0; i < 1000000; i++ {
		cp.Shuffle()
	}
	println(cp.Print())
	fmt.Printf("%.3f seconds\n", time.Since(start).Seconds())
}
