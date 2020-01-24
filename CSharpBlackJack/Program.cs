using System;

namespace NETCoreBlackJack
{
    class Program
    {
        static void Main(string[] args)
        {

            Console.WriteLine(Strategies.getAction(9, 10, Strategies.Array2dToMap(Strategies.stratHard)));
        }
    }
}
