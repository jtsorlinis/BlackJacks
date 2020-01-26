using System;
using System.Collections.Generic;
using System.Linq;

namespace NETCoreBlackJack {
    class CardPile {

        public List<Card> mCards = new List<Card>();
        public List<Card> mOriginalCards = new List<Card>();

        public CardPile(int numOfDecks) {
            for(int x = 0; x < numOfDecks; x++) {
                Deck temp = new Deck();
                mCards.AddRange(temp.mCards);
            }
            mOriginalCards = new List<Card>(mCards);
        }

        public void Refresh() {
            mCards = new List<Card>(mOriginalCards);
        }

        public string Print() {
            string output = "";
            foreach(Card card in mCards) {
                output += card.Print() + "\n";
            }
            return output;
        }

        public void Shuffle() {
            Random rnd = new Random();
            mCards = mCards.OrderBy(item => rnd.Next()).ToList();
        }
    }
}