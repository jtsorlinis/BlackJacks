using System;
using System.Collections.Generic;
using System.Linq;

namespace NETCoreBlackJack {
    class Deck {
        public List<Card> mCards = new List<Card>();
        private List<string> mRanks = new List<string> { "A", "2", "3", "4", "5", "6", "7", "8","9","10","J","Q","K" };
        private List<string> mSuits = new List<string> { "Clubs", "Hearts", "Spades", "Diamonds" };

        public Deck() {
            foreach (string suit in mSuits) {
                foreach(string rank in mRanks) {
                    mCards.Add(new Card(rank,suit));
                }
            }
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