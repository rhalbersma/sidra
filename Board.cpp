#include <windows.h>
#include <iostream>
using namespace std;

#include "Checkers.h"

/*

	45-клеточное представление доски (с буферными полями)
	реальные поля имеют индексы:

              5   6   7   8
            9  10  11  12  
             14  15  16  17
           18  19  20  21  
             23  24  25  26
           27  28  29  30  
             32  33  34  35
           36  37  38  39  


"b8" - 5, "d8" - 6, ..., "e1" - 38, "g1" - 39
*/

unsigned Board[45]; // доска
int stm; // очередность хода (side to move)
unsigned Pieces; // общее количество шашек на доске

Move MoveBuffer[MOVE_BUFFER_LEN]; // буфер ходов - сюда будем генерить ходы
Move *MP; // указатель для доступа к буферу ходов

Move PV[MAX_DEPTH][MAX_DEPTH]; // лучший вариант (principal variation)

double Nodes; // количество просмотренных позиций
bool Reversible[MAX_DEPTH]; // ход на уровне ply был обратимым, т.е. дамочным без взятия


// для отображения доски из 45-клеточной (с буферными полями) в 32-клеточную (только поля реальной доски)
unsigned Map_32_to_45[32] =
{
	5, 6, 7, 8, 9, 10, 11, 12,
	14, 15, 16, 17, 18, 19, 20, 21,
	23, 24, 25, 26, 27, 28, 29, 30,
	32, 33, 34, 35, 36, 37, 38, 39
};

// расстановка заданной позиции
void SetupBoard(char *p)
{
	unsigned i;

	Pieces = 0;

	// делаем все поля пустыми
	for (i = 0; i < 45; i++) Board[i] = 0;

	// помечаем буферные поля
	Board[ 0] = OFF;
	Board[ 1] = OFF;
	Board[ 2] = OFF;
	Board[ 3] = OFF;
	Board[ 4] = OFF;
	Board[13] = OFF;
	Board[22] = OFF;
	Board[31] = OFF;
	Board[40] = OFF;
	Board[41] = OFF;
	Board[42] = OFF;
	Board[43] = OFF;
	Board[44] = OFF;

	// парсим строку и ставим шашки куда надо
	for (i = 0; i < 32; i++)
	{
		switch (p[i])
		{
			case 'w': Board[Map_32_to_45[i]] = WHITE_MAN;  Pieces++; break;
			case 'W': Board[Map_32_to_45[i]] = WHITE_KING; Pieces++; break;
			case 'b': Board[Map_32_to_45[i]] = BLACK_MAN;  Pieces++; break;
			case 'B': Board[Map_32_to_45[i]] = BLACK_KING; Pieces++; break;
			case '.': Board[Map_32_to_45[i]] = 0; break;

			 // неправильный символ в описании позиции
			default:  MessageBox(0, p, "Unknown symbol in position string", 0); return;
		}
	}

	// устанавливаем очередность хода
	if (p[32] == 'w' || p[32] == 'W') stm = WHITE;
	else stm = BLACK;
}

// новая игра
void NewGame()
{
	SetupBoard("bbbbbbbbbbbb........wwwwwwwwwwwww");
}

unsigned Map_64_to_45[64] =
{
	 0,  5,  0,  6,  0,  7,  0,  8,
	 9,  0, 10,  0, 11,  0, 12,  0,
	 0, 14,  0, 15,  0, 16,  0, 17,
	18,  0, 19,  0, 20,  0, 21,  0,
	 0, 23,  0, 24,  0, 25,  0, 26,
	27,  0, 28,  0, 29,  0, 30,  0,
	 0, 32,  0, 33,  0, 34,  0, 35,
	36,  0, 37,  0, 38,  0, 39,  0,
};

// вывод доски на консоль
void PrintBoard()
{
	cout << endl;
	for (unsigned y = 0; y < 8; y++)
	{
		cout << "\t\t";
		for (unsigned x = 0; x < 8; x++)
		{
			unsigned sq = Map_64_to_45[x + y * 8];
			if (sq)
			{
				if (Board[sq] & WHITE)
				{
					if (Board[sq] & KING) cout << 'W';
					else cout << 'w';
				}
				else if (Board[sq] & BLACK)
				{
					if (Board[sq] & KING) cout << 'B';
					else cout << 'b';
				}
				else if (!Board[sq]) cout << '.';
			}
			else cout << ' ';
			cout << ' ';
		}
		cout << endl;
	}
	cout << endl;
	cout.flush();
}
