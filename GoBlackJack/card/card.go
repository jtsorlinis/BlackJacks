package card

import "strconv"

// Card class
type Card struct {
	mRank     string
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
	return c.mRank
}

func (c *Card) evaluate() int32 {
	if c.mRank == "J" || c.mRank == "Q" || c.mRank == "K" {
		return 10
	} else if c.mRank == "A" {
		return 11
	} else {
		r, _ := strconv.Atoi(c.mRank)
		return int32(r)
	}
}

func (c *Card) count() int32 {
	if c.mRank == "10" || c.mRank == "J" || c.mRank == "Q" || c.mRank == "K" || c.mRank == "A" {
		return -1
	} else if c.mRank == "7" || c.mRank == "8" || c.mRank == "9" {
		return 0
	} else {
		return 1
	}
}

func (c *Card) isAce() bool {
	if c.mRank == "A" {
		return true
	}
	return false
}
