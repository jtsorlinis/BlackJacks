package main

import (
	"math/bits"
	"time"
)

var state = uint64(time.Now().Unix())

// From https://www.pcg-random.org/download.html#minimal-c-implementation
func yrand() uint32 {
	state += 0xa0761d6478bd642f
	hi, lo := bits.Mul64(state^0xe7037ed1a0b428db, state)
	return uint32(hi ^ lo)
}

// use nearly divisionless technique found here https://github.com/lemire/FastShuffleExperiments
func rand_range(s uint32) uint32 {
	x := yrand()
	m := uint64(x) * uint64(s)
	l := uint32(m)
	if l < s {
		t := -s % s
		for l < t {
			x = yrand()
			m = uint64(x) * uint64(s)
			l = uint32(m)
		}
	}
	return uint32(m >> 32)
}

// CardPile class
type CardPile struct {
	MCards         []*Card
	mOriginalCards []*Card
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
		j := rand_range(i + 1)
		c.MCards[i], c.MCards[j] = c.MCards[j], c.MCards[i]
	}
}

// Refresh the cardpile
func (c *CardPile) Refresh() {
	c.MCards = c.MCards[:0]
	c.MCards = append(c.MCards, c.mOriginalCards...)
}
