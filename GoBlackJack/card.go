package main

import (
	"strconv"
)

// Card class
type Card struct {
	MRank     string
	mSuit     string
	mFaceDown bool
	MValue    int32
	mCount    int32
	MIsAce    bool
}

//NewCard constructor
func NewCard(rank string, suit string) *Card {
	c := new(Card)
	c.MRank = rank
	c.mSuit = suit
	c.MValue = c.evaluate()
	c.mCount = c.count()
	c.MIsAce = c.MRank == "A"
	return c
}

// Print the current value of the card
func (c *Card) Print() string {
	if c.mFaceDown {
		return "X"
	}
	return c.MRank
}

func (c *Card) evaluate() int32 {
	if c.MRank == "J" || c.MRank == "Q" || c.MRank == "K" {
		return 10
	} else if c.MRank == "A" {
		return 11
	} else {
		r, _ := strconv.Atoi(c.MRank)
		return int32(r)
	}
}

func (c *Card) count() int32 {
	if c.MRank == "10" || c.MRank == "J" || c.MRank == "Q" || c.MRank == "K" || c.MRank == "A" {
		return -1
	} else if c.MRank == "7" || c.MRank == "8" || c.MRank == "9" {
		return 0
	} else {
		return 1
	}
}
