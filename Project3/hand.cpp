#include "hand.h"

Hand::Hand()
{
    curValue.count = 0;
    curValue.soft = false;
}

void Hand::discardAll()
{
    curValue.count = 0;
    curValue.soft = false;
}

void Hand::addCard(Card c)
{
    if (c.spot==ACE)
    {
        // At most one ACE can be 11.
        if ((11+curValue.count)>21)
            curValue.count += 1;
        else
        {
            curValue.count += 11;
            curValue.soft = true;
        }
    }
    else if (c.spot>=TEN)
    {
        if (curValue.soft)
        {
            // When soft is true, check whether the value exceeds 21.
            // If so, the ACE should change to 1 and soft to false.
            if ((curValue.count+10)>21)
                curValue.soft = false;
            else
                curValue.count += 10;
        }
        else
            curValue.count += 10;
    }
    else
    {
        if (curValue.soft)
        {
            // Similar to the case above.
            if ((curValue.count+c.spot+2)>21)
            {
                curValue.count += (c.spot-8);
                curValue.soft = false;
            }
            else
                curValue.count += (c.spot+2);
        }
        else
            curValue.count += (c.spot+2);
    }
}

HandValue Hand::handValue() const
{
    return curValue;
}