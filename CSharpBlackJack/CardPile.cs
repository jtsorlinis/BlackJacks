using System.Collections.Generic;
using System;

namespace CSharpBlackJack {
    internal class CardPile {
        private readonly List<Card> _originalCards;

        //Random rnd = new Random();
        // private readonly QuickRand _rnd = new QuickRand();
        private uint _seed = (uint)DateTime.Now.Ticks;

        private uint XorShift() {
            _seed ^= _seed << 13;
            _seed ^= _seed >> 17;
            _seed ^= _seed << 5;
            return _seed;
}

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
            for (var i = (uint)mCards.Count - 1; i > 0; i--) {
                var j = XorShift() % (i + 1);
                var temp = mCards[(int)i];
                mCards[(int)i] = mCards[(int)j];
                mCards[(int)j] = temp;
            }
        }
    }
}