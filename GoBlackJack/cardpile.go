package main

import (
	"math/bits"
	"time"
)

var state = uint64(time.Now().Unix())

// From https://github.com/lemire/testingRNG
func wyrand() uint64 {
	state += 0xa0761d6478bd642f
	hi, lo := bits.Mul64(state^0xe7037ed1a0b428db, state)
	return uint64(hi ^ lo)
}

// use nearly divisionless technique found here https://github.com/lemire/FastShuffleExperiments
func rand_range(s uint64) uint64 {
	x := wyrand()
	mhi, mlo := bits.Mul64(x, s)
	l := mlo
	if l < s {
		t := -s % s
		for l < t {
			x = wyrand()
			mhi, mlo = bits.Mul64(x, s)
			l = mlo
		}
	}
	return mhi
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
	var i = uint64(len(c.MCards) - 1)
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
