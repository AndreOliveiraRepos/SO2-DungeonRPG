#pragma once
#include "Monster.h"

Monstro NovoMonstroBully(int nPassos,int hp,int x,int y) {
	Monstro m;
	m.hp = hp;
	m.lentidao = 7;
	m.tipo = 51;
	m.posX = x;
	m.posY = y;
	m.stamina = m.lentidao;
	m.passos = nPassos;
	m.direcao = 2;
	return m;
}

Monstro NovoMonstroDistraido(int nPassos,int hp,int x, int y) {
	Monstro m;
	m.hp = hp;
	m.lentidao = 3;
	m.tipo = 52;
	m.posX = x;
	m.posY = y;
	m.passos = nPassos;
	m.stamina = m.lentidao;
	return m;
}

void MoveMonstro(Labirinto * shLab, int d, Monstro *m) {
	switch (d)
	{
	case UP:
	{		
		//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
		WaitForSingleObject(gMutexLabirinto, INFINITE);

		if ((m->posY - 1) >= 1 && !hasWallIn(m->posX, m->posY - 1) && !hasMonsterIn(m->posX, m->posY - 1)) {
			if (shLab->labirinto[m->posY][m->posX] > 1000)
			{
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] - (m->tipo * 100);
			}
			else
			{
				shLab->labirinto[m->posY][m->posX] = EMPTY;
			}
			m->posY = m->posY - 1;
			if (shLab->labirinto[m->posY][m->posX] == EMPTY)
			{
				shLab->labirinto[m->posY][m->posX] = m->tipo;
			}
			else {
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] + (m->tipo * 100);
			}
			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			ReleaseMutex(gMutexLabirinto);
			break;
		}
		else {
			m->direcao = MudaDirecao(UP);

			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			//A fun��o pai, tem de entregar o recurso antes do "filho" pedir.
			ReleaseMutex(gMutexLabirinto);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}
	case DOWN:
	{	
		//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
		WaitForSingleObject(gMutexLabirinto, INFINITE);
		if ((m->posY + 1) <= LABIRINTOSIZE - 2 && !hasWallIn( m->posX, m->posY + 1 ) && !hasMonsterIn(m->posX, m->posY + 1)) {
			//_tprintf(TEXT("[sfsfsf] a mover para (%d)\n"), m->posY+1);
			//system("pause");
			if (shLab->labirinto[m->posY][m->posX] > 1000)
			{
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] - (m->tipo * 100);
			}
			else
			{
				shLab->labirinto[m->posY][m->posX] = EMPTY;
			}
			m->posY = m->posY + 1;
			if (shLab->labirinto[m->posY][m->posX] == EMPTY)
			{
				shLab->labirinto[m->posY][m->posX] = m->tipo;
			}
			else {
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] +( m->tipo * 100);
			}
			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			ReleaseMutex(gMutexLabirinto);
			break;
		}
		else {
			m->direcao = MudaDirecao(DOWN);
			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			ReleaseMutex(gMutexLabirinto);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}
	case RIGHT:
	{	
		//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
		WaitForSingleObject(gMutexLabirinto, INFINITE);

		if ((m->posX + 1) <= LABIRINTOSIZE - 2 && !hasWallIn(m->posX + 1, m->posY) && !hasMonsterIn( m->posX+1, m->posY)) {
			if (shLab->labirinto[m->posY][m->posX] > 1000)
			{
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] - (m->tipo * 100);
			}
			else
			{
				shLab->labirinto[m->posY][m->posX] = EMPTY;
			}
			m->posX = m->posX + 1;
			if (shLab->labirinto[m->posY][m->posX] == EMPTY)
			{
				shLab->labirinto[m->posY][m->posX] = m->tipo;
			}
			else {
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] + (m->tipo * 100);
			}
			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			ReleaseMutex(gMutexLabirinto);
			break;
		}
		else {
			m->direcao = MudaDirecao(RIGHT);
			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			ReleaseMutex(gMutexLabirinto);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}
	case LEFT:
	{	
		//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
		WaitForSingleObject(gMutexLabirinto, INFINITE);

		if ((m->posX - 1) >= 1 && !hasWallIn(m->posX - 1, m->posY) && !hasMonsterIn(m->posX - 1, m->posY)) {
			if (shLab->labirinto[m->posY][m->posX] > 1000)
			{
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] - (m->tipo * 100);
			}
			else
			{
				shLab->labirinto[m->posY][m->posX] = EMPTY;
			}
			m->posX = m->posX - 1;
			if (shLab->labirinto[m->posY][m->posX] == EMPTY)
			{
				shLab->labirinto[m->posY][m->posX] = m->tipo;
			}
			else {
				shLab->labirinto[m->posY][m->posX] = shLab->labirinto[m->posY][m->posX] + (m->tipo * 100);
			}
			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			ReleaseMutex(gMutexLabirinto);
			break;
		}
		else {
			m->direcao = MudaDirecao(LEFT);
			//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
			ReleaseMutex(gMutexLabirinto);
			MoveMonstro(shLab, m->direcao, m);
			break;
		}
		
	}
	default:
		break;
	}
	//No fim de mover, sinaliza o evento que vai actualizar a interface dos clientes.
	SetEvent(ghUpdateGameClientEvent);
}

int MudaDirecao(int anterior) {
	int r = anterior;

	while (r == anterior)
	{
		r = rand() % 4 + 1;
	}
	return r;
}

//Memoria

void InitializeSharedMemory(HANDLE * hMappedObj) {

	*hMappedObj = OpenFileMapping(FILE_MAP_ALL_ACCESS,
		FALSE,
		TEXT("ShLabirinto"));
	if (hMappedObj == NULL) {
		_tprintf(TEXT("[Erro] Criar objectos mapeados(%d)\n"), GetLastError());
		system("pause");
		return -1;
	}

	shLabirinto = (Labirinto*)MapViewOfFile(*hMappedObj,
		FILE_MAP_ALL_ACCESS,
		0,
		0,
		sizeof(Labirinto)
		);
	if (shLabirinto == NULL) {
		_tprintf(TEXT("[Erro] Mapear para mem�ria(%d)\n"), GetLastError());
		system("pause");
		return -1;
	}
}

void DisplayMonsterSurroundings(int x, int y,int hp) {
	//int matriz[MONSTER_MAT_SIZE][MONSTER_MAT_SIZE]; //Criar um di�mentro de vis�o para o monstro

	int iniX, iniY, maxX, maxY;

	if (x - (MONSTER_MAT_SIZE/2) < 0) iniX = 0;
	else iniX = x - MONSTER_MAT_SIZE / 2;
	if (x + MONSTER_MAT_SIZE / 2 > LABIRINTOSIZE) maxX = LABIRINTOSIZE;
	else maxX = x + MONSTER_MAT_SIZE / 2;

	if (y - MONSTER_MAT_SIZE / 2 < 0) iniY = 0;
	else iniY = y - MONSTER_MAT_SIZE / 2;
	if (y + MONSTER_MAT_SIZE / 2 > LABIRINTOSIZE) maxY = LABIRINTOSIZE;
	else maxY = y + MONSTER_MAT_SIZE / 2;

	int m = 0, n = 0;

	//Labirinto Ocupado - Bloqueia o acesso ao labirinto por outras threads
	WaitForSingleObject(gMutexLabirinto, INFINITE);

	clrscr();
	for (int i = iniY; i < maxY; i++, n++, m = 0)
	{
		for (int j = iniX; j < maxX; j++, m++)
		{
			int c = shLabirinto->labirinto[i][j];
			if (c > 1000) {
				c = c % 100;
				if (c >= PLAYER_START_INDEX && c <= PLAYER_END_INDEX)
				{
					_tprintf(TEXT("%c"), 64);
				}
				else if (c >= MONSTER_START_INDEX && c <= MONSTER_END_INDEX)
				{
					_tprintf(TEXT("%c"), 167);
				}
				else if (c >= ITEM_START_INDEX && c <= ITEM_END_INDEX)
				{
					_tprintf(TEXT("%c"), '+');
				}
				else if (c > PEDRAS && c < 600)
				{
					_tprintf(TEXT("%c"), 'P');
				}
			}
			else
			{
				if (c >= WALL_START_INDEX && c <= WALL_END_INDEX)
				{
					_tprintf(TEXT("%c"), 9619);
				}
				else if (c == EMPTY)
				{
					_tprintf(TEXT("%c"), 9617);
				}
				else if (c >= PLAYER_START_INDEX && c <= PLAYER_END_INDEX)
				{
					_tprintf(TEXT("%c"), 64);
				}
				else if (c >= MONSTER_START_INDEX && c <= MONSTER_END_INDEX)
				{
					_tprintf(TEXT("%c"), 167);
				}
				else if (c >= ITEM_START_INDEX && c <= ITEM_END_INDEX)
				{
					_tprintf(TEXT("%c"), '+');
				}
				else if (c > PEDRAS && c < 600)
				{
					_tprintf(TEXT("%c"), 'P');
				}
			}
			
		}
		_tprintf(TEXT("\n"));
	}
	_tprintf(TEXT("Monster at: Y:%d X:%d\n HP: %d"),y,x, hp);

	//Liberta Labirinto - Desbloqueia o acesso ao labirinto a outras threads
	ReleaseMutex(gMutexLabirinto);
}

