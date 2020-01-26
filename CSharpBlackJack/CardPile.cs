using System;
using System.Collections.Generic;
using System.Linq;

namespace NETCoreBlackJack {
    class CardPile {

        public List<Card> mCards = new List<Card>();
        public List<Card> mOriginalCards = new List<Card>();
        Random rnd = new Random();

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
            
            //mCards = mCards.OrderBy(item => rnd.Next()).ToList();

            // Fisher Yates
            for (int i = mCards.Count - 1; i > 0; i--)
            {
                int j = rnd.Next() % (i + 1);
                Card temp = mCards[i];
                mCards[i] = mCards[j];
                mCards[j] = temp;
            }

        }
    }
}