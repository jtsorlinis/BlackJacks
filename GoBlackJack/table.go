package main

import "fmt"

//Table class
type Table struct {
	MVerbose        bool
	MBetSize        int32
	MPlayers        []*Player
	MNumOfDecks     int32
	MCardPile       *CardPile
	MMinCards       int32
	MDealer         *Dealer
	mCurrentPlayer  int32
	MCasinoEarnings float32
	MRunningCount   int32
	MTrueCount      float32
	MStratHard      map[string]int32
	MStratSoft      map[string]int32
	MStratSplit     map[string]int32
}

// NewTable constructor
func NewTable(numplayers int32, numdecks int32, betsize int32, mincards int32, verbose bool) *Table {
	t := Table{
		MVerbose:        verbose,
		MBetSize:        betsize,
		MNumOfDecks:     numdecks,
		MCardPile:       NewCardPile(numdecks),
		MMinCards:       mincards,
		MDealer:         NewDealer(),
		mCurrentPlayer:  0,
		MCasinoEarnings: 0,
		MRunningCount:   0,
		MTrueCount:      0,
		MStratHard:      nil,
		MStratSoft:      nil,
		MStratSplit:     nil,
	}
	for i := int32(0); i < numplayers; i++ {
		t.MPlayers = append(t.MPlayers, NewPlayer(&t, nil))
	}

	return &t
}

func (t *Table) DealRound() {
	for _, player := range t.MPlayers {
		t.Deal()
		player.Evaluate()
		t.mCurrentPlayer++
	}
	t.mCurrentPlayer = 0
}

func (t *Table) Deal() {
	var card *Card
	card, t.MCardPile.MCards = t.MCardPile.MCards[len(t.MCardPile.MCards)-1], t.MCardPile.MCards[:len(t.MCardPile.MCards)-1]
	t.MPlayers[t.mCurrentPlayer].MHand = append(t.MPlayers[t.mCurrentPlayer].MHand, card)
	t.MRunningCount += card.mCount
}

func (t *Table) Hit() {
	t.Deal()
	t.MPlayers[t.mCurrentPlayer].Evaluate()
	if t.MVerbose {
		println("Player " + fmt.Sprint(t.MPlayers[t.mCurrentPlayer].MPlayerNum) + " hits")
	}
}

func (t *Table) Print() {
	for _, player := range t.MPlayers {
		println(player.Print())
	}
	println(t.MDealer.Print())
}
