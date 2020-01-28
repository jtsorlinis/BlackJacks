package javablackjack;

public class Main {
    public static void main(String[] args) throws Exception {
        int numOfPlayers = 5;
            int numOfDecks = 8;
            int betSize = 10;
            int minCards = 40;

            int rounds = 100000;
            int verbosity = 0;

            Table table1 = new Table(numOfPlayers, numOfDecks, betSize, minCards, verbosity);
            table1.mCardPile.shuffle();
            
            long start = System.nanoTime();

            int x = 0;
            while(x++ < rounds) {
                if(verbosity > 0) {
                    System.out.println("Round " + x);
                }
                if(verbosity == 0 && rounds > 1000 && x % (rounds/100) == 0) {
                    System.out.print("Progress: " + (int)(((float)x/rounds)*100) + "\r");
                }

                table1.startRound();
                table1.checkEarnings();
            }

            table1.clear();

            for(Player player : table1.mPlayers) {
                System.out.println("Player " + player.mPlayerNum + " earnings: " + player.mEarnings + "\t\tWin Percentage: " + (50 + (player.mEarnings/(rounds*betSize)*50f)) + "%");
            }
            System.out.println("Casino earnings: " + table1.mCasinoEarnings);
            float duration = (System.nanoTime() - start)/(float)1000000000;
            System.out.println("Played " + rounds + " rounds in " + duration + " seconds");
    }
}