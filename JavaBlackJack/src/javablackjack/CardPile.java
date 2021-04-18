package javablackjack;

import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

class CardPile {
    List<Card> mCards = new ArrayList<>();
    List<Card> mOriginalCards = new ArrayList<>();
    long state = System.currentTimeMillis();

    // From https://www.pcg-random.org/download.html#minimal-c-implementation
    int pcg32() {
        long oldState = state;
        state = oldState * 6364136223846793005L + 1;
        int xorShifted = (int) (((oldState >>> 18) ^ oldState) >>> 27);
        int rot = (int) (oldState >>> 59);
        return Integer.rotateRight(xorShifted, rot);
    }

    // use nearly divisionless technique found here
    // https://github.com/lemire/FastShuffleExperiments
    int pcg32_range(int s) {
        int x = pcg32();
        long m = Integer.toUnsignedLong(x) * Integer.toUnsignedLong(s);
        int l = (int) m;

        if (Integer.compareUnsigned(l, s) < 0) {
            int t = Integer.remainderUnsigned(-s, s);
            while (Integer.compareUnsigned(l, t) < 0) {
                x = pcg32();
                m = Integer.toUnsignedLong(x) * Integer.toUnsignedLong(s);
                l = (int) m;
            }
        }
        return (int) (m >> 32);
    }

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
        for (Card card : mCards) {
            output += card.print() + "\n";
        }
        return output;
    }

    void shuffle() {
        for (int i = mCards.size() - 1; i > 0; i--) {
            int j = pcg32_range(i + 1);
            Collections.swap(mCards, i, j);
        }
    }
}