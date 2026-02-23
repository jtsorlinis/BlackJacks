module Strategies
  def self.vec_to_map(vec : Array(Array(String))) : Array(Char)
    temp = Array(Char).new(300, ' ')
    row = 0
    while row < vec.size
      col = 0
      while col < vec[0].size
        player_val = vec[row][0].to_i32
        dealer_val = vec[0][col].to_i32
        key = player_val * 12 + dealer_val
        temp[key] = vec[row][col][0]
        col += 1
      end
      row += 1
    end
    temp
  end

  STRAT_HARD = [
    %w(0 2 3 4 5 6 7 8 9 10 11),
    %w(2 H H H H H H H H H H),
    %w(3 H H H H H H H H H H),
    %w(4 H H H H H H H H H H),
    %w(5 H H H H H H H H H H),
    %w(6 H H H H H H H H H H),
    %w(7 H H H H H H H H H H),
    %w(8 H H H H H H H H H H),
    %w(9 H D D D D H H H H H),
    %w(10 D D D D D D D D H H),
    %w(11 D D D D D D D D D H),
    %w(12 H H S S S H H H H H),
    %w(13 S S S S S H H H H H),
    %w(14 S S S S S H H H H H),
    %w(15 S S S S S H H H H H),
    %w(16 S S S S S H H H H H),
    %w(17 S S S S S S S S S S),
    %w(18 S S S S S S S S S S),
    %w(19 S S S S S S S S S S),
    %w(20 S S S S S S S S S S),
    %w(21 S S S S S S S S S S),
  ]

  STRAT_SOFT = [
    %w(0 2 3 4 5 6 7 8 9 10 11),
    %w(13 H H H D D H H H H H),
    %w(14 H H H D D H H H H H),
    %w(15 H H D D D H H H H H),
    %w(16 H H D D D H H H H H),
    %w(17 H D D D D H H H H H),
    %w(18 S D D D D S S H H H),
    %w(19 S S S S S S S S S S),
    %w(20 S S S S S S S S S S),
    %w(21 S S S S S S S S S S),
  ]

  STRAT_SPLIT = [
    %w(0 2 3 4 5 6 7 8 9 10 11),
    %w(2 P P P P P P H H H H),
    %w(3 P P P P P P H H H H),
    %w(4 H H H P P H H H H H),
    %w(6 P P P P P H H H H H),
    %w(7 P P P P P P H H H H),
    %w(8 P P P P P P P P P P),
    %w(9 P P P P P S P P S S),
    %w(11 P P P P P P P P P P),
  ]

  MAP_HARD = vec_to_map(STRAT_HARD)
  MAP_SOFT = vec_to_map(STRAT_SOFT)
  MAP_SPLIT = vec_to_map(STRAT_SPLIT)

  def self.get_action(player_val : Int32, dealer_val : Int32, strategy : Array(Char)) : Char
    key = player_val * 12 + dealer_val
    strategy[key]
  end
end
