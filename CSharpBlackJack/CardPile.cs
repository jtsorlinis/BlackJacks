using System.Collections.Generic;

namespace CSharpBlackJack {
    internal class CardPile {
        private readonly List<Card> _originalCards;

        //Random rnd = new Random();
        private readonly QuickRand _rnd = new QuickRand();
        public List<Card> mCards = new List<Card>();

        public CardPile(int numOfDecks) {
            for (var x = 0; x < numOfDecks; x++) {
                var temp = new Deck();
                mCards.AddRange(temp.mCards);
            }

            _originalCards = new List<Card>(mCards);
        }

        public void Refresh() {
            mCards = new List<Card>(_originalCards);
        }

        public string Print() {
            var output = "";
            foreach (var card in mCards) output += card.Print() + "\n";
            return output;
        }

        public void Shuffle() {
            //mCards = mCards.OrderBy(item => rnd.Next()).ToList();

            // Fisher Yates
            for (var i = mCards.Count - 1; i > 0; i--) {
                var j = _rnd.Next() % (i + 1);
                var temp = mCards[i];
                mCards[i] = mCards[j];
                mCards[j] = temp;
            }
        }
    }
}