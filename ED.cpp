#include "checkers.h"
#include "EdAccess.h"

EdAccess *ED = 0; // интерфейс для доступа к эндшпильным базам
unsigned EdPieces = 0; // количество шашек в базе
bool EdNocaptures = false; // в базе нет позиций со взятиями

int EdProbe()
{
	if (!ED) return EdAccess::not_found;

	unsigned i;
	EdAccess::EdBoard1 b;

	if (stm == WHITE)
	{
		for (i = 0; i < 32; i++)
		{
			switch (Board[Map_32_to_45[i]])
			{
				case 0           : b.board[i] = EdAccess::empty; break;
				case WHITE       : b.board[i] = EdAccess::white; break;
				case BLACK       : b.board[i] = EdAccess::black; break;
				case WHITE | KING: b.board[i] = EdAccess::white | EdAccess::king; break;
				case BLACK | KING: b.board[i] = EdAccess::black | EdAccess::king; break;
			}		
		}
	}
	else
	{
		// при ходе черных "переворачиваем" доску
		for (i = 0; i < 32; i++)
		{
			switch (Board[Map_32_to_45[31 - i]])
			{
				case 0           : b.board[i] = EdAccess::empty; break;
				case WHITE       : b.board[i] = EdAccess::black; break;
				case BLACK       : b.board[i] = EdAccess::white; break;
				case WHITE | KING: b.board[i] = EdAccess::black | EdAccess::king; break;
				case BLACK | KING: b.board[i] = EdAccess::white | EdAccess::king; break;
			}		
		}
	}

	return ED->GetResult(&b, 0);
}
