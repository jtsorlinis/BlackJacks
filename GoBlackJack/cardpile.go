package main

import (
	"time"
)

// var rnd = NewSource(time.Now().Unix())
var seed = time.Now().Unix()

func xorShift() uint32 {
	seed ^= seed << 13
	seed ^= seed >> 17
	seed ^= seed << 5
	return uint32(seed)
}

// CardPile class
type CardPile struct {
	MCards         []*Card
	mOriginalCards []Card
}

// NewCardPile constructor
func NewCardPile(numofdecks int32) CardPile {
	var cp CardPile
	for x := int32(0); x < numofdecks; x++ {
		temp := NewDeck()
		cp.mOriginalCards = append(cp.mOriginalCards, temp.MCards...)
	}

	cp.Refresh()
	return cp
}

// Print the cards
func (c *CardPile) Print() string {
	output := ""
	for _, card := range c.MCards {
		output += card.Print()
	}
	return output
}

// Shuffle the cards
func (c *CardPile) Shuffle() {
	var i = uint32(len(c.MCards) - 1)
	for ; i > 0; i-- {
		j := xorShift() % (i + 1)
		c.MCards[i], c.MCards[j] = c.MCards[j], c.MCards[i]
	}
}

// Refresh the cardpile
func (c *CardPile) Refresh() {
	c.MCards = c.MCards[:0]
	for i := range c.mOriginalCards {
		c.MCards = append(c.MCards, &c.mOriginalCards[i])
	}
}
