package javablackjack;

import java.util.ArrayList;
import java.util.List;

class Deck {
    public List<Card> mCards = new ArrayList<>();
    String[] mRanks = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
    String[] mSuits = { "Clubs", "Hearts", "Spades", "Diamonds" };

    public Deck() {
        for (String suit : mSuits) {
            for (String rank : mRanks) {
                mCards.add(new Card(rank, suit));
            }
        }
    }

    public String print() {
        String output = "";
        for (Card card : mCards) {
            output += card.print() + "\n";
        }
        return output;
    }

}