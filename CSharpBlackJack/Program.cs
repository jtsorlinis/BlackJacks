using System;
using System.Diagnostics;
using System.Globalization;

namespace CSharpBlackJack
{
  internal static class Program
  {
    public const bool Verbose = false;

    private static void Main(string[] args)
    {
      const int numOfPlayers = 5;
      const int numOfDecks = 8;
      const int betSize = 10;
      const int minCards = 52;

      var rounds = 1000000;

      if (args.Length == 1) rounds = int.Parse(args[0]);
      CultureInfo.CurrentCulture = CultureInfo.InvariantCulture;

      var table1 = new Table(numOfPlayers, numOfDecks, betSize, minCards);
      table1.mCardPile.Shuffle();

      var timer = new Stopwatch();
      timer.Start();

      var x = 0;
      while (x++ < rounds)
      {
        if (Verbose) Console.WriteLine("Round " + x);
        if (!Verbose && rounds > 1000 && x % (rounds / 100) == 0)
          Console.Error.Write("\tProgress: " + x * 100 / rounds + "%\r");

        table1.StartRound();
        table1.CheckEarnings();
      }

      table1.Clear();

      foreach (var player in table1.mPlayers)
        Console.WriteLine("Player " + player.mPlayerNum + " earnings: " + (int)player.mEarnings +
                          "\t\tWin Percentage: " +
                          (50 + player.mEarnings / ((double)rounds * betSize) * 50).ToString("F2") + "%");
      Console.WriteLine("Casino earnings: " + (int)table1.mCasinoEarnings);
      Console.WriteLine("Played " + rounds + " rounds in " + timer.Elapsed.TotalSeconds.ToString("F3") + " seconds");
    }
  }
}
