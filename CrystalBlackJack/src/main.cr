require "./table"

NUM_PLAYERS = 5
NUM_DECKS = 8
BET_SIZE = 10
MIN_CARDS = 40
VERBOSE = false

rounds = 1_000_000
if ARGV.size == 1
  rounds = ARGV[0].to_i32
end

table1 = Table.new(NUM_PLAYERS, NUM_DECKS, BET_SIZE, MIN_CARDS, VERBOSE)
table1.cardpile.shuffle

start = Time.instant
(1..rounds).each do |x|
  if VERBOSE
    puts "Round #{x}"
  end
  if !VERBOSE && rounds > 1000 && x % (rounds // 100) == 0
    STDERR.print("\rProgress: #{x * 100 // rounds}%")
  end

  table1.start_round
  table1.check_earnings
end

table1.clear
print "\r"
table1.players.each do |player|
  win_percentage = 50.0 + (player.earnings / (rounds * BET_SIZE).to_f64 * 50.0)
  puts "Player #{player.player_num} earnings: #{player.earnings}\t\tWin Percentage: #{win_percentage}%"
end
puts "Casino earnings: #{table1.casino_earnings}"

elapsed = Time.instant - start
printf "Played %d rounds in %.3f seconds\n", rounds, elapsed.total_milliseconds / 1000.0
