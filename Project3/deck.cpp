#include "deck.h"
#include <iostream>

Suit getSuit(int x)
{
    // REQUIRES: x=1 or 2 or 3 or 4.
    // EFFECTS: return the suit corresponding to x.
    if (x==0)
        return SPADES;
    else if (x==1)
        return HEARTS;
    else if (x==2)
        return CLUBS;
    else
        return DIAMONDS;
}

Spot getSpot(int x)
{
    // REQUIRES: x=0~12.
    // EFFECTS: return the spot corresponding to x.
    if (x==0)
        return TWO;
    else if (x==1)
        return THREE;
    else if (x==2)
        return FOUR;
    else if (x==3)
        return FIVE;
    else if (x==4)
        return SIX;
    else if (x==5)
        return SEVEN;
    else if (x==6)
        return EIGHT;
    else if (x==7)
        return NINE;
    else if (x==8)
        return TEN;
    else if (x==9)
        return JACK;
    else if (x==10)
        return QUEEN;
    else if (x==11)
        return KING;
    else
        return ACE;
}

Deck::Deck()
{
    for (int i=0; i<DeckSize; i++)
    {
        deck[i].spot = getSpot(i%(ACE+1));
        deck[i].suit = getSuit(i/(ACE+1));
    }
    next = 0;
}

void Deck::reset()
{
    for (int i=0; i<DeckSize; i++)
    {
        deck[i].spot = getSpot(i%(ACE+1));
        deck[i].suit = getSuit(i/(ACE+1));
    }
    next = 0;
}

void Deck::shuffle(int n)
{
    int temp = std::min(n,DeckSize-n);
    Card store[DeckSize];
    for (int i=0; i<temp; i++)
    {
        store[2*i] = deck[n+i];
        store[2*i+1] = deck[i];
    }
    // Assign the rest cards
    for (int i=temp*2; i<DeckSize; i++)
    {
        if (n>(DeckSize-n))
            store[i] = deck[i-temp];
        else
            store[i] = deck[i];
    }
    for (int i=0; i<DeckSize; i++)
        deck[i] = store[i];
    
    next = 0;
}

Card Deck::deal()
{
    if (cardsLeft()==0)
        throw DeckEmpty();
    else
        return deck[next++];
}

int Deck::cardsLeft()
{
    return (DeckSize-next);
}