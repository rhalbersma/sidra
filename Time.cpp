#include <windows.h>

int StartTime;
int TimeRemaining = 300000;
int BaseTime;
int IncTime;
int TimeForMove;

void StartTimer()
{
	StartTime = GetTickCount();
	TimeForMove = TimeRemaining / 20 + IncTime;
	if (TimeForMove > TimeRemaining / 3) TimeForMove = TimeRemaining / 3;
}

int GetTimeElaps()
{
	return GetTickCount() - StartTime;
}

void SetTime(int time, int op_time)
{
	TimeRemaining = time;
}

void SetTimeControl(int base, int inc)
{
	BaseTime = base * 1000 * 60;
	IncTime = inc;
}

bool CheckTime()
{
	if (GetTimeElaps() >= TimeForMove) return true;
	return false;
}
