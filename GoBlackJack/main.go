package main

import (
	"goblackjack/cardpile"
	"time"
)

func main() {
	start := time.Now()
	cp := cardpile.New(8)
	println(cp.Print())
	for i := 0; i < 1000000; i++ {
		cp.Shuffle()
	}
	println(time.Since(start).Milliseconds())
}
