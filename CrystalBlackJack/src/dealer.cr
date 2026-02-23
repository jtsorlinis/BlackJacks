require "./card"

class Dealer
  getter hand : Array(Card)
  getter player_num : String
  property value : Int32
  property aces : Int32
  property is_soft : Bool
  property hide_second : Bool

  def initialize
    @hand = Array(Card).new(5)
    @player_num = "D"
    @value = 0
    @aces = 0
    @is_soft = false
    @hide_second = true
  end

  def up_card : Int32
    @hand[0].value
  end

  def reset_hand : Nil
    @hand.clear
    @value = 0
    @aces = 0
    @is_soft = false
    @hide_second = true
  end

  def print : String
    output = "Player #{@player_num}: "
    @hand.each_with_index do |card, i|
      if i == 1 && @hide_second
        output += "X "
      else
        output += "#{card.print} "
      end
    end
    (@hand.size...5).each { output += "  " }
    output += "\tScore: #{@value}"
    output += @value > 21 ? " (Bust) " : "        "
    output
  end

  def evaluate : Int32
    @aces = 0
    @value = 0
    @is_soft = false

    @hand.each do |card|
      @value += card.value
      if card.is_ace
        @aces += 1
        @is_soft = true
      end
    end

    while @value > 21 && @aces > 0
      @value -= 10
      @aces -= 1
    end

    @is_soft = false if @aces == 0
    @value
  end
end
