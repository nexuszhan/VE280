#include "deck.h"
#include <iostream>

using namespace std;

int main()
{
    Deck d;
    cout << d.cardsLeft() << endl;
    d.shuffle(20);
    return 0;
}