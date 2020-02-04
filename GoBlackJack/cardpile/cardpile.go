package cardpile

import (
	"goblackjack/card"
	"goblackjack/deck"
	"math/rand"
)

type CardPile struct {
	deck.Deck
	MCards []*card.Card
}

func New(numofdecks int32) CardPile {
	cp := CardPile{}
	for x := int32(0); x < numofdecks; x++ {
		temp := deck.New()
		cp.MCards = append(cp.MCards, temp.MCards...)
	}
	return cp
}

// Print the cards
func (c CardPile) Print() string {
	output := ""
	for _, card := range c.MCards {
		output += card.Print()
	}
	return output
}

// Shuffle the cards
func (c CardPile) Shuffle() {
	rand.Shuffle(len(c.MCards), func(i, j int) {
		c.MCards[i], c.MCards[j] = c.MCards[j], c.MCards[i]
	})
}
