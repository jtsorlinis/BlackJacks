package deck

import "goblackjack/card"

var suits = []string{"Clubs", "Hearts", "Spades", "Diamonds"}
var ranks = []string{"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"}

type Deck struct {
	mCards []card.Card
}

func New() Deck {
	d := Deck{}
	for _, suit := range suits {
		for _, rank := range ranks {
			d.mCards = append(d.mCards, card.New(rank, suit))
		}
	}
	return d
}

func (d Deck) Print() string {
	output := ""
	for _, card := range d.mCards {
		output += card.Print() + "\n"
	}
	return output
}
