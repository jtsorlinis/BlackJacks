package main

import (
	"fmt"
	"os"
	"strconv"
	"time"
)

const numPlayers int32 = 5
const numDecks int32 = 8
const betSize int32 = 10
const minCards int32 = 40

var rounds = 1000000

const verbose bool = false

func main() {
	// f, _ := os.Create("test.prof")
	// pprof.StartCPUProfile(f)
	// defer pprof.StopCPUProfile()

	if len(os.Args) == 2 {
		rounds, _ = strconv.Atoi(os.Args[1])
	}

	table1 := NewTable(numPlayers, numDecks, betSize, minCards, verbose)
	table1.MCardPile.Shuffle()

	start := time.Now()

	rounds32 := int32(rounds)
	var x int32
	for ; x < rounds32; x++ {
		if verbose {
			println("Round " + fmt.Sprint(x+1))
		}
		if !verbose && rounds > 1000 && x%(rounds32/100) == 0 {
			print("\tProgress: " + fmt.Sprint(x*100/rounds32) + "%\r")
		}

		table1.StartRound()
		table1.CheckEarnings()
	}
	table1.clear()

	for _, player := range table1.MPlayers {
		println("Player " + fmt.Sprint(player.MPlayerNum) + " earnings: " + fmt.Sprint(player.MEarnings) + "\t\tWin Percentage: " + fmt.Sprint((50 + (player.MEarnings / float32((rounds32 * betSize)) * 50))) + "%")
	}
	println("Casino earnings: " + fmt.Sprintf("%.0f", table1.MCasinoEarnings))
	fmt.Printf("Played %d rounds in %.3f seconds\n", rounds, time.Since(start).Seconds())
}
