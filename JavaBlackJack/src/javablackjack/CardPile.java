package javablackjack;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.ThreadLocalRandom;

class CardPile {
     List<Card> mCards = new ArrayList<>();
     List<Card> mOriginalCards = new ArrayList<>();
     ThreadLocalRandom rnd = ThreadLocalRandom.current();

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
        for(int i = mCards.size()-1; i > 0; i--) {
            int j = rnd.nextInt(i+1);
            Collections.swap(mCards,i,j);
        }
    }
}