#include "Checkers.h"

inline void AddManMove(unsigned sq, int dir)
{
	if (Board[sq + dir] == 0)
	{
		MP->from = sq;
		MP->to = sq + dir;
		MP->cap_sq[0] = 0;
		MP->promotion = MP->to < 9 || MP->to > 35;
		MP++;
	}
}

inline void AddKingMoves(int sq, int dir)
{
	for (unsigned to = sq + dir; Board[to] == 0; to += dir)
	{
		MP->from = sq;
		MP->to = to;
		MP->cap_sq[0] = 0;
		MP->promotion = false;
		MP++;
	}
}

void GenerateSilentMoves()
{
	for (unsigned sq = 5; sq < 40; sq++)
	{
		if (Board[sq] & stm)
		{
			if (Board[sq] & KING)
			{
				AddKingMoves(sq, -4);
				AddKingMoves(sq, -5);
				AddKingMoves(sq, 4);
				AddKingMoves(sq, 5);
			}
			else
			{
				static const int ForwardDirections[2][2] = {{-4, -5}, {4, 5}};
				AddManMove(sq, ForwardDirections[stm - 1][0]);
				AddManMove(sq, ForwardDirections[stm - 1][1]);
			}
		}
	}
}

inline void CopyMoveFromTemplate(Move *t, unsigned caps)
{
	unsigned i;
	MP->from = t->from;
	MP->promotion = t->promotion;
	for (i = 0; i < caps; i++)
	{
		MP->cap_sq[i] = t->cap_sq[i];
		MP->cap_type[i] = t->cap_type[i];
	}
	MP->cap_sq[i] = 0;
}

inline void AddCaptured(unsigned sq, Move *m, unsigned caps)
{
	m->cap_sq[caps] = sq;
	m->cap_type[caps] = Board[sq];
}

void AddKingCaptures(unsigned sq, Move *t, unsigned caps, int dir);

inline void KingCapture(unsigned sq, Move *t, unsigned caps, int dir, int bad_dir, bool &found)
{
	if (dir == bad_dir || dir == -bad_dir) return;
	int m = sq + dir;
	while (!Board[m]) m += dir;
	if (!OP_CHECKER(m)) return;
	int to = m + dir;
	if (Board[to]) return;
	AddCaptured(m, t, caps);
	Board[m] ^= CHANGE_COLOR;
	AddKingCaptures(to, t, caps + 1, dir);
	Board[m] ^= CHANGE_COLOR;
	found = true;
}

void AddKingCaptures(unsigned sq, Move *t, unsigned caps, int dir)
{
	bool found = false;
	unsigned m = sq;
	while (!Board[m])
	{
		KingCapture(m, t, caps, -4, dir, found);
		KingCapture(m, t, caps, -5, dir, found);
		KingCapture(m, t, caps,  4, dir, found);
		KingCapture(m, t, caps,  5, dir, found);
		m += dir;
	}

	unsigned to = m + dir;
	if (OP_CHECKER(m) && !Board[to])
	{
		AddCaptured(m, t, caps);
		Board[m] ^= CHANGE_COLOR;
		AddKingCaptures(to, t, caps + 1, dir);
		Board[m] ^= CHANGE_COLOR;
		return;
	}

	if (!found)
	{
		do {
			CopyMoveFromTemplate(t, caps);
			MP->to = sq;
			++MP;
			sq += dir;
		} while (!Board[sq]);
	}
}

void AddPromoCaptures(unsigned sq, Move *t, unsigned caps, int dir)
{
	if (dir == -4)      dir = 5;
	else if (dir == -5) dir = 4;
	else if (dir == 4)  dir = -5;
	else                dir = -4;

	unsigned m = sq + dir;
	while (!Board[m]) m += dir;
	unsigned to = m + dir;
	if (OP_CHECKER(m) && !Board[to])
	{
		AddCaptured(m, t, caps);
		Board[m] ^= CHANGE_COLOR;
		AddKingCaptures(to, t, caps + 1, dir);
		Board[m] ^= CHANGE_COLOR;
		return;
	}
	CopyMoveFromTemplate(t, caps);
	MP->to = sq;
	MP++;
}

void AddManCaptures(unsigned sq, Move *t, unsigned caps, int bad_dir);

inline void ManCapture(unsigned sq, Move *t, unsigned caps, int dir, int bad_dir, bool &found)
{
	if (dir == bad_dir) return;

	unsigned m = sq + dir;
	if (!OP_CHECKER(m)) return;

	unsigned to = m + dir;
	if (Board[to]) return;

	AddCaptured(m, t, caps);
	Board[m] ^= CHANGE_COLOR;

	if (to < 9 && stm == WHITE || to > 35 && stm == BLACK)
	{
		t->promotion = true;
		AddPromoCaptures(to, t, caps + 1, dir);
		t->promotion = false;
	}
	else
	{
		t->promotion = false;
		AddManCaptures(to, t, caps + 1, -dir);
	}

	Board[m] ^= CHANGE_COLOR;
	found = true;
}

void AddManCaptures(unsigned sq, Move *t, unsigned caps, int bad_dir)
{
	bool found = false;
	ManCapture(sq, t, caps, -4, bad_dir, found);
	ManCapture(sq, t, caps, -5, bad_dir, found);
	ManCapture(sq, t, caps,  4, bad_dir, found);
	ManCapture(sq, t, caps,  5, bad_dir, found);
	if (!found)
	{
		CopyMoveFromTemplate(t, caps);
		MP->to = sq;
		MP++;
	}
}

inline void TryKingCapture(unsigned sq, int dir)
{
	unsigned m = sq + dir;
	while (!Board[m]) m += dir;
	if (!OP_CHECKER(m)) return;

	unsigned to = m + dir;
	if (Board[to]) return;

	unsigned save = Board[sq];
	Board[sq] = 0;
	Move t;
	t.from = sq;
	t.promotion = false;
	AddCaptured(m, &t, 0);
	Board[m] ^= CHANGE_COLOR;
	AddKingCaptures(to, &t, 1, dir);
	Board[m] ^= CHANGE_COLOR;
	Board[sq] = save;
}

inline void TryManCapture(unsigned sq, int dir)
{
	unsigned m = sq + dir;
	if (!OP_CHECKER(m)) return;

	unsigned to = m + dir;
	if (Board[to]) return;

	unsigned save = Board[sq];
	Board[sq] = 0;
	Move t;
	t.from = sq;
	AddCaptured(m, &t, 0);
	Board[m] ^= CHANGE_COLOR;
	if (to < 9 && stm == WHITE || to > 35 && stm == BLACK)
	{
		t.promotion = true;
		AddPromoCaptures(to, &t, 1, dir);
	}
	else
	{
		t.promotion = false;
		AddManCaptures(to, &t, 1, -dir);
	}
	Board[m] ^= CHANGE_COLOR;
	Board[sq] = save;
}

void GenerateCaptures()
{
	for (unsigned sq = 5; sq < 40; ++sq)
	{
		if (Board[sq] & stm)
		{
			if (Board[sq] & KING)
			{
				TryKingCapture(sq, -4);
				TryKingCapture(sq, -5);
				TryKingCapture(sq, 4);
				TryKingCapture(sq, 5);
			}
			else
			{
				TryManCapture(sq, -4);
				TryManCapture(sq, -5);
				TryManCapture(sq, 4);
				TryManCapture(sq, 5);
			}
		}
	}
}

void GenerateAllMoves()
{
	Move *old_MP = MP;
	GenerateCaptures();
	if (MP == old_MP) GenerateSilentMoves();
}
