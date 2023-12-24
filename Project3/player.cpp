#include "player.h"

class SimplePlayer : public Player
{
    public:
        int bet(unsigned int bankroll,unsigned int minimum)  {return minimum;};

        bool draw(Card dealer,const Hand & player)
        {
            HandValue cur = player.handValue();
            int val = cur.count;

            if (cur.soft)
            {
                if (val<=17)
                    return true;
                else if (val==18)
                {
                    if (dealer.spot!=TWO && dealer.spot!=SEVEN && dealer.spot!=EIGHT)
                        return true;
                }
            }
            else
            {
                if (val<=11)
                    return true;
                else if (val==12)
                {
                    if (dealer.spot<FOUR || dealer.spot>SIX)
                        return true;
                }
                else if (val<=16)
                {
                    if (dealer.spot>=SEVEN)
                        return true;
                }
            }
            return false;
        };

        void expose(Card c) {};

        void shuffled() {};
};

class CountingPlayer : public SimplePlayer
{
    private:
        int cnt;
    public:
        int bet(unsigned int bankroll,unsigned int minimum)
        {
            if (cnt>=2 && bankroll>=2*minimum)
                return 2*minimum;
            else
                return minimum;
        };

        void expose(Card c)
        {
            if (c.spot>=TEN)
                cnt--;
            else if (c.spot<=SIX)
                cnt++;
        };

        void shuffled()  {cnt = 0;};
};

static SimplePlayer sp;
static CountingPlayer cp;

extern Player* get_Simple()  {return &sp;}

extern Player* get_Counting()  {return &cp;}