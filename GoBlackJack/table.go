package main

import (
	"fmt"
	"math"
	"os"
)

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

func (t *Table) dealRound() {
	for _, player := range t.MPlayers {
		t.deal()
		player.Evaluate()
		t.mCurrentPlayer++
	}
	t.mCurrentPlayer = 0
}

func (t *Table) deal() {
	var card *Card
	card, t.MCardPile.MCards = t.MCardPile.MCards[len(t.MCardPile.MCards)-1], t.MCardPile.MCards[:len(t.MCardPile.MCards)-1]
	t.MPlayers[t.mCurrentPlayer].MHand = append(t.MPlayers[t.mCurrentPlayer].MHand, card)
	t.MRunningCount += card.mCount
}

func (t *Table) preDeal() {
	for _, player := range t.MPlayers {
		t.selectBet(player)
	}
}

func (t *Table) selectBet(player *Player) {
	if t.MTrueCount >= 2 {
		//TODO type conversions can be optimized
		player.MInitialBet = int32(float64(t.MBetSize) * math.Floor(float64(t.MTrueCount)) * 1.25)
	}
}

func (t *Table) dealDealer(facedown bool) {
	var card *Card
	card, t.MCardPile.MCards = t.MCardPile.MCards[len(t.MCardPile.MCards)-1], t.MCardPile.MCards[:len(t.MCardPile.MCards)-1]
	card.mFaceDown = facedown
	t.MDealer.MHand = append(t.MDealer.MHand, card)
	if !facedown {
		t.MRunningCount += card.mCount
	}
}

// StartRound plays a round
func (t *Table) StartRound() {
	t.clear()
	t.updateCount()
	if t.MVerbose {
		println(fmt.Sprint(len(t.MCardPile.MCards)) + " cards left")
		println("Running count is: " + fmt.Sprint(t.MRunningCount) + "\tTrue count is: " + fmt.Sprint(int32(t.MTrueCount)))
	}
	t.getNewCards()
	t.preDeal()
	t.dealRound()
	t.dealDealer(false)
	t.dealRound()
	t.dealDealer(true)
	t.mCurrentPlayer = 0
	if t.checkDealerNatural() {
		t.finishRound()
	} else {
		t.checkPlayerNatural()
		if t.MVerbose {
			t.print()
		}
		t.autoPlay()
	}
}

func (t *Table) getNewCards() {
	if int32(len(t.MCardPile.MCards)) < t.MMinCards {
		t.MCardPile.Refresh()
		t.MCardPile.Shuffle()
		t.MTrueCount = 0
		t.MRunningCount = 0
		if t.MVerbose {
			println("Got " + fmt.Sprint(t.MNumOfDecks) + " new decks as number of cards left is below " + fmt.Sprint(t.MMinCards))
		}
	}
}

func (t *Table) clear() {
	clearedList := t.MPlayers[:0]
	for _, player := range t.MPlayers {
		if player.MSplitFrom == nil {
			player.ResetHand()
			clearedList = append(clearedList, player)
		}
	}
	t.MDealer.ResetHand()
	t.MPlayers = clearedList
}

func (t *Table) updateCount() {
	t.MTrueCount = float32(t.MRunningCount) / (float32(len(t.MCardPile.MCards)) / 52)
}

func (t *Table) hit() {
	t.deal()
	t.MPlayers[t.mCurrentPlayer].Evaluate()
	if t.MVerbose {
		println("Player " + fmt.Sprint(t.MPlayers[t.mCurrentPlayer].MPlayerNum) + " hits")
		t.print()
	}
}

func (t *Table) stand() {
	if t.MVerbose && t.MPlayers[t.mCurrentPlayer].MValue <= 21 {
		println("Player " + fmt.Sprint(t.MPlayers[t.mCurrentPlayer].MPlayerNum) + " stands")
		t.print()
	}
	t.MPlayers[t.mCurrentPlayer].MIsDone = true
}

func (t *Table) split() {
	//TODO
}

func (t *Table) splitAces() {
	//TODO
}

func (t *Table) doubleBet() {
	if t.MPlayers[t.mCurrentPlayer].MBetMult == 1 && len(t.MPlayers[t.mCurrentPlayer].MHand) == 2 {
		t.MPlayers[t.mCurrentPlayer].DoubleBet()
		if t.MVerbose {
			println("Player " + fmt.Sprint(t.MPlayers[t.mCurrentPlayer].MPlayerNum) + " doubles")
		}
		t.hit()
		t.stand()
	} else {
		t.hit()
	}
}

