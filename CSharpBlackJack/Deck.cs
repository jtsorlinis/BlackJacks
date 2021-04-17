using System.Collections.Generic;

namespace CSharpBlackJack
{
  internal class Deck
  {
    private readonly List<string> _ranks = new() { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };

    private readonly List<string> _suits = new() { "Clubs", "Hearts", "Spades", "Diamonds" };
    public readonly List<Card> mCards = new();

    public Deck()
    {
      foreach (var suit in _suits)
        foreach (var rank in _ranks)
          mCards.Add(new Card(rank, suit));
    }
  }
}