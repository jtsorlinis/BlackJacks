package javablackjack;

public class Main {
    public static void main(String[] args) throws Exception {
        CardPile deck1 = new CardPile(8);
        deck1.shuffle();
        System.out.println(deck1.print());
    }
}