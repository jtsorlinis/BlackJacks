using System;

namespace NETCoreBlackJack
{
    class Program
    {
        static void Main(string[] args)
        {
            CardPile test = new CardPile(5);
            test.Shuffle();
            Console.WriteLine(test.Print());
        }
    }
}
