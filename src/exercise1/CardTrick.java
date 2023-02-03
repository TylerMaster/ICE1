package exercise1;

/**
 * A class that fills a hand of 7 cards with random Card Objects and then asks the user to pick a card.
 * It then searches the array of cards for the match to the user's card. 
 * To be used as starting code in Exercise
 *
 * @author dancye
 * @author Paul Bonenfant Jan 25, 2022 
 */
import java.util.Random;
public class CardTrick {
    
    public static void main(String[] args) {
        
        Card[] hand = new Card[7];
        
        Random random = new Random();

        for (int i = 0; i < hand.length; i++) {
            int value = random.nextInt(13) + 1;
            String suit = Card.SUITS[random.nextInt(4)];
            Card card = new Card();
            hand[i] = card;
         
        }

       
        
    }
    

    /**
     * A simple method to print out personal information. Follow the instructions to 
     * replace this information with your own.
     * @author Paul Bonenfant Jan 2022
     */
    private static void printInfo() {
    // Im done
        System.out.println("Congratulations, you guessed right!");
        System.out.println();
        
        System.out.println("My name is Brandon Smith");
        System.out.println();
        
        System.out.println("My career ambitions:");
        System.out.println("-- Artist");
        System.out.println("-- Programmer!");
	System.out.println();	

        System.out.println("My hobbies:");
        System.out.println("-- fitness");
        System.out.println("-- art");
        System.out.println("-- video games");
        System.out.println("-- school");

        System.out.println();
        
    
    }

}
