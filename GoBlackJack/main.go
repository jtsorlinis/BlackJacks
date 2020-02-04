package main

import (
	"fmt"
	"time"
)

const NumPlayers int32 = 5
const NumDecks int32 = 8
const BetSize int32 = 10
const MinCards int32 = 40

const Rounds int32 = 1000
const Verbose bool = true

func main() {
	start := time.Now()
	t := NewTable(NumPlayers, NumDecks, BetSize, MinCards, Verbose)
	t.Print()
	fmt.Printf("%.3f seconds\n", time.Since(start).Seconds())
}
