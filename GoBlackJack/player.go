package main

import (
	"fmt"
	"strings"
)

var playerNumCount int32
var maxHands int32 = 4

// Player class
type Player struct {
	MPlayerNum  string
	MHand       []*Card
	MValue      int32
	MEarnings   float32
	MAces       int32
	MIsSoft     bool
	MSplitCount *int32
	MIsDone     bool
	MSplitFrom  *Player
	MBetMult    float32
	MHasNatural bool
	MTable      *Table
	MInitialBet int32
}

// NewPlayer constructor
func NewPlayer(table *Table, split *Player) *Player {
	p := new(Player)
	p.MTable = table
	p.MInitialBet = p.MTable.MBetSize
	p.MBetMult = 1
	p.MHand = make([]*Card, 0, 5)
	splitCount := int32(0)
	p.MSplitCount = &splitCount
	if split != nil {
		p.MSplitCount = split.MSplitCount
		*p.MSplitCount = *p.MSplitCount + 1
		p.MHand = append(p.MHand, split.MHand[1])
		p.MPlayerNum = split.MPlayerNum + "S"
		p.MInitialBet = split.MInitialBet
		p.MSplitFrom = split
	} else {
		playerNumCount++
		p.MPlayerNum = fmt.Sprint(playerNumCount)
	}
	return p
}

// DoubleBet doubles the player's bet
func (p *Player) DoubleBet() {
	p.MBetMult = 2
}

// ResetHand resets the player's hand
func (p *Player) ResetHand() {
	p.MHand = p.MHand[:0]
	p.MValue = 0
	p.MAces = 0
	p.MIsSoft = false
	*p.MSplitCount = 0
	p.MIsDone = false
	p.MBetMult = 1
	p.MHasNatural = false
	p.MInitialBet = p.MTable.MBetSize
}

// CanSplit checks if the player can split
func (p *Player) CanSplit() int32 {
	if len(p.MHand) == 2 && p.MHand[0].MRank[0] == p.MHand[1].MRank[0] && *p.MSplitCount < maxHands-1 {
		return p.MHand[0].MValue
	}
	return 0
}

// Win increases player earnings
func (p *Player) Win(mult float32) {
	x := float32(p.MInitialBet) * p.MBetMult * mult
	p.MEarnings += x
	p.MTable.MCasinoEarnings -= x

}

// Lose decreases player earnings
func (p *Player) Lose() {
	x := float32(p.MInitialBet) * p.MBetMult
	p.MEarnings -= x
	p.MTable.MCasinoEarnings += x
}

// Print prints the players number and hand
func (p *Player) Print() string {
	var output strings.Builder
	output.WriteString("Player " + fmt.Sprint(p.MPlayerNum) + ": ")
	for _, card := range p.MHand {
		output.WriteString(card.Print() + " ")
	}
	for i := len(p.MHand); i < 5; i++ {
		output.WriteString("  ")
	}
	output.WriteString("\tScore: " + fmt.Sprint(p.MValue))
	if p.MValue > 21 {
		output.WriteString(" (Bust) ")
	} else {
		output.WriteString("       ")
	}
	output.WriteString("\tBet: ")
	output.WriteString(fmt.Sprint(float32(p.MInitialBet) * p.MBetMult))
	return output.String()
}

// Evaluate evaluates the player's hand
func (p *Player) Evaluate() {
	p.MAces = 0
	p.MValue = 0
	for _, card := range p.MHand {
		p.MValue += card.MValue
		// check for ace
		if card.MIsAce {
			p.MAces++
			p.MIsSoft = true
		}
	}

	for p.MValue > 21 && p.MAces > 0 {
		p.MValue -= 10
		p.MAces--
	}

	if p.MAces == 0 {
		p.MIsSoft = false
	}
}
