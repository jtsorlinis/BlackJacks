package main

import (
	"fmt"
	"os"
)

//Table class
type Table struct {
	MVerbose        bool
	MBetSize        int32
	MPlayers        []*Player
	MNumOfDecks     int32
	MCardPile       CardPile
	MMinCards       int32
	MDealer         *Dealer
	mCurrentPlayer  int32
	MCasinoEarnings float32
	MRunningCount   int32
	MTrueCount      int32
	MStratHard      []byte
	MStratSoft      []byte
	MStratSplit     []byte
}

// NewTable constructor
func NewTable(numplayers int32, numdecks int32, betsize int32, mincards int32, verbose bool) *Table {
	t := new(Table)
	t.MVerbose = verbose
	t.MBetSize = betSize
	t.MNumOfDecks = numdecks
	t.MCardPile = NewCardPile(numdecks)
	t.MMinCards = mincards
	t.MDealer = NewDealer()
	t.MStratHard = array2dToMap(stratHard)
	t.MStratSoft = array2dToMap(stratSoft)
	t.MStratSplit = array2dToMap(stratSplit)
	t.MPlayers = make([]*Player, 0, numplayers*3)
	for i := int32(0); i < numplayers; i++ {
		t.MPlayers = append(t.MPlayers, NewPlayer(t, nil))
	}

	return t
}

func (t *Table) dealRound() {
	for range t.MPlayers {
		t.deal()
		t.mCurrentPlayer++
	}
	t.mCurrentPlayer = 0
}

func (t *Table) evaluateAll() {
	for _, player := range t.MPlayers {
		player.Evaluate()
	}
}

func (t *Table) deal() {
	t.MPlayers[t.mCurrentPlayer].MHand = append(t.MPlayers[t.mCurrentPlayer].MHand, t.MCardPile.MCards[len(t.MCardPile.MCards)-1])
	t.MRunningCount += t.MCardPile.MCards[len(t.MCardPile.MCards)-1].mCount
	t.MCardPile.MCards = t.MCardPile.MCards[:len(t.MCardPile.MCards)-1]
}

func (t *Table) preDeal() {
	for _, player := range t.MPlayers {
		t.selectBet(player)
	}
}

func (t *Table) selectBet(player *Player) {
	if t.MTrueCount >= 2 {
		player.MInitialBet = t.MBetSize * (t.MTrueCount - 1)
	}
}

func (t *Table) dealDealer(facedown bool) {
	t.MCardPile.MCards[len(t.MCardPile.MCards)-1].mFaceDown = facedown
	t.MDealer.MHand = append(t.MDealer.MHand, t.MCardPile.MCards[len(t.MCardPile.MCards)-1])
	if !facedown {
		t.MRunningCount += t.MCardPile.MCards[len(t.MCardPile.MCards)-1].mCount
	}
	t.MCardPile.MCards = t.MCardPile.MCards[:len(t.MCardPile.MCards)-1]
}

// StartRound plays a round
func (t *Table) StartRound() {
	t.clear()
	t.updateCount()
	if t.MVerbose {
		println(fmt.Sprint(len(t.MCardPile.MCards)) + " cards left")
		println("Running count is: " + fmt.Sprint(t.MRunningCount) + "\tTrue count is: " + fmt.Sprint(t.MTrueCount))
	}
	t.getNewCards()
	t.preDeal()
	t.dealRound()
	t.dealDealer(false)
	t.dealRound()
	t.dealDealer(true)
	t.evaluateAll()
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
	for i := len(t.MPlayers) - 1; i >= 0; i-- {
		if t.MPlayers[i].MSplitFrom != nil {
			t.MPlayers[i-1].MEarnings += t.MPlayers[i].MEarnings
			t.MPlayers = append(t.MPlayers[:i], t.MPlayers[i+1:]...)
		} else {
			t.MPlayers[i].ResetHand()
		}
	}
	t.MDealer.ResetHand()
	t.mCurrentPlayer = 0
}

func (t *Table) updateCount() {
	if len(t.MCardPile.MCards) > 51 {
		t.MTrueCount = t.MRunningCount / (int32(len(t.MCardPile.MCards)) / 52)
	}
}

