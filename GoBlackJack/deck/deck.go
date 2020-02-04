package deck

import (
	"goblackjack/card"
	"math/rand"
)

var suits = []string{"Clubs", "Hearts", "Spades", "Diamonds"}
var ranks = []string{"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"}
var rnd = rand.Seed

// Deck class
type Deck struct {
	MCards []*card.Card
}

// New deck constructor
func New() Deck {
	d := Deck{}
	for _, suit := range suits {
		for _, rank := range ranks {
			c := card.New(rank, suit)
			d.MCards = append(d.MCards, &c)
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
