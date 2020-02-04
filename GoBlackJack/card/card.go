package card

import "strconv"

// Card class
type Card struct {
	MRank     string
	mSuit     string
	mFaceDown bool
	mValue    int32
	mCount    int32
	mIsAce    bool
}

//New card constructor
func New(rank string, suit string) *Card {
	c := Card{rank, suit, false, 0, 0, false}
	c.mValue = c.evaluate()
	c.mCount = c.count()
	c.mIsAce = c.isAce()
	return &c
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

func (c *Card) isAce() bool {
	if c.MRank == "A" {
		return true
	}
	return false
}
