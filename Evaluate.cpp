#include "Checkers.h"

int PST_man[45] ={0,0,0,0,0,

        0,    0,    0,    0,
    25,   50,   50,   50,        0,
       25,   40,   50,   20,
    15,   30,   20,   20,        0,
       15,   25,   25,   10,
     5,   10,   10,   10,        0,
        5,    5,    5,    0,
     0,    5,   15,    5,
	 
0,0,0,0,0
};

int PST_king[45] = {0,0,0,0,0,

       20,    0,   10,   30,
    20,   20,   10,   30,        0,
       20,   20,   30,   10,
     0,   20,   30,   10,        0,
       10,   30,   20,    0,
    10,   30,   20,   20,        0,
       30,   10,   20,   20,
    30,   10,    0,   20,
	
0,0,0,0,0
};

int Eval()
{
	int score = 0;
	for (unsigned sq = 5; sq < 40; ++sq)
	{
		if (Board[sq] & WHITE)
		{
			if (Board[sq] & KING)
			{
				score += KING_VALUE;
				score += PST_king[sq];
			}
			else
			{
				score += MAN_VALUE;
				score += PST_man[sq];
			}
		}
		else if (Board[sq] & BLACK)
		{
			if (Board[sq] & KING)
			{
				score -= KING_VALUE;
				score -= PST_king[44 - sq];
			}
			else
			{
				score -= MAN_VALUE;
				score -= PST_man[44 - sq];
			}
		}
	}

	if (stm == BLACK) return -score;
	return score;
}
