package main

import (
	"fmt"
	"os"
)

// Table class
type Table struct {
	MBetSize        int32
	MPlayers        []*Player
	MNumOfDecks     int32
	MCardPile       CardPile
	MMinCards       int32
	MDealer         *Dealer
	mCurrentPlayer  int32
	MCasinoEarnings float64
	MRunningCount   int32
	MTrueCount      int32
	MStratHard      []byte
	MStratSoft      []byte
	MStratSplit     []byte
}

// NewTable constructor
func NewTable(numplayers int32, numdecks int32, betsize int32, mincards int32) *Table {
	t := new(Table)
	t.MBetSize = betsize
	t.MNumOfDecks = numdecks
	t.MCardPile = NewCardPile(numdecks)
	t.MMinCards = mincards
	t.MDealer = NewDealer()
	t.MStratHard = array2dToMap(stratHard)
	t.MStratSoft = array2dToMap(stratSoft)
	t.MStratSplit = array2dToMap(stratSplit)
	t.MPlayers = make([]*Player, 0, numplayers*3)
	for range numplayers {
		t.MPlayers = append(t.MPlayers, NewPlayer(t, nil))
	}

	return t
}

func (t *Table) dealRound() {
	runningCount := t.MRunningCount
	for _, player := range t.MPlayers {
		card := t.MCardPile.MCards[len(t.MCardPile.MCards)-1]
		player.MHand = append(player.MHand, card)
		runningCount += card.mCount
		t.MCardPile.MCards = t.MCardPile.MCards[:len(t.MCardPile.MCards)-1]
	}
	t.MRunningCount = runningCount
	t.mCurrentPlayer = 0
}

func (t *Table) evaluateAll() {
	for _, player := range t.MPlayers {
		player.Evaluate()
	}
}

func (t *Table) deal() {
	card := t.MCardPile.MCards[len(t.MCardPile.MCards)-1]
	t.MPlayers[t.mCurrentPlayer].MHand = append(t.MPlayers[t.mCurrentPlayer].MHand, card)
	t.MRunningCount += card.mCount
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
	card := t.MCardPile.MCards[len(t.MCardPile.MCards)-1]
	card.mFaceDown = facedown
	t.MDealer.MHand = append(t.MDealer.MHand, card)
	if !facedown {
		t.MRunningCount += card.mCount
	}
	t.MCardPile.MCards = t.MCardPile.MCards[:len(t.MCardPile.MCards)-1]
}

