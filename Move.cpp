#include "Checkers.h"

void MakeMove(Move *m, unsigned ply)
{
	unsigned i;

	Nodes++;

	Reversible[ply] = (Board[m->from] & KING) && !m->cap_sq[0];

	// перемещаем шашку
	if (m->promotion) Board[m->to] = Board[m->from] | KING;
	else Board[m->to] = Board[m->from];
	if (m->from != m->to) Board[m->from] = 0;

	// снимаем сбитые
	for (i = 0; m->cap_sq[i]; i++)
	{
		Board[m->cap_sq[i]] = 0;
	}
	Pieces -= i;

	stm ^= CHANGE_COLOR;
}

void UnmakeMove(Move *m)
{
	unsigned i;

	stm ^= CHANGE_COLOR;

	// восстанавливаем сбитые
	for (i = 0; m->cap_sq[i]; i++)
	{
		Board[m->cap_sq[i]] = m->cap_type[i];
	}
	Pieces += i;

	// перемещаем шашку
	if (m->promotion) Board[m->from] = stm;
	else Board[m->from] = Board[m->to];
	if (m->from != m->to) Board[m->to] = 0;
}
