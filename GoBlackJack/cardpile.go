package main

import "time"

var rnd = NewSource(time.Now().Unix())

// CardPile class
type CardPile struct {
	MCards         []*Card
	mOriginalCards []*Card
}

// NewCardPile constructor
func NewCardPile(numofdecks int32) *CardPile {
	cp := CardPile{}
	for x := int32(0); x < numofdecks; x++ {
		temp := NewDeck()
		cp.MCards = append(cp.MCards, temp.MCards...)
	}

	cp.mOriginalCards = append(cp.mOriginalCards, cp.MCards...)
	return &cp
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
	var i uint32 = uint32(len(c.MCards) - 1)
	for ; i > 0; i-- {
		j := uint32(rnd.Uint64()) % (i + 1)
		c.MCards[i], c.MCards[j] = c.MCards[j], c.MCards[i]
	}
}

// Refresh the cardpile
func (c *CardPile) Refresh() {
	c.MCards = append([]*Card(nil), c.mOriginalCards...)
}
