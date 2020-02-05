package main

import (
	"fmt"
	"time"
)

const numPlayers int32 = 5
const numDecks int32 = 8
const betSize int32 = 10
const minCards int32 = 40

const rounds int32 = 1000
const verbose bool = true

func main() {

	t := NewTable(numPlayers, numDecks, betSize, minCards, verbose)

	start := time.Now()
	t.StartRound()

	fmt.Printf("%.3f seconds\n", time.Since(start).Seconds())
}
