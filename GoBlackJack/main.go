package main

import (
	"fmt"
	"time"
)

const numPlayers int32 = 5
const numDecks int32 = 8
const betSize int32 = 10
const minCards int32 = 40

const rounds int32 = 1000000
const verbose bool = false

func main() {

	table1 := NewTable(numPlayers, numDecks, betSize, minCards, verbose)
	table1.MCardPile.Shuffle()

	start := time.Now()

	var x int32 = 0
	for ; x < rounds; x++ {
		if verbose {
			println("Round " + fmt.Sprint(x))
		}
		if !verbose && rounds > 1000 && x%(rounds/100) == 0 {
			print("\tProgress: " + fmt.Sprint(int32((float32(x)/float32(rounds))*100)) + "%\r")
		}

		table1.StartRound()
		table1.CheckEarnings()
	}
	table1.clear()

	for _, player := range table1.MPlayers {
		println("Player " + fmt.Sprint(player.MPlayerNum) + " earnings: " + fmt.Sprint(player.MEarnings) + "\t\tWin Percentage: " + fmt.Sprint((50 + (player.MEarnings / float32((rounds * betSize)) * 50))) + "%")
	}
	println("Casino earnings: " + fmt.Sprintf("%.0f", table1.MCasinoEarnings))
	fmt.Printf("Played %d rounds in %.3f seconds\n", rounds, time.Since(start).Seconds())
}
