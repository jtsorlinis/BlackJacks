package javablackjack;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

class CardPile {
     List<Card> mCards = new ArrayList<>();
     List<Card> mOriginalCards = new ArrayList<>();

     CardPile(int numOfDecks) {
        for (int i = 0; i < numOfDecks; i++) {
            Deck temp = new Deck();
            mCards.addAll(temp.mCards);
        }
        mOriginalCards = new ArrayList<>(mCards);
    }

     void refresh() {
        mCards = new ArrayList<>(mOriginalCards);
    }

     String print() {
        String output = "";
            for(Card card : mCards) {
                output += card.print() + "\n";
            }
            return output;
    }

     void shuffle() {
        // TODO: Speed this up
        Collections.shuffle(mCards);
    }
}