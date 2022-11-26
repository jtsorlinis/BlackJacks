using System.Collections.Generic;
using System.Runtime.InteropServices;
using System;

namespace CSharpBlackJack
{
  internal class CardPile
  {
    private readonly List<Card> _originalCards;

    private ulong state = (UInt64)DateTime.Now.Ticks;

    // From https://github.com/lemire/testingRNG
    private ulong wyRand()
    {
      state += 0xa0761d6478bd642f;
      var hi = Math.BigMul(state ^ 0xe7037ed1a0b428db, state, out ulong lo);
      return hi ^ lo;
    }

    // use nearly divisionless technique found here https://github.com/lemire/FastShuffleExperiments
    private ulong randRange(ulong s)
    {
      var x = wyRand();
      var mHi = Math.BigMul(x, s, out ulong mLo);
      if (mLo < s)
      {
        var t = (ulong.MaxValue - s + 1) % s;
        while (mLo < t)
        {
          x = wyRand();
          mHi = Math.BigMul(x, s, out mLo);
        }
      }
      return mHi;
    }

    public List<Card> mCards = new();

    public CardPile(int numOfDecks)
    {
      for (var x = 0; x < numOfDecks; x++)
      {
        var temp = new Deck();
        mCards.AddRange(temp.mCards);
      }

      _originalCards = new List<Card>(mCards);
    }

    public void Refresh()
    {
      mCards = _originalCards.GetRange(0, _originalCards.Count);
    }

    public string Print()
    {
      var output = "";
      foreach (var card in mCards) output += card.Print() + "\n";
      return output;
    }

    public void Shuffle()
    {
      // Fisher Yates
      Span<Card> cardsSpan = CollectionsMarshal.AsSpan(mCards);
      for (var i = mCards.Count - 1; i > 0; i--)
      {
        int j = (int)randRange((ulong)i + 1);
        (cardsSpan[i], cardsSpan[j]) = (cardsSpan[j], cardsSpan[i]);
      }
    }
  }
}