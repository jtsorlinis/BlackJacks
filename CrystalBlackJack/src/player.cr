require "./card"

class Player
  MAX_SPLITS = 10

  getter player_num : String
  getter hand : Array(Card)
  property value : Int32
  property earnings : Float64
  property aces : Int32
  property is_soft : Bool
  property split_count : Int32
  property is_done : Bool
  property bet_mult : Float64
  property has_natural : Bool
  property initial_bet : Int32
  getter original_bet : Int32

  def initialize(@player_num : String, bet_size : Int32, @split_count : Int32 = 0)
    @hand = Array(Card).new(5)
    @value = 0
    @earnings = 0.0
    @aces = 0
    @is_soft = false
    @is_done = false
    @bet_mult = 1.0
    @has_natural = false
    @initial_bet = bet_size
    @original_bet = bet_size
  end

  def double_bet : Nil
    @bet_mult = 2.0
  end

  def reset_hand : Nil
    @hand.clear
    @value = 0
    @aces = 0
    @is_soft = false
    @split_count = 0
    @is_done = false
    @bet_mult = 1.0
    @has_natural = false
    @initial_bet = @original_bet
  end

  def can_split : Int32
    if @hand.size == 2 && @hand[0].rank == @hand[1].rank && @split_count < MAX_SPLITS
      @hand[0].value
    else
      0
    end
  end

  def win(mult : Float64 = 1.0) : Float64
    x = @initial_bet.to_f64 * @bet_mult * mult
    @earnings += x
    -x
  end

  def lose : Float64
    x = @initial_bet.to_f64 * @bet_mult
    @earnings -= x
    x
  end

  def print : String
    output = "Player #{@player_num}: "
    @hand.each { |card| output += "#{card.print} " }
    (@hand.size...5).each { output += "  " }
    output += "\tScore: #{@value}"
    output += @value > 21 ? " (Bust) " : "        "
    output += "\tBet: #{@initial_bet.to_f64 * @bet_mult}"
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
