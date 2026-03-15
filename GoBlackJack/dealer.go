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
	d.MAces = 0
	d.MValue = 0
	for _, card := range d.MHand {
		d.MValue += card.MValue
		// check for ace
		if card.MIsAce {
			d.MAces++
			d.MIsSoft = true
		}
	}

	for d.MValue > 21 && d.MAces > 0 {
		d.MValue -= 10
		d.MAces--
	}

	if d.MAces == 0 {
		d.MIsSoft = false
	}
}

// UpCard returns first card in dealer's hand
func (d *Dealer) UpCard() int32 {
	return d.MHand[0].MValue
}
