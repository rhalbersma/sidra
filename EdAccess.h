#ifndef ED_ACCESS_H
#define ED_ACCESS_H

// интерфейсный класс для доступа к эндшпильным базам
struct EdAccess
{
	// структуры для представления шашечной позиции 

	struct EdBoard1
	{
		// все поля идут по порядку b8, d8, и т.д. до g1
		unsigned char board[32];
	};

	struct EdBoard2
	{
		unsigned char *wman;
		unsigned wman_cnt;

		unsigned char *wkings;
		unsigned wkings_cnt;

		unsigned char *bman;
		unsigned bman_cnt;

		unsigned char *bkings;
		unsigned bkings_cnt;
	};


	// возвращаемые значения

    enum
	{
		draw      =      0,
		win       =  10000,
		lose      = -10000,
		not_found =  32000
	};


	// коды для обозначений шашек

	enum
	{
		white = 1,
		black = 2,
		empty = 4,
		king  = 8
	};


	// флаги для доступа к базе

	enum
	{
		in_mem = 1
	};


	// загрузить базы
	// пока такие типы игр:
	// 		russian
	// 		russianlosers
	// 		brazil
	// 		brazillosers
	// 		pool
	// 		poollosers
	// 		checkers
	// 		checkerslosers

	virtual unsigned __stdcall Load(char *game_type) = 0;


	// получить тип базы

	virtual char * __stdcall GetBaseType() = 0;


	// оценка позиции (всегда ход белых)

	virtual int __stdcall GetResult(EdBoard1 *board, unsigned flags) = 0;
	virtual int __stdcall GetResult(EdBoard2 *board, unsigned flags) = 0;


	// получить указатель на таблицу по материалу

	virtual unsigned __stdcall GetTable(unsigned wm, unsigned wk, unsigned bm, unsigned bk) = 0;


	// получить указатель на таблицу по материалу и по наиболее продвинутой шашке

	virtual unsigned __stdcall GetTable(unsigned wm, unsigned wk, unsigned bm, unsigned bk, unsigned rank) = 0;


	// проверка загруженности таблицы целиком в память

	virtual unsigned __stdcall IsTableInMemory(unsigned table) = 0;

	
	// получить индекс в таблице

	virtual unsigned __int64 __stdcall GetIndex(EdBoard1 *board) = 0;
	virtual unsigned __int64 __stdcall GetIndex(EdBoard2 *board) = 0;

	
	// получить оценку по указателю на таблицу и индексу

	virtual int __stdcall GetResult(unsigned table, unsigned __int64 index, unsigned flags) = 0;
};

#endif