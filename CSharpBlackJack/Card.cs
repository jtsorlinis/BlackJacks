namespace CSharpBlackJack
{
  internal class Card
  {
    public readonly int mCount;
    public readonly bool mIsAce;
    public readonly string mRank;
    public readonly int mValue;
    private readonly string _suit;
    public bool mFaceDown = false;

    public Card(string rank, string suit)
    {
      mRank = rank;
      _suit = suit;
      mValue = Evaluate();
      mCount = Count();
      if (mRank == "A") mIsAce = true;
    }

    public string Print()
    {
      return mFaceDown ? "X" : mRank;
    }

    private int Evaluate()
    {
      return mRank switch
      {
        "J" => 10,
        "Q" => 10,
        "K" => 10,
        "A" => 11,
        _ => int.Parse(mRank)
      };
    }

    private int Count()
    {
      return mRank switch
      {
        "10" => -1,
        "J" => -1,
        "Q" => -1,
        "K" => -1,
        "A" => -1,
        "7" => 0,
        "8" => 0,
        "9" => 0,
        _ => 1
      };
    }
  }
}