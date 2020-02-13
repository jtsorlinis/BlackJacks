using System;
using System.Diagnostics;

namespace CSharpBlackJack {
    internal static class Program {
        private static void Main(string[] args) {
            const int numOfPlayers = 5;
            const int numOfDecks = 8;
            const int betSize = 10;
            const int minCards = 40;

            var rounds = 1000000;
            int verbosity = 0;

            if (args.Length == 1) rounds = int.Parse(args[0]);

            var table1 = new Table(numOfPlayers, numOfDecks, betSize, minCards);
            table1.mCardPile.Shuffle();

            var timer = new Stopwatch();
            timer.Start();

            var x = 0;
            while (x++ < rounds) {
                if (verbosity > 0) Console.WriteLine("Round " + x);
                if (verbosity == 0 && rounds > 1000 && x % (rounds / 100) == 0)
                    Console.Write("\tProgress: " + (int) ((float) x / rounds * 100) + "\r");

                table1.StartRound();
                table1.CheckEarnings();
            }

            table1.Clear();

            foreach (var player in table1.mPlayers)
                Console.WriteLine("Player " + player.mPlayerNum + " earnings: " + player.mEarnings +
                                  "\t\tWin Percentage: " + (50 + player.mEarnings / (rounds * betSize) * 50f) + "%");
            Console.WriteLine("Casino earnings: " + table1.mCasinoEarnings);
            Console.WriteLine("Played " + rounds + " rounds in " + timer.ElapsedMilliseconds / 1000f + " seconds");
        }
    }
}