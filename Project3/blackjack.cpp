#include "player.h"
#include "deck.h"
#include "rand.h"
#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

void shuffleDeck(Deck & D,Player* P)
{
    // Choose seven cuts to shuffle
    cout << "Shuffling the deck\n";
    for (int i=0; i<7; i++)
    {
        int cut = get_cut();
        D.shuffle(cut);
        cout << "cut at " << cut << endl;
    }
    P->shuffled();
}

void dealCard(Deck & D,Player* P,Hand & H,string participant)
{
    // Deal a card for either player or dealer
    Card card=D.deal();
    H.addCard(card);
    cout << participant <<  " dealt " << SpotNames[card.spot] << " of " << SuitNames[card.suit] << endl;
    P->expose(card);
}

int main(int argc,char* argv[])
{
    const int minimum_bet = 5; // Minumum bet value.
    // Initialize
    int bankroll=atoi(argv[1]);
    int hands=atoi(argv[2]);
    string player_type(argv[3]);
    Player* player;
    if (player_type=="simple")
        player=get_Simple();
    else
        player=get_Counting();
    Deck d;
    shuffleDeck(d,player);

    int this_hand = 1;
    while (bankroll>=minimum_bet && this_hand<=hands)
    {
        cout << "Hand " << this_hand << " bankroll " << bankroll << endl;
        if (d.cardsLeft()<20)
            shuffleDeck(d,player);
        int wager=player->bet(bankroll,minimum_bet);
        cout << "Player bets " << wager << endl;

        Hand player_hand; 
        dealCard(d,player,player_hand,"Player");

        Card card2=d.deal();
        player->expose(card2);
        cout << "Dealer dealt " << SpotNames[card2.spot] << " of " << SuitNames[card2.suit] << endl;

        dealCard(d,player,player_hand,"Player");
    
        Card hole_card=d.deal();
        
        if (player_hand.handValue().count==21)
        {   // Player wins with natural 21
            cout << "Player dealt natural 21\n";
            bankroll += wager*3/2;
            this_hand++;
            continue;
        }
        else
        {
            while (player->draw(card2,player_hand))
                dealCard(d,player,player_hand,"Player");
        }
        cout << "Player's total is " << player_hand.handValue().count << endl;
        if (player_hand.handValue().count>21) 
        {   // Player busts!
            cout << "Player busts\n";
            bankroll -= wager;
            this_hand++;
            continue;
        }
        else
        {
            Hand dealer_hand;
            cout << "Dealer's hole card is " << SpotNames[hole_card.spot] << " of " << SuitNames[hole_card.suit] << endl;
            player->expose(hole_card);
            dealer_hand.addCard(card2); dealer_hand.addCard(hole_card);

            while (dealer_hand.handValue().count<17)
                dealCard(d,player,dealer_hand,"Dealer");

            cout << "Dealer's total is " << dealer_hand.handValue().count << endl;
            if (dealer_hand.handValue().count>21)
            {   // Dealer busts!
                cout << "Dealer busts\n";
                bankroll += wager;
                this_hand++;
                continue;
            }
            else
            {   // Compare two total values and judge the result.
                if (player_hand.handValue().count > dealer_hand.handValue().count)
                {
                    cout << "Player wins\n";
                    bankroll += wager;
                }
                else if (player_hand.handValue().count < dealer_hand.handValue().count)
                {
                    cout << "Dealer wins\n";
                    bankroll -= wager;
                }
                else
                    cout << "Push\n";
                this_hand++;
            }
        }
    }
    cout << "Player has " << bankroll << " after " << --this_hand << " hands\n";

    return 0;
}