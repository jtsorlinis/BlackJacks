import time
import sys
from table import Table


def main():

    players = 5
    decks = 8
    bet_size = 10
    min_cards = 52

    rounds = 1000000
    verbose = False

    if len(sys.argv) == 2:
        rounds = int(sys.argv[1])

    table1 = Table(players, decks, bet_size, min_cards, verbose)
    table1.cardpile.shuffle()

    start_time = time.perf_counter()
    for x in range(1, rounds + 1):
        if verbose:
            print("Round " + str(x))
        if not verbose and rounds > 1000 and x % (rounds // 100) == 0:
            print("\tProgress: " + str(x * 100 // rounds) + "%",
                  end="\r", file=sys.stderr, flush=True)
        table1.start_round()
        table1.check_earnings()

    table1.clear()

    for player in table1.players:
        print(
            "Player "
            + str(player.player_num)
            + " earnings: "
            + str(int(player.earnings))
            + "\t\tWin Percentage: "
            + format(50 + player.earnings / (rounds * bet_size) * 50, ".2f")
            + "%"
        )
    print("Casino earnings: " + str(int(table1.casino_earnings)))

    print(
        "Played "
        + str(rounds)
        + " rounds in "
        + format(time.perf_counter() - start_time, ".3f")
        + " seconds"
    )


if __name__ == "__main__":
    main()
