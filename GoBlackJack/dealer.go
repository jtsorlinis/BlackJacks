package main

import "strings"

import "fmt"

// Dealer class
type Dealer struct {
	MPlayerNum string
	MHand      []*Card
	MValue     int32
	MAces      int32
	MIsSoft    bool
}

// NewDealer constructor
func NewDealer() *Dealer {
	d := new(Dealer)
	d.MPlayerNum = "D"
	return d
}

// ResetHand resets dealer's hand
func (d *Dealer) ResetHand() {
	d.MHand = d.MHand[:0]
	d.MValue = 0
}

// Print prints the dealers number and hand
func (d *Dealer) Print() string {
	var output strings.Builder
	output.WriteString("Player " + fmt.Sprint(d.MPlayerNum) + ": ")
	for _, card := range d.MHand {
		output.WriteString(card.Print() + " ")
	}
	for i := len(d.MHand); i < 5; i++ {
		output.WriteString("  ")
	}
	output.WriteString("\tScore: " + fmt.Sprint(d.MValue))
	if d.MValue > 21 {
		output.WriteString(" (Bust) ")
	} else {
		output.WriteString("        ")
	}
	return output.String()
}

// Evaluate evaluates the player's hand
func (d *Dealer) Evaluate() {
	var value, aces int32
	for _, card := range d.MHand {
		value += card.MValue
		// check for ace
		if card.MIsAce {
			aces++
		}
	}

	for value > 21 && aces > 0 {
		value -= 10
		aces--
	}

	d.MValue = value
	d.MAces = aces
	d.MIsSoft = aces > 0
}

// UpCard returns first card in dealer's hand
func (d *Dealer) UpCard() int32 {
	return d.MHand[0].MValue
}