// StartRound plays a round
func (t *Table) StartRound() {
	t.updateCount()
	if verbose {
		fmt.Println(fmt.Sprint(len(t.MCardPile.MCards)) + " cards left")
		fmt.Println("Running count is: " + fmt.Sprint(t.MRunningCount) + "\tTrue count is: " + fmt.Sprint(t.MTrueCount))
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
		if verbose {
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
		if verbose {
			fmt.Println("Got " + fmt.Sprint(t.MNumOfDecks) + " new decks as number of cards left is below " + fmt.Sprint(t.MMinCards))
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
	if verbose {
		fmt.Println("Player " + fmt.Sprint(t.MPlayers[t.mCurrentPlayer].MPlayerNum) + " hits")
	}
}

func (t *Table) stand() {
	if verbose && t.MPlayers[t.mCurrentPlayer].MValue <= 21 {
		fmt.Println("Player " + fmt.Sprint(t.MPlayers[t.mCurrentPlayer].MPlayerNum) + " stands")
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
	if verbose {
		fmt.Println("Player " + fmt.Sprint(t.MPlayers[t.mCurrentPlayer].MPlayerNum) + " splits")
	}
}

func (t *Table) splitAces() {
	if verbose {
		fmt.Println("Player " + fmt.Sprint(t.MPlayers[t.mCurrentPlayer].MPlayerNum) + " splits aces")
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
	if verbose {
		t.print()
	}
}

func (t *Table) doubleBet() {
	if t.MPlayers[t.mCurrentPlayer].MBetMult < 1.1 && len(t.MPlayers[t.mCurrentPlayer].MHand) == 2 {
		t.MPlayers[t.mCurrentPlayer].DoubleBet()
		if verbose {
			fmt.Println("Player " + fmt.Sprint(t.MPlayers[t.mCurrentPlayer].MPlayerNum) + " doubles")
		}
		t.hit()
		t.stand()
	} else if t.MPlayers[t.mCurrentPlayer].MIsSoft && t.MPlayers[t.mCurrentPlayer].MValue == 18 {
		t.stand()
	} else {
		t.hit()
	}
}

func (t *Table) autoPlay() {
	dealerUp := t.MDealer.UpCard()
	for t.mCurrentPlayer < int32(len(t.MPlayers)) {
		for !t.MPlayers[t.mCurrentPlayer].MIsDone {
			// check if player just split
			if len(t.MPlayers[t.mCurrentPlayer].MHand) == 1 {
				if verbose {
					fmt.Println("Player " + t.MPlayers[t.mCurrentPlayer].MPlayerNum + " gets 2nd card after splitting")
				}
				t.deal()
				t.MPlayers[t.mCurrentPlayer].Evaluate()
			}

			if t.MPlayers[t.mCurrentPlayer].MValue < 21 {
				splitCardVal := t.MPlayers[t.mCurrentPlayer].CanSplit()
				if splitCardVal == 11 {
					t.splitAces()
				} else if splitCardVal != 0 && (splitCardVal != 5 && splitCardVal != 10) {
					t.action(getAction(splitCardVal, dealerUp, &t.MStratSplit))
				} else if t.MPlayers[t.mCurrentPlayer].MIsSoft {
					t.action(getAction(t.MPlayers[t.mCurrentPlayer].MValue, dealerUp, &t.MStratSoft))
				} else {
					t.action(getAction(t.MPlayers[t.mCurrentPlayer].MValue, dealerUp, &t.MStratHard))
				}
			} else {
				t.stand()
			}
		}
		t.mCurrentPlayer++
	}
	t.mCurrentPlayer = 0
	t.dealerPlay()
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
	allSettled := true
	for _, player := range t.MPlayers {
		if player.MValue < 22 && !player.MHasNatural {
			allSettled = false
			break
		}
	}
	t.MDealer.MHand[1].mFaceDown = false
	t.MRunningCount += t.MDealer.MHand[1].mCount
	if verbose {
		fmt.Println("Dealer's turn")
		t.print()
	}
	if allSettled {
		if verbose {
			fmt.Println("Dealer does not draw because all player hands are settled")
		}
		t.finishRound()
	} else {
		for t.MDealer.MValue < 17 {
			t.dealDealer(false)
			t.MDealer.Evaluate()
			if verbose {
				fmt.Println("Dealer hits")
				t.print()
			}
		}
		t.finishRound()
	}
}

func (t *Table) checkPlayerNatural() {
	for _, player := range t.MPlayers {
		if player.MValue == 21 && len(player.MHand) == 2 && *player.MSplitCount == 0 {
			player.MHasNatural = true
		}
	}
}

func (t *Table) checkDealerNatural() bool {
	t.MDealer.Evaluate()
	if t.MDealer.MValue == 21 {
		t.MDealer.MHand[1].mFaceDown = false
		t.MRunningCount += t.MDealer.MHand[1].mCount
		if verbose {
			t.print()
			fmt.Println("Dealer has a natural 21")
		}
		return true
	}
	return false
}

// CheckEarnings check that players earnings match the casinos losses or vice versa
func (t *Table) CheckEarnings() {
	var check float64
	for _, player := range t.MPlayers {
		check += player.MEarnings
	}
	if check+t.MCasinoEarnings != 0 {
		println("Earnings don't match")
		os.Exit(1)
	}
}

func (t *Table) finishRound() {
	if verbose {
		fmt.Println("Scoring round")
	}
	casinoEarnings := t.MCasinoEarnings
	for _, player := range t.MPlayers {
		var mult float64
		if player.MHasNatural {
			if verbose {
				fmt.Println("Player " + player.MPlayerNum + " Wins " + fmt.Sprint(int32(1.5*player.MBetMult*float64(player.MInitialBet))) + " with a natural 21")
			}
			mult = 1.5
		} else if player.MValue > 21 {
			if verbose {
				fmt.Println("Player " + player.MPlayerNum + " Busts and Loses " + fmt.Sprint(int32(player.MBetMult*float64(player.MInitialBet))))
			}
			mult = -1
		} else if t.MDealer.MValue > 21 || player.MValue > t.MDealer.MValue {
			if verbose {
				fmt.Println("Player " + player.MPlayerNum + " Wins " + fmt.Sprint(int32(player.MBetMult*float64(player.MInitialBet))))
			}
			mult = 1
		} else if player.MValue == t.MDealer.MValue {
			if verbose {
				fmt.Println("Player " + player.MPlayerNum + " Draws")
			}
			mult = 0
		} else {
			if verbose {
				fmt.Println("Player " + player.MPlayerNum + " Loses " + fmt.Sprint(int32(player.MBetMult*float64(player.MInitialBet))))
			}
			mult = -1
		}
		casinoEarnings += player.Settle(mult)
	}
	t.MCasinoEarnings = casinoEarnings
	if verbose {
		for _, player := range t.MPlayers {
			if player.MSplitFrom == nil {
				fmt.Println("Player " + player.MPlayerNum + " Earnings: " + fmt.Sprint(int32(player.MEarnings)))
			}
		}
		fmt.Println()
	}
	t.clear()
}

func (t *Table) print() {
	for _, player := range t.MPlayers {
		fmt.Println(player.Print())
	}
	fmt.Println(t.MDealer.Print())
	fmt.Println()
}
