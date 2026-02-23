class Card
  getter rank : String
  getter suit : String
  property face_down : Bool
  getter value : Int32
  getter count : Int32
  getter is_ace : Bool

  def initialize(@rank : String, @suit : String)
    @face_down = false
    @value = evaluate
    @count = card_count
    @is_ace = @rank == "A"
  end

  def print : String
    @face_down ? "X" : @rank
  end

  private def evaluate : Int32
    case @rank
    when "J", "Q", "K"
      10
    when "A"
      11
    else
      @rank.to_i32
    end
  end

  private def card_count : Int32
    case @rank
    when "10", "J", "Q", "K", "A"
      -1
    when "7", "8", "9"
      0
    else
      1
    end
  end
end
