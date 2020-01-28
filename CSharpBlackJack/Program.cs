using System;
using System.Diagnostics;

namespace NETCoreBlackJack
{
    class Program
    {
        static void Main(string[] args)
        {
            int numOfPlayers = 5;
            int numOfDecks = 8;
            int betSize = 10;
            int minCards = 40;

            int rounds = 100000;
            int verbosity = 0;

            if(args.Length == 1) {
                rounds = int.Parse(args[0]);
            }

            Table table1 = new Table(numOfPlayers, numOfDecks, betSize, minCards, verbosity);
            table1.mCardPile.Shuffle();
            
            Stopwatch timer = new Stopwatch();
            timer.Start();

            int x = 0;
            while(x++ < rounds) {
                if(verbosity > 0) {
                    Console.WriteLine("Round " + x);
                }
                if(verbosity == 0 && rounds > 1000 && x % (rounds/100) == 0) {
                    Console.Write("\tProgress: " + (int)(((float)x/rounds)*100) + "\r");
                }

                table1.StartRound();
                table1.CheckEarnings();
            }

            table1.Clear();

            foreach(var player in table1.mPlayers) {
                Console.WriteLine("Player " + player.mPlayerNum + " earnings: " + player.mEarnings + "\t\tWin Percentage: " + (50 + (player.mEarnings/(rounds*betSize)*50f)) + "%");
            }
            Console.WriteLine("Casino earnings: " + table1.mCasinoEarnings);
            Console.WriteLine("Played " + rounds + " rounds in " + timer.ElapsedMilliseconds/1000f + " seconds");
       
        }
    }
}
