using System;

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

            int rounds = 100;
            int verbosity = 1;

            Table table1 = new Table(numOfPlayers, numOfDecks, betSize, minCards, verbosity);
            table1.StartRound();
       
        }
    }
}