func (t *Table) autoPlay() {
	// TODO implement proper strategy
	for t.MPlayers[t.mCurrentPlayer].MValue < 17 {
		t.hit()
	}
	t.nextPlayer()
}

func (t *Table) action(action string) {
	if action == "H" {
		t.hit()
	} else if action == "S" {
		t.stand()
	} else if action == "D" {
		t.doubleBet()
	} else if action == "P" {
		t.split()
	} else {
		println("No action found")
		os.Exit(1)
	}
}

func (t *Table) dealerPlay() {
	allBusted := true
	for _, player := range t.MPlayers {
		if player.MValue < 22 {
			allBusted = false
		}
	}
	t.MDealer.MHand[1].mFaceDown = false
	t.MRunningCount += t.MDealer.MHand[1].mCount
	t.MDealer.Evaluate()
	if t.MVerbose {
		println("Dealer's turn")
	}
	if allBusted {
		if t.MVerbose {
			println("Dealer automatically wins cause all players busted")
			t.print()
		}
		t.finishRound()
	} else {
		for t.MDealer.MValue < 17 && len(t.MDealer.MHand) < 5 {
			t.dealDealer(false)
			t.MDealer.Evaluate()
			if t.MVerbose {
				println("Dealer hits")
				t.print()
			}
		}
		t.finishRound()
	}
}

func (t *Table) nextPlayer() {
	if t.mCurrentPlayer < int32(len(t.MPlayers)-1) {
		t.mCurrentPlayer++
		t.autoPlay()
	} else {
		t.dealerPlay()
	}
}

func (t *Table) checkPlayerNatural() {
	for _, player := range t.MPlayers {
		if player.MValue == 21 && len(player.MHand) == 2 && player.MSplitFrom == nil {
			player.MHasNatural = true
		}
	}
}

func (t *Table) checkDealerNatural() bool {
	t.MDealer.Evaluate()
	if t.MDealer.MValue == 21 {
		t.MDealer.MHand[1].mFaceDown = false
		t.MRunningCount += t.MDealer.MHand[1].mCount
		if t.MVerbose {
			t.print()
			println("Dealer has a natural 21")
		}
		return true
	}
	return false
}

// CheckEarnings check that players earnings match the casinos losses or vice versa
func (t *Table) CheckEarnings() {
	var check float32 = 0
	for _, player := range t.MPlayers {
		check += player.MEarnings
	}
	if check*-1 != t.MCasinoEarnings {
		println("Earnings don't match")
		os.Exit(1)
	}
}

func (t *Table) finishRound() {
	if t.MVerbose {
		println("Scoring round")
	}
	for _, player := range t.MPlayers {
		if player.MHasNatural {
			player.Win(1.5)
			if t.MVerbose {
				println("Player " + player.MPlayerNum + " Wins " + fmt.Sprint(1.5*player.MBetMult*float32(player.MInitialBet)) + " with a natural 21")
			}
		} else if player.MValue > 21 {
			player.Lose()
			if t.MVerbose {
				println("Player " + player.MPlayerNum + " Busts and Loses " + fmt.Sprint(player.MBetMult*float32(player.MInitialBet)))
			}

		} else if t.MDealer.MValue > 21 {
			player.Win(1)
			if t.MVerbose {
				println("Player " + player.MPlayerNum + " Wins " + fmt.Sprint(player.MBetMult*float32(player.MInitialBet)))
			}
		} else if player.MValue > t.MDealer.MValue {
			player.Win(1)
			if t.MVerbose {
				println("Player " + player.MPlayerNum + " Wins " + fmt.Sprint(player.MBetMult*float32(player.MInitialBet)))
			}
		} else if player.MValue == t.MDealer.MValue {
			if t.MVerbose {
				println("Player " + player.MPlayerNum + " Draws")
			}
		} else {
			player.Lose()
			if t.MVerbose {
				println("Player " + player.MPlayerNum + " Loses " + fmt.Sprint(player.MBetMult*float32(player.MInitialBet)))
			}
		}
	}
	if t.MVerbose {
		for _, player := range t.MPlayers {
			if player.MSplitFrom == nil {
				println("Player " + player.MPlayerNum + " Earnings: " + fmt.Sprint(player.MEarnings))
			}
		}
		println()
	}
}

func (t *Table) print() {
	for _, player := range t.MPlayers {
		println(player.Print())
	}
	println(t.MDealer.Print())
	println()
}
