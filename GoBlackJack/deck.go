package main

import (
	"math/rand"
)

var suits = []string{"Clubs", "Hearts", "Spades", "Diamonds"}
var ranks = []string{"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"}

// Deck class
type Deck struct {
	MCards []Card
}

// NewDeck constructor
func NewDeck() Deck {
	var d Deck
	for _, suit := range suits {
		for _, rank := range ranks {
			c := NewCard(rank, suit)
			d.MCards = append(d.MCards, c)
		}
	}
	return d
}

// Print the cards
func (d *Deck) Print() string {
	output := ""
	for _, card := range d.MCards {
		output += card.Print()
	}
	return output
}

// Shuffle the cards
func (d *Deck) Shuffle() {
	rand.Shuffle(len(d.MCards), func(i, j int) {
		d.MCards[i], d.MCards[j] = d.MCards[j], d.MCards[i]
	})
}
