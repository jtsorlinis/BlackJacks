require "./cardpile"
require "./dealer"
require "./player"
require "./strategies"

class Table
  getter verbose : Bool
  getter bet_size : Int32
  getter players : Array(Player)
  getter num_decks : Int32
  getter cardpile : CardPile
  getter min_cards : Int32
  getter dealer : Dealer
  property current_player : Int32
  property casino_earnings : Float64
  property running_count : Int32
  property true_count : Int32

  @strat_hard : Array(Char)
  @strat_soft : Array(Char)
  @strat_split : Array(Char)

  def initialize(
    num_players : Int32,
    @num_decks : Int32,
    @bet_size : Int32,
    @min_cards : Int32,
    @verbose : Bool
  )
    @players = fill(num_players, @bet_size)
    @cardpile = CardPile.new(@num_decks)
    @dealer = Dealer.new
    @current_player = 0
    @casino_earnings = 0.0
    @running_count = 0
    @true_count = 0
    @strat_hard = Strategies::MAP_HARD
    @strat_soft = Strategies::MAP_SOFT
    @strat_split = Strategies::MAP_SPLIT
  end

  def start_round : Nil
    update_count
    if @verbose
      puts "#{@cardpile.cards.size} cards left"
      puts "Running count is: #{@running_count}\tTrue count is: #{@true_count}"
    end
    get_new_cards
    pre_deal
    deal_round
    deal_dealer(false)
    deal_round
    deal_dealer(true)
    evaluate_all
    @current_player = 0
    if check_dealer_natural
      finish_round
    else
      check_player_natural
      print if @verbose
      auto_play
    end
  end

  def check_earnings : Nil
    check = 0.0
    @players.each do |player|
      check += player.earnings
    end
    if check + @casino_earnings != 0.0
      puts "NO MATCH\t Casino earnings: #{@casino_earnings}\t Player earnings: #{check}"
      exit(1)
    end
  end

  def clear : Nil
    i = @players.size - 1
    while i >= 0
      if @players[i].split_count > 0
        @players[i - 1].earnings += @players[i].earnings
        @players.delete_at(i)
      else
        @players[i].reset_hand
      end
      i -= 1
    end
    @dealer.reset_hand
    @current_player = 0
  end

  private def fill(num_players : Int32, bet_size : Int32) : Array(Player)
    temp = Array(Player).new(num_players * 3)
    i = 0
    while i < num_players
      temp << Player.new((i + 1).to_s, bet_size, 0)
      i += 1
    end
    temp
  end

  private def deal_round : Nil
    @players.size.times do
      deal
      @current_player += 1
    end
    @current_player = 0
  end

  private def evaluate_all : Nil
    @players.each do |player|
      player.evaluate
    end
  end

  private def deal : Nil
    card = @cardpile.cards.pop
    @running_count += card.count
    @players[@current_player].hand << card
  end

  private def pre_deal : Nil
    @players.each do |player|
      select_bet(player)
    end
  end

  private def select_bet(player : Player) : Nil
    if @true_count >= 2
      player.initial_bet = @bet_size * (@true_count - 1)
    end
  end

  private def deal_dealer(face_down : Bool = false) : Nil
    card = @cardpile.cards.pop
    @running_count += card.count unless face_down
    @dealer.hand << card
  end

  private def get_new_cards : Nil
    if @cardpile.cards.size < @min_cards
      @cardpile.refresh
      @cardpile.shuffle
      @true_count = 0
      @running_count = 0
      if @verbose
        puts "Got #{@num_decks} new decks as number of cards left is below #{@min_cards}"
      end
    end
  end

  private def update_count : Nil
    if @cardpile.cards.size > 51
      @true_count = @running_count // (@cardpile.cards.size.to_i32 // 52)
    end
  end

  private def hit : Nil
    deal
    @players[@current_player].evaluate
    if @verbose
      puts "Player #{@players[@current_player].player_num} hits"
    end
  end

  private def stand : Nil
    if @verbose && @players[@current_player].value <= 21
      puts "Player #{@players[@current_player].player_num} stands"
      print
    end
    @players[@current_player].is_done = true
  end

  private def split : Nil
    split_player_num = @players[@current_player].player_num + "S"
    split_player = Player.new(
      split_player_num,
      @players[@current_player].initial_bet,
      @players[@current_player].split_count + 1
    )
    split_player.hand << @players[@current_player].hand.pop
    @players.insert(@current_player + 1, split_player)
    @players[@current_player].evaluate
    @players[@current_player + 1].evaluate
    if @verbose
      puts "Player #{@players[@current_player].player_num} splits"
    end
  end

  private def split_aces : Nil
    if @verbose
      puts "Player #{@players[@current_player].player_num} splits aces"
    end
    split_player_num = @players[@current_player].player_num + "S"
    split_player = Player.new(
      split_player_num,
      @players[@current_player].initial_bet,
      @players[@current_player].split_count + 1
    )
    split_player.hand << @players[@current_player].hand.pop
    @players.insert(@current_player + 1, split_player)
    deal
    @players[@current_player].evaluate
    stand
    @current_player += 1
    deal
    @players[@current_player].evaluate
    stand
    print if @verbose
  end

  private def double_bet : Nil
    if @players[@current_player].bet_mult < 1.1 && @players[@current_player].hand.size == 2
      @players[@current_player].double_bet
      if @verbose
        puts "Player #{@players[@current_player].player_num} doubles"
      end
      hit
      stand
    else
      hit
    end
  end

  private def auto_play : Nil
    while @current_player < @players.size
      while !@players[@current_player].is_done
        if @players[@current_player].hand.size == 1
          if @verbose
            puts "Player #{@players[@current_player].player_num} gets 2nd card after splitting"
          end
          deal
          @players[@current_player].evaluate
        end

        if @players[@current_player].hand.size < 5 && @players[@current_player].value < 21
          split_player_val = @players[@current_player].can_split
          dealer_up = @dealer.up_card
          if split_player_val == 11
            split_aces
          elsif split_player_val != 0 && split_player_val != 5 && split_player_val != 10
            action(Strategies.get_action(split_player_val, dealer_up, @strat_split))
          elsif @players[@current_player].is_soft
            action(
              Strategies.get_action(
                @players[@current_player].value,
                dealer_up,
                @strat_soft
              )
            )
          else
            action(
              Strategies.get_action(
                @players[@current_player].value,
                dealer_up,
                @strat_hard
              )
            )
          end
        else
          stand
        end
      end
      @current_player += 1
    end
    @current_player = 0
    dealer_play
  end

  private def action(action : Char) : Nil
    case action
    when 'H'
      hit
    when 'S'
      stand
    when 'D'
      double_bet
    when 'P'
      split
    else
      puts "No action found"
      exit(1)
    end
  end

  private def dealer_play : Nil
    all_busted = true
    @players.each do |player|
      if player.value < 22
        all_busted = false
        break
      end
    end
    @dealer.hide_second = false
    @running_count += @dealer.hand[1].count
    @dealer.evaluate
    if @verbose
      puts "Dealer's turn"
      print
    end
    if all_busted
      if @verbose
        puts "Dealer automatically wins cause all players busted"
      end
      finish_round
    else
      while @dealer.value < 17 && @dealer.hand.size < 5
        deal_dealer(false)
        @dealer.evaluate
        if @verbose
          puts "Dealer hits"
          print
        end
      end
      finish_round
    end
  end

  private def check_player_natural : Nil
    @players.each do |player|
      if player.value == 21 && player.hand.size == 2 && player.split_count == 0
        player.has_natural = true
      end
    end
  end

  private def check_dealer_natural : Bool
    if @dealer.evaluate == 21
      @dealer.hide_second = false
      @running_count += @dealer.hand[1].count
      if @verbose
        print
        puts "Dealer has a natural 21"
        puts
      end
      return true
    end
    false
  end

  private def finish_round : Nil
    puts "Scoring round" if @verbose
    @players.each do |player|
      if player.has_natural
        @casino_earnings += player.win(1.5)
        if @verbose
          puts "Player #{player.player_num} Wins #{1.5 * player.bet_mult * player.initial_bet} with a natural 21"
        end
      elsif player.value > 21
        @casino_earnings += player.lose
        if @verbose
          puts "Player #{player.player_num} Busts and Loses #{player.bet_mult * player.initial_bet}"
        end
      elsif @dealer.value > 21 || player.value > @dealer.value
        @casino_earnings += player.win
        if @verbose
          puts "Player #{player.player_num} Wins #{player.bet_mult * player.initial_bet}"
        end
      elsif player.value == @dealer.value
        puts "Player #{player.player_num} Draws" if @verbose
      else
        @casino_earnings += player.lose
        if @verbose
          puts "Player #{player.player_num} Loses #{player.bet_mult * player.initial_bet}"
        end
      end
    end
    if @verbose
      @players.each do |player|
        if player.split_count == 0
          puts "Player #{player.player_num} Earnings: #{player.earnings}"
        end
      end
      puts
    end
    clear
  end

  private def print : Nil
    @players.each do |player|
      puts player.print
    end
    puts @dealer.print
    puts
  end
end
