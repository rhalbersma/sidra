#include <string.h>

#include "Checkers.h"

unsigned Map_45_to_64[] =
{
	 0,  0,  0,  0,  0,
	 1,  3,  5,  7,
	 8, 10, 12, 14,  0,
	17, 19, 21, 23,
	24, 26, 28, 30,  0,
	33, 35, 37, 39,
	40, 42, 44, 46,  0,
	49, 51, 53, 55,
	56, 58, 60, 62
};

void SquareToStr(unsigned sq, char *s)
{
	sq = Map_45_to_64[sq];
	s[0] = sq % 8 + 'a';
	s[1] = 8 - sq / 8 + '0';
	s[2] = 0;
}

void MoveToStr(Move *m, char *s)
{
	unsigned i;
	SquareToStr(m->from, s);
	for (i = 0; m->cap_sq[i]; i++)
	{
		s[i * 3 + 2] = ':';
		SquareToStr(m->cap_sq[i], s + i * 3 + 3);
	}
	if (m->cap_sq[0])
	{
		s[i * 3 + 2] = ':';
		SquareToStr(m->to, s + i * 3 + 3);
	} else SquareToStr(m->to, s + 2);
}

unsigned StrToSquare(char *sq)
{
	return Map_64_to_45[sq[0] - 'a' + 8 * ('8' - sq[1])];
}

void StrToMove(char *s, Move *m)
{
	char str_move[128];
	MP = MoveBuffer;
	GenerateAllMoves();
	for (Move *pm = MoveBuffer; pm != MP; pm++)
	{
		MoveToStr(pm, str_move);
		if (!strcmp(str_move, s))
		{
			*m = *pm;
			MP = MoveBuffer;
			return;
		}
	}
	MP = MoveBuffer;
	m->from = 0;
}
