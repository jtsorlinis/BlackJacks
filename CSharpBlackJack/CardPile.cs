using System.Collections.Generic;
using System;

namespace CSharpBlackJack
{
  internal class CardPile
  {
    private readonly List<Card> _originalCards;

    private UInt64 state = (UInt64)DateTime.Now.Ticks;

    // From https://www.pcg-random.org/download.html#minimal-c-implementation
    private uint Pcg32()
    {
      UInt64 oldState = state;
      state = oldState * 6364136223846793005U + 1;
      UInt32 xorshifted = (UInt32)(((oldState >> 18) ^ oldState) >> 27);
      UInt32 rot = (UInt32)(oldState >> 59);
      return (xorshifted >> (int)rot) | (xorshifted << (((int)-rot) & 31));
    }

    // use nearly divisionless technique found here https://github.com/lemire/FastShuffleExperiments
    private UInt32 Pcg32Range(UInt32 s)
    {
      UInt32 x = Pcg32();
      UInt64 m = (UInt64)x * (UInt64)s;
      UInt32 l = (UInt32)m;
      if (l < s)
      {
        UInt32 t = (UInt32)(-s % s);
        while (l < t)
        {
          x = Pcg32();
          m = (UInt64)x * (UInt64)s;
          l = (UInt32)m;
        }
      }
      return (UInt32)(m >> 32);
    }

    public List<Card> mCards = new List<Card>();

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
      mCards.Clear();
      mCards.AddRange(_originalCards);
    }

    public string Print()
    {
      var output = "";
      foreach (var card in mCards) output += card.Print() + "\n";
      return output;
    }

    public void Shuffle()
    {
      //mCards = mCards.OrderBy(item => rnd.Next()).ToList();

      // Fisher Yates
      for (var i = mCards.Count - 1; i > 0; i--)
      {
        var j = (int)Pcg32Range((UInt32)(i + 1));
        (mCards[i], mCards[j]) = (mCards[j], mCards[i]);
      }
    }
  }
}