void ReadSharedMemory() {
	//while (!fim) {
		system("cls");
		for (int i = 0; i < LABIRINTOSIZE; i++)
		{
			for (int j = 0; j < LABIRINTOSIZE; j++)
			{
				int c = shLabirinto->labirinto[i][j];
				if (c >= WALL_START_INDEX && c <= WALL_END_INDEX)
				{
					_tprintf(TEXT("%c"), 9619);
				}
				else if (c == EMPTY)
				{
					_tprintf(TEXT("%c"), 9617);
				}
				else if (c >= PLAYER_START_INDEX && c <= PLAYER_END_INDEX)
				{
					_tprintf(TEXT("%c"), 64);
				}
				else if (c >= MONSTER_START_INDEX && c <= MONSTER_END_INDEX)
				{
					_tprintf(TEXT("%c"), 167);
				}
				else if (c >= ITEM_START_INDEX && c <= ITEM_END_INDEX)
				{
					_tprintf(TEXT("%c"), '+');
				}
				else if (c > PEDRAS && c < 600)
				{
					_tprintf(TEXT("%c"), 'P');
				}
			}
			_tprintf(TEXT("\n"));
		//}
		//Vai ler o que est� em mem�ria de 1 em 1 seg
		//Sleep(1000);
	}
}

void CloseHandles(HANDLE * hMappedObj) {
	UnmapViewOfFile(shLabirinto);
	CloseHandle(*hMappedObj);
}


void gotoxy(int x, int y) {
	COORD coord;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
	coord.X = x;
	coord.Y = y;
	SetConsoleCursorPosition(hStdout, coord);
}


void escondeCursor() {
	CONSOLE_CURSOR_INFO cci;
	HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

	cci.bVisible = FALSE;				// Desativa a visibilidade do cursor [TRUE/FALSE]
	cci.dwSize = 20;					// Define a altura do cursor [1-100]

	SetConsoleCursorInfo(hStdout, &cci);	// Aplica as defini��es anteriores
}

void CheckForThreats(Monstro *m) {
	
	if (shLabirinto->labirinto[m->posY][m->posX] > 1000 && !hasItemIn(shLabirinto->labirinto[m->posY][m->posX]) )
	{
		//_tprintf(TEXT("threat at Y:%d X: %d\nvalor no mapa:%d\nResto = %d\n"), m->posY, m->posX, shLabirinto->labirinto[m->posY][m->posX], shLabirinto->labirinto[m->posY][m->posX] % 100);
		//system("pause");
		m->hp++;
	}
}

void clrscr() {
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	const COORD startCoords = { 0, 0 };
	DWORD dummy;
	HANDLE hconsola = GetStdHandle(STD_OUTPUT_HANDLE);;

	GetConsoleScreenBufferInfo(hconsola, &csbi);
	FillConsoleOutputCharacter(hconsola,
		' ',
		csbi.dwSize.X * csbi.dwSize.Y,
		startCoords,
		&dummy);
	FillConsoleOutputAttribute(hconsola,
		csbi.wAttributes,
		csbi.dwSize.X * csbi.dwSize.Y,
		startCoords,
		&dummy);
	gotoxy(0, 0);
}

BOOL hasWallIn(int x, int y) {
	return shLabirinto->labirinto[y][x] >= WALL_START_INDEX
		&& shLabirinto->labirinto[y][x] <= WALL_END_INDEX;
}

BOOL hasMonsterIn(int x, int y) {
	return shLabirinto->labirinto[y][x] >= MONSTER_START_INDEX
		&& shLabirinto->labirinto[y][x] <= MONSTER_END_INDEX;
}

BOOL hasItemIn(int d) {
	
	int mp = (d - 501) / 100;
	
	
	if ((mp >= MONSTER_START_INDEX && mp <= MONSTER_END_INDEX))
	{
		
		return TRUE;
	}
	else if ((d % 100 >= ITEM_START_INDEX && d % 100 <= ITEM_END_INDEX))
	{
		
		return TRUE;
	}
	else
		return FALSE;
}

BOOL hasPlayerIn(int x, int y) {
	return shLabirinto->labirinto[y][x] >= PLAYER_START_INDEX
		&& shLabirinto->labirinto[y][x] <= PLAYER_END_INDEX;
}


//procura player
void CheckForPlayers(Monstro *m) {
	

	WaitForSingleObject(gMutexLabirinto, INFINITE);
	for (int i = 0; i < LABIRINTOSIZE; i++)
	{
		for (int j = 0; j < LABIRINTOSIZE; j++)
		{
			
			int c = shLabirinto->labirinto[i][j];
			if (!hasItemIn(c) && hasPlayerIn(i, j))
			{
				if (i > m->posY)//se estiver abaixo
				{
					MoveMonstro(shLabirinto,DOWN,m);
					break;
				}
				else if (i < m->posY) //se estiver acima
				{
					MoveMonstro(shLabirinto, UP, m);
					break;
				}
				else
				{
					if (j > m->posX)// se estiver � direita
					{
						MoveMonstro(shLabirinto, LEFT, m);
						break;
					}
					else if (j < m->posX)//se estiver � esquerda
					{
						MoveMonstro(shLabirinto, RIGHT, m);
						break;
					}
				}
			}
			
		}
	}
	ReleaseMutex(gMutexLabirinto);
}

	