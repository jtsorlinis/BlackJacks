package player

import (
	"GoBlackJack/card"
	"GoBlackJack/table"
	"strconv"
)

var playerNumCount = 0
var maxSplits = 10

// Player class
type Player struct {
	MPlayerNum  string
	MHand       []*card.Card
	MValue      int32
	MEarnings   float32
	MAces       int32
	MIsSoft     bool
	MSplitCount int32
	MIsDone     bool
	MSplitFrom  *Player
	MBetMult    float32
	MHasNatural bool
	MTable      *table.Table
	MInitialBet int32
}

// New Player constructor
func New(table *table.Table, split *Player) *Player {
	p := Player{"", nil, 0, 0, 0, false, 0, false, nil, 1, false, nil, 0}
	p.MTable = table
	p.MInitialBet = p.MTable.MBetSize
	if split != nil {
		p.MHand = append(p.MHand, split.MHand[1])
		p.MSplitCount++
		p.MPlayerNum = split.MPlayerNum + "S"
		p.MSplitFrom = split
	} else {
		playerNumCount++
		p.MPlayerNum = strconv.Itoa(playerNumCount)
	}
	return &p
}

// DoubleBet doubles the player's bet
func (p *Player) DoubleBet() {
	p.MBetMult = 2
}

// ResetHand resets the player's hand
func (p *Player) ResetHand() {
	p.MHand = nil
	p.MValue = 0
	p.MAces = 0
	p.MIsSoft = false
	p.MSplitCount = 0
	p.MIsDone = false
	p.MBetMult = 1
	p.MHasNatural = false
	p.MInitialBet = p.MTable.MBetSize
}
