require "./deck"

class CardPile
  getter cards : Array(Card)
  getter original_cards : Array(Card)

  @state : UInt64

  def initialize(num_decks : Int32)
    @cards = [] of Card
    num_decks.times do
      temp_deck = Deck.new
      @cards.concat(temp_deck.cards)
    end
    @original_cards = @cards.dup
    @state = Time.utc.to_unix_ms.to_u64
  end

  def refresh : Nil
    @cards = @original_cards.dup
  end

  def shuffle : Nil
    i = @cards.size - 1
    while i > 0
      j = rand_range((i + 1).to_u64).to_i
      @cards[i], @cards[j] = @cards[j], @cards[i]
      i -= 1
    end
  end

  private def wyrand : UInt64
    @state &+= 0xa0761d6478bd642f_u64
    t = @state.to_u128 * (@state ^ 0xe7037ed1a0b428db_u64).to_u128
    ((t >> 64) ^ t).to_u64!
  end

  private def rand_range(s : UInt64) : UInt64
    x = wyrand
    m = x.to_u128 * s.to_u128
    l = m.to_u64!
    if l < s
      thresh = (0_u64 &- s) % s
      while l < thresh
        x = wyrand
        m = x.to_u128 * s.to_u128
        l = m.to_u64!
      end
    end
    (m >> 64).to_u64
  end
end
