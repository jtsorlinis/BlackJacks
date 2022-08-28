namespace CSharpBlackJack
{
  internal class Dealer : Player
  {
    public Dealer()
    {
      mPlayerNum = "D";
      mValue = 0;
    }

    public override void ResetHand()
    {
      mHand.Clear();
      mValue = 0;
    }

    public int UpCard()
    {
      return mHand[0].mValue;
    }
  }
}