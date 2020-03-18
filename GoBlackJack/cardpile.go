package main

import "time"

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
	mOriginalCards []*Card
}

// NewCardPile constructor
func NewCardPile(numofdecks int32) *CardPile {
	cp := new(CardPile)
	for x := int32(0); x < numofdecks; x++ {
		temp := NewDeck()
		cp.MCards = append(cp.MCards, temp.MCards...)
	}

	cp.mOriginalCards = append(cp.mOriginalCards, cp.MCards...)
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
	c.MCards = append(c.mOriginalCards[:0:0], c.mOriginalCards...)
}
