package main

//Table class
type Table struct {
	MVerbose        bool
	MBetSize        int32
	MPlayers        []*Player
	MNumOfDecks     int32
	MCardPile       *CardPile
	MMinCards       int32
	MDealer         *Dealer
	MCasinoEarnings float32
}

// NewTable constructor
func NewTable(numplayers int32, numdecks int32, betsize int32, mincards int32, verbose bool) {

}
