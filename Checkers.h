#ifndef Checkers_H
#define Checkers_H

#include "EdAccess.h"

// -------------------------   defines

#define WHITE	1
#define BLACK	2
#define KING	4
#define OFF     0xf0
#define CHANGE_COLOR 3

#define WHITE_MAN  (WHITE)
#define WHITE_KING (WHITE | KING)
#define BLACK_MAN  (BLACK)
#define BLACK_KING (BLACK | KING)

#define OP_CHECKER(x) (Board[x] & (stm ^ CHANGE_COLOR))

#define MOVE_BUFFER_LEN 409600
#define WIN 32767
#define ED_WIN 30000
#define MAX_DEPTH 128

#define MAN_VALUE 100
#define KING_VALUE 300

// -------------------------   structures

struct Move {
	unsigned from;
	unsigned to;
	bool promotion;
	unsigned cap_sq[12];
	unsigned cap_type[12];
};

// -------------------------   variables

extern bool StopRequest;
extern bool AnalyseMode;
extern int stm;
extern unsigned Board[45];
extern double PerftNodes;
extern Move *MP;
extern Move MoveBuffer[MOVE_BUFFER_LEN];
extern Move PV[MAX_DEPTH][MAX_DEPTH];
extern double Nodes;
extern unsigned Map_32_to_45[32];
extern unsigned Map_64_to_45[64];
extern unsigned Pieces;
extern EdAccess *ED;
extern unsigned EdPieces;
extern bool EdNocaptures;
extern bool Reversible[MAX_DEPTH];



typedef void (__stdcall *PF_SearchInfo)(int score, int depth, int speed, char *pv, char *cm);
extern PF_SearchInfo pfSearchInfo;
typedef void (__stdcall *PF_SearchInfoEx)(char *score, char *depth, char *speed, char **pv, char *cv);
extern PF_SearchInfoEx pfSearchInfoEx;



// -------------------------   prototypes

void NewGame();
void StrToMove(char *s, Move *m);
void MoveToStr(Move *m, char *s);
void SetupBoard(char *p);
void SetTimeControl(int base, int inc);
void SetTime(int time, int op_time);
void Init();
void Perft(unsigned depth);
void GenerateCaptures();
void GenerateAllMoves();
//void MakeMove(char *s);
void MakeMove(Move *m, unsigned ply);
void UnmakeMove(Move *m);
void PrintBoard();
void StartTimer();
int GetTimeElaps();
int Eval();
Move RootSearch();
bool CheckTime();
int EdProbe();

#endif