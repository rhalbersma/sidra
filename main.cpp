#include <windows.h>
#include <fstream>
using namespace std;

#include "checkers.h"
#include "EdAccess.h"

// функция обратного вызова для отображения информации о ходе вычислений
// score - оценка позиции.
//         выигрыш лучше обозначать так: 32767 - N, где N это количество полуходов до выигрыша
//         выигрыш по безранговой базе, лучше обозначать так: 30000 - N
// depth - информация о глубине перебора
// pv - лучший вариант
// cm - ход анализируемый в данный момент
typedef void (__stdcall *PF_SearchInfo)(int score, int depth, int speed, char *pv, char *cm);
PF_SearchInfo pfSearchInfo = 0;

// второй вариант
// все параметры строковые
// для улучшенного отображения информации о переборе
typedef void (__stdcall *PF_SearchInfoEx)(char *score, char *depth, char *speed, char **pv, char *cv);
PF_SearchInfoEx pfSearchInfoEx = 0;

// Сделать ход move
// Формат ходов: "a3b4" и "a3:b4:d6:e7". Такой формат позволяет устранить все неоднозначности при взятиях
__declspec(dllexport) void __stdcall EI_MakeMove(char *move)
{

	Move m;
	StrToMove(move, &m);
	if (!m.from) MessageBox(0, "SiDra: move not found", move, MB_OK);
	else MakeMove(&m, 0);
}

// Начать вычисления. После вернуть лучший ход
// Эта функция может выполняться как угодно долго
// Но надо иметь в виду количество оставшегося времени (см. EI_SetTimeControl и EI_SetTime)
__declspec(dllexport) char * __stdcall EI_Think()
{
	static char move_buf[128];
	Move m = RootSearch();
	MakeMove(&m, 0);
	MoveToStr(&m, move_buf);
	return move_buf;
}

// Здесь можно делать что угодно и как угодно долго
// Эта функция вызывается в момент когда противник думает над своим ходом
__declspec(dllexport) void __stdcall EI_Ponder()
{
	// здесь можно ничего и не делать :)
	return;
}

// Противник делает ход move
// Перед этим вызывалась функция Ponder
// Здесь сразу можно вернуть ход на основе вычиcлений сделанных в Ponder
// Можно подумать еще и только после этого вернуть ход
__declspec(dllexport) char * __stdcall EI_PonderHit(char *move)
{
	EI_MakeMove(move);
	return EI_Think();
}

// Инициализация движка
// si - см. выше описание PF_SearchInfo
// mem_lim - лимит памяти, которую может использовать движок
// здесь в основном имеется ввиду размер хэш-таблицы
__declspec(dllexport) void __stdcall EI_Initialization(PF_SearchInfo si, int mem_lim)
{
	pfSearchInfo = si;
}


// установка указателя на улучшенную функцию вывода инормации о переборе
__declspec(dllexport) void __stdcall EI_SetSearchInfoEx(PF_SearchInfoEx sie)
{
	pfSearchInfoEx = sie;
}

__declspec(dllexport) void __stdcall EI_NewGame()
{
	NewGame();
}

// Закончить вычисления и выйти из функций EI_Think, EI_Ponder, EI_PonderHit или EI_Analyse
__declspec(dllexport) void __stdcall EI_Stop()
{
	StopRequest = true;
}

// Установить позицию pos на доске
// например, начальная позиция bbbbbbbbbbbb........wwwwwwwwwwwww
// b - простая черная
// B - черная дамка
// w - простая белая
// W - белая дамка
// . - пустое поле
// поля перечисляются так: b8, d8, f8, h8, a7, c7, ..., a1, c1, e1, g1
// последний символ определяет очередность хода
// w - белые, b - черные
__declspec(dllexport) void __stdcall EI_SetupBoard(char *pos)
{
	SetupBoard(pos);
}

// Установить контроль времени
// time минут на партию
// inc миллисекунд - бонус за каждый сделанный ход (часы Фишера)
__declspec(dllexport) void __stdcall EI_SetTimeControl(int time, int inc)
{
	SetTimeControl(time, inc);
}
	
// Установить время в миллисекундах оставшееся на часах
// time - свое время
// otime - время противника
// вызывается перед каждым ходом
__declspec(dllexport) void __stdcall EI_SetTime(int time, int otime)
{
	SetTime(time, otime);
}

// Вернуть название движка
__declspec(dllexport) char * __stdcall EI_GetName()
{
	return "SiDra 2";
}

// Вызывается перед выгрузкой движка
__declspec(dllexport) void __stdcall EI_OnExit()
{
}

// Анализировать текущую позицию
// Выход из режима анализа осуществляется при получении команды Stop
__declspec(dllexport) void __stdcall EI_Analyse()
{
	AnalyseMode = true;
	RootSearch();
	AnalyseMode = false;
}

// функция интерфейса экспортируемая из dll
__declspec(dllexport) void __stdcall EI_EGDB(EdAccess *eda)
{
	ED = eda;
	if (ED)
	{
		EdPieces = ED->Load("russian");
		if (strstr(ED->GetBaseType(), "nocaptures"))
		{
			EdNocaptures = true;
		}
	}
}

// Если есть какие-нибудь предложения по поводу улучшения интерфейса, то буду рад их обсудить
// Пишите письма igorkorshunov@yandex.ru или korshunov@gsu.unibel.by
