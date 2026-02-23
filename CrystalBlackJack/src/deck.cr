require "./card"

class Deck
  RANKS = ["A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"]
  SUITS = ["Clubs", "Hearts", "Spades", "Diamonds"]

  getter cards : Array(Card)

  def initialize
    @cards = [] of Card
    SUITS.each do |suit|
      RANKS.each do |rank|
        @cards << Card.new(rank, suit)
      end
    end
  end
end
