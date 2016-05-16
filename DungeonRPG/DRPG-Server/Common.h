#pragma once
#ifndef COMMON_HEADER
#define COMMON_HEADER
#define _CRT_SECURE_NO_WARNINGS

#include <windows.h>
#include <tchar.h>
#include <io.h>
#include <fcntl.h>
#include <stdio.h>

#ifdef UNICODE 
#define tcout wcout
#define tcin wcin
#define tcerr wcerr
#define tstring wstring
#else
#define tcout cout
#define tcin cin
#define tcerr cerr
#define tstring string
#endif

// ---  Game Constants
#define MAX_CLIENTS 10
#define BUFFERSIZE 256
#define PLAYER_LOS 10	//player line of sight
#define LABIRINTOSIZE 70

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

// - Ints da matriz
	//0 - espa�o vazio
	//players 1~19
	//paredes 20~29
	//objectos boost 30~49
	//monstros 51~99
	//pedras 50(quant), 500+ , ex 503 -> 3 pedras numa posi��o
	#define EMPTY -1
	#define PLAYER_STONE_CAP 15
	#define HP_BASE 100
	#define LENTIDAO_BASE 7
	#define PLAYER_START_INDEX 0
	#define PLAYER_END_INDEX 19

	#define FOG_OF_WAR -2
	#define STONEWALL 20
	// ... v�rios tipos (chamar tijolo, stonewall, agua, erva, etc?)?

	#define VITAMINA	30	//+1 HP (Up to 200% base hp (200))
	#define ORANGE_BULL 31	//+3 HP (Up to 200% base hp (200))
	#define REB_CAFEINA 32	//-2 Lentid�o (1min, n�o stackam)
	#define PEDRAS 500		//+1 ataque quando usada

// ---  Game Globals
extern BOOL fim;
extern BOOL start;
extern int totalConnections;

#endif
