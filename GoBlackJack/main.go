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
const minCards int32 = 52

var rounds = 1000000

const verbose bool = false

func main() {
	// f, _ := os.Create("test.prof")
	// pprof.StartCPUProfile(f)
	// defer pprof.StopCPUProfile()

	if len(os.Args) == 2 {
		rounds, _ = strconv.Atoi(os.Args[1])
	}

	table1 := NewTable(numPlayers, numDecks, betSize, minCards)
	table1.MCardPile.Shuffle()

	start := time.Now()

	rounds32 := int32(rounds)
	for x := int32(1); x <= rounds32; x++ {
		if verbose {
			fmt.Println("Round " + fmt.Sprint(x))
		}
		if !verbose && rounds > 1000 && x%(rounds32/100) == 0 {
			print("\tProgress: " + fmt.Sprint(x*100/rounds32) + "%\r")
		}

		table1.StartRound()
		table1.CheckEarnings()
	}
	table1.clear()

	for _, player := range table1.MPlayers {
		fmt.Printf("Player %s earnings: %d\t\tWin Percentage: %.2f%%\n", player.MPlayerNum, int32(player.MEarnings), 50+player.MEarnings/(float64(rounds)*float64(betSize))*50)
	}
	fmt.Printf("Casino earnings: %d\n", int32(table1.MCasinoEarnings))
	fmt.Printf("Played %d rounds in %.3f seconds\n", rounds, time.Since(start).Seconds())
}
