package javablackjack;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

class CardPile {
    public List<Card> mCards = new ArrayList<>();
    public List<Card> mOriginalCards = new ArrayList<>();

    public CardPile(int numOfDecks) {
        for (int i = 0; i < numOfDecks; i++) {
            Deck temp = new Deck();
            mCards.addAll(temp.mCards);
        }
        mOriginalCards = new ArrayList<>(mCards);
    }

    public void refresh() {
        mCards = new ArrayList<>(mOriginalCards);
    }

    public String print() {
        String output = "";
            for(Card card : mCards) {
                output += card.print() + "\n";
            }
            return output;
    }

    public void shuffle() {
        // TODO: Speed this up
        Collections.shuffle(mCards);
    }
}