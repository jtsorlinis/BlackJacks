package main

import (
	"math/rand"
	"time"
)

// CardPile class
type CardPile struct {
	MCards         []*Card
	mOriginalCards []*Card
}

// NewCardPile constructor
func NewCardPile(numofdecks int32) *CardPile {
	rand.Seed(time.Now().Unix())
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
	rand.Shuffle(len(c.MCards), func(i, j int) {
		c.MCards[i], c.MCards[j] = c.MCards[j], c.MCards[i]
	})
}

// Refresh the cardpile
func (c *CardPile) Refresh() {
	c.MCards = append([]*Card(nil), c.mOriginalCards...)
}