func (t *Table) hit() {
	t.deal()
	t.MPlayers[t.mCurrentPlayer].Evaluate()
	if t.MVerbose {
		println("Player " + fmt.Sprint(t.MPlayers[t.mCurrentPlayer].MPlayerNum) + " hits")
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
	splitPlayer := NewPlayer(t, t.MPlayers[t.mCurrentPlayer])
	_, t.MPlayers[t.mCurrentPlayer].MHand = t.MPlayers[t.mCurrentPlayer].MHand[len(t.MPlayers[t.mCurrentPlayer].MHand)-1], t.MPlayers[t.mCurrentPlayer].MHand[:len(t.MPlayers[t.mCurrentPlayer].MHand)-1]
	t.MPlayers = append(t.MPlayers, nil)
	copy(t.MPlayers[t.mCurrentPlayer+2:], t.MPlayers[t.mCurrentPlayer+1:])
	t.MPlayers[t.mCurrentPlayer+1] = splitPlayer
	t.MPlayers[t.mCurrentPlayer].Evaluate()
	t.MPlayers[t.mCurrentPlayer+1].Evaluate()
	if t.MVerbose {
		println("Player " + fmt.Sprint(t.MPlayers[t.mCurrentPlayer].MPlayerNum) + " splits")
	}
}

func (t *Table) splitAces() {
	if t.MVerbose {
		println("Player " + fmt.Sprint(t.MPlayers[t.mCurrentPlayer].MPlayerNum) + " splits Aces")
	}
	splitPlayer := NewPlayer(t, t.MPlayers[t.mCurrentPlayer])
	_, t.MPlayers[t.mCurrentPlayer].MHand = t.MPlayers[t.mCurrentPlayer].MHand[len(t.MPlayers[t.mCurrentPlayer].MHand)-1], t.MPlayers[t.mCurrentPlayer].MHand[:len(t.MPlayers[t.mCurrentPlayer].MHand)-1]
	t.MPlayers = append(t.MPlayers, nil)
	copy(t.MPlayers[t.mCurrentPlayer+2:], t.MPlayers[t.mCurrentPlayer+1:])
	t.MPlayers[t.mCurrentPlayer+1] = splitPlayer
	t.deal()
	t.MPlayers[t.mCurrentPlayer].Evaluate()
	t.stand()
	t.mCurrentPlayer++
	t.deal()
	t.MPlayers[t.mCurrentPlayer].Evaluate()
	t.stand()
	if t.MVerbose {
		t.print()
	}
}

func (t *Table) doubleBet() {
	if t.MPlayers[t.mCurrentPlayer].MBetMult < 1.1 && len(t.MPlayers[t.mCurrentPlayer].MHand) == 2 {
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
	for !t.MPlayers[t.mCurrentPlayer].MIsDone {
		// check if player just split
		if len(t.MPlayers[t.mCurrentPlayer].MHand) == 1 {
			if t.MVerbose {
				println("Player " + t.MPlayers[t.mCurrentPlayer].MPlayerNum + " gets 2nd card after splitting")
			}
			t.deal()
			t.MPlayers[t.mCurrentPlayer].Evaluate()
		}

		if len(t.MPlayers[t.mCurrentPlayer].MHand) < 5 && t.MPlayers[t.mCurrentPlayer].MValue < 21 {
			splitCardVal := t.MPlayers[t.mCurrentPlayer].CanSplit()
			if splitCardVal == 11 {
				t.splitAces()
			} else if splitCardVal != 0 && (splitCardVal != 5 && splitCardVal != 10) {
				t.action(getAction(splitCardVal, t.MDealer.UpCard(), &t.MStratSplit))
			} else if t.MPlayers[t.mCurrentPlayer].MIsSoft {
				t.action(getAction(t.MPlayers[t.mCurrentPlayer].MValue, t.MDealer.UpCard(), &t.MStratSoft))
			} else {
				t.action(getAction(t.MPlayers[t.mCurrentPlayer].MValue, t.MDealer.UpCard(), &t.MStratHard))
			}
		} else {
			t.stand()
		}
	}
	t.nextPlayer()
}

func (t *Table) action(action byte) {
	switch action {
	case 'H':
		t.hit()
	case 'S':
		t.stand()
	case 'D':
		t.doubleBet()
	case 'P':
		t.split()
	default:
		println("No action found")
		os.Exit(1)
	}
}

func (t *Table) dealerPlay() {
	allBusted := true
	for _, player := range t.MPlayers {
		if player.MValue < 22 {
			allBusted = false
			break
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
	var check float32
	for _, player := range t.MPlayers {
		check += player.MEarnings
	}
	if check+t.MCasinoEarnings != 0 {
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

		} else if t.MDealer.MValue > 21 || player.MValue > t.MDealer.MValue {
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
