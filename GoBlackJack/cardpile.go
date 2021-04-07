package main

import (
	"time"
)

var state uint64 = uint64(time.Now().Unix())

// From https://www.pcg-random.org/download.html#minimal-c-implementation
func pcg32() uint32 {
	oldState := state
	state = oldState*6364136223846793005 + 1
	xorshifted := uint32(((oldState >> 18) ^ oldState) >> 27)
	rot := uint32(oldState >> 59)
	return (xorshifted >> rot) | (xorshifted << ((-rot) & 31))
}

// use nearly divisionless technique found here https://github.com/lemire/FastShuffleExperiments
func pcg32Range(s uint32) uint32 {
	x := pcg32()
	m := uint64(x) * uint64(s)
	l := uint32(m)
	if l < s {
		t := -s % s
		for l < t {
			x = pcg32()
			m = uint64(x) * uint64(s)
			l = uint32(m)
		}
	}
	return uint32(m >> 32)
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
		j := pcg32Range(i + 1)
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
