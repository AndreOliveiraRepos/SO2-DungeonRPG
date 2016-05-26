#include "Server.h"
#include "Jogador.h"

TCHAR broadcastMessage[BUFFERSIZE];

//SignUp and connect clients
DWORD WINAPI RecebeClientes(LPVOID param) {
	DWORD n;
	while (!fim && totalConnections < MAX_CLIENTS) {

		//PIPE TO HANDLE REQUESTS
		_tprintf(TEXT("[SERVER] Vou passar � cria��o de uma c�pia do pipe '%s' ... \n"), PIPE_NAME);
		if ((gClients[totalConnections].hPipe = CreateNamedPipe(
			PIPE_NAME, 
			PIPE_ACCESS_DUPLEX, 
			PIPE_WAIT | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 
			MAX_CLIENTS, 
			BUFFERSIZE * sizeof(TCHAR), 
			BUFFERSIZE * sizeof(TCHAR),
			1000, NULL)) == INVALID_HANDLE_VALUE){
			_tperror(TEXT("Erro ao criar named pipe!"));
			exit(-1);
		}

		//PIPE FOR SENDING GAME BOARD
		_tprintf(TEXT("[SERVER] Vou passar � cria��o de uma c�pia do pipe '%s' ... \n"), PIPE_NAME_JOGO);
		if ((gClients[totalConnections].hPipeJogo = CreateNamedPipe(
			PIPE_NAME_JOGO, 
			PIPE_ACCESS_OUTBOUND, 
			PIPE_WAIT | PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE, 
			MAX_CLIENTS, 
			sizeof(ServerResponse), 
			sizeof(ServerResponse),
			1000, NULL)) == INVALID_HANDLE_VALUE) {
			_tperror(TEXT("Erro ao criar named pipe para o jogo!"));
			exit(-1);
		}

		//Espera a liga��o de um cliente (ConnectNamedPipe � bloqueante)
		_tprintf(TEXT("[SERVER] Esperar liga��o de um cliente...\n"));
		if (!ConnectNamedPipe(gClients[totalConnections].hPipe, NULL)) {
			_tperror(TEXT("Erro na liga��o!"));
			exit(-1);
		}

		if (!ConnectNamedPipe(gClients[totalConnections].hPipeJogo, NULL)) {
			_tperror(TEXT("Erro na liga��o!"));
			exit(-1);
		}

		NovoJogador(totalConnections); //prepara os dados do jogador
		//Atende o cliente que se ligou
		gClients[totalConnections].hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtendeCliente, (LPVOID)totalConnections, 0, &n);
		totalConnections++;
	}
	//DesligarNamedPipes(); //depois de fim //VERIFICAR!!
	return 0;
}

DWORD WINAPI GameTimer(LPVOID param){
	ResetEvent(ghGameInstanceEvent);	//Reseta o evento para o estado inicial
	Sleep(1800);
	//Sleep(1000/15);						//Passa um instante de jogo
	SetEvent(ghGameInstanceEvent);		//Sinaliza o evento
}

DWORD WINAPI AtendeCliente(LPVOID param) {
	//ide buscar o handle do cliente que est� no array global
	HANDLE hPipeCliente = gClients[(int)param].hPipe;

	TCHAR buf[BUFFERSIZE];
	TCHAR respostaServidor[BUFFERSIZE];
	DWORD n;
	BOOL ret;

	ClientRequest pedido;

	do {
		ret = ReadFile(hPipeCliente, &pedido, sizeof(ClientRequest), &n, NULL);
		if (!ret || !n) break;
		_tprintf(TEXT("[Server] Recebi %d bytes: \'%s\'... (ReadFile)\n"),n,pedido.msg);
		memset(respostaServidor, '\0', sizeof(TCHAR) * BUFFERSIZE); //prepara a "string" de resposta

		if (!start) {
			switch (pedido.command)
			{
			case GAMESTATUS:
				//swprintf(resposta.msg, "%d", totalConnections); //Clientes Ligados
				_tcscpy(respostaServidor, TEXT("Estat�sticas dados pr�-jogo...\n"));
				_swprintf(buf, TEXT("%d"), totalConnections);
				_tcscat(respostaServidor, TEXT("Total de jogadores ligados: "));
				_tcscat(respostaServidor, buf);
				break;
			case SETNAME:
				_tcscpy(gClients[(int)param].nome, pedido.msg);
				_tcscpy(respostaServidor, TEXT("Est�s registado como: "));
				_tcscat(respostaServidor, pedido.msg);

				//Broadcast
				_tcscat(broadcastMessage, TEXT("Novo Cliente Ligado: "));
				_tcscat(broadcastMessage, gClients[(int)param].nome);
				break;

			case STARTGAME:
				if (!start) {
					start = TRUE;
					_tcscpy(respostaServidor, TEXT("Come�aste um novo jogo!"));
				}
				else {
					_tcscpy(respostaServidor, TEXT("O jogo j� come�ou!"));
				}

				//Broadcast
				_tcscat(broadcastMessage, TEXT("Novo jogo iniciado por: "));
				_tcscat(broadcastMessage, gClients[(int)param].nome);
				break;

			case QUITGAME:
				break;

			default:
				break;
			}
		}
		else { //O jogo est� a decorrer
			if (pedido.command == MOVEUP || pedido.command == MOVEDOWN
				|| pedido.command == MOVELEFT || pedido.command == MOVERIGHT) {
				MoverJogador((int)param, (pedido.command - 600));
				_tcscpy(respostaServidor, TEXT("A mover..."));
			}
			else {
				switch (pedido.command)
				{
				case GAMESTATUS:
					_tcscpy(respostaServidor, TEXT("Estat�sticas de jogo a decorrer"));
					break;
				case SWITCH_STONE_AUTOHIT:
					if (gClients[(int)param].stoneAutoHit) {
						gClients[(int)param].stoneAutoHit = FALSE;
						_tcscpy(respostaServidor, TEXT("Stone AutoHit: Desligado!"));
					}
					else {
						gClients[(int)param].stoneAutoHit = TRUE;
						_tcscpy(respostaServidor, TEXT("Stone AutoHit: Ligado!"));
					}
					break;

				case QUITGAME:
					break;
				}
			} //fimif - mover
		} //fimif - start
		//Entrega a resposta final ao cliente
		_tprintf(TEXT("[Server] A responder com %d bytes: \'%s\' ao cliente [%d] (ReadFile)\n"), n, respostaServidor, (int)param);
		if (!WriteFile(hPipeCliente, respostaServidor, sizeof(TCHAR) * BUFFERSIZE, &n, NULL)) {
			_tperror(TEXT("[ERRO] Responder ao cliente... (WriteFile)\n"));
			exit(-1);
		}
	} while (pedido.command != QUITGAME);

	DisconnectNamedPipe(hPipeCliente);
	DisconnectNamedPipe(gClients[(int)param].hPipeJogo);
	CloseHandle(hPipeCliente);
	CloseHandle(gClients[(int)param].hPipeJogo);

	_tprintf(TEXT("[SERVIDOR] Cliente [%d] desligou-se!\n"), gClients[(int)param].id);
	gClients[(int)param].hp = 0;
	//deixa cair pedras
	printf("Thread %d exiting\n", GetCurrentThreadId());
	return 0;
}

//Disconnect from all pipes (Provavelmente n�o vai ser preciso, s� para as threads)
void DesligarNamedPipes() {
	for (int i = 0; i < totalConnections; i++) {
		DisconnectNamedPipe(gClients[i].hPipe);
		_tprintf(TEXT("[ESCRITOR] Vou desligar o pipe... (CloseHandle)\n"));
		CloseHandle(gClients[i].hPipe);
	}
}

void DesligarThreadsDeCliente() {
	for (int i = 0; i < totalConnections; i++) {
		CloseHandle(gClients[i].hThread);
	}
}

int activePlayers() {
	int nPlayers = 0;
	for (size_t i = 0; i < totalConnections; i++)
		if (gClients[i].hp > 0) ++nPlayers;
	return nPlayers;
}

DWORD WINAPI ActualizaClientes(LPVOID param) {
	TCHAR buf[BUFFERSIZE];
	DWORD n;
	ServerResponse resposta;

	while (!fim) {
		
		if (totalConnections > 0 && activePlayers() > 0) {
			//prepara a "string" de resposta
			memset(resposta.msg, TEXT('\0'), sizeof(TCHAR) * BUFFERSIZE);
			if (broadcastMessage != TEXT('\0')) {
				_tcscpy(resposta.msg, broadcastMessage);
				memset(broadcastMessage, TEXT('\0'), sizeof(TCHAR));
			}
			//Para cada cliente
			for (int i = 0; i < totalConnections; i++) {
				if (gClients[i].hp > 0) {

					//Envia a matriz para os jogadores
					if (!start) SetEmptyMatrix(&resposta.matriz); //security
					else UpdatePlayerLOS(gClients[i].x, gClients[i].y, &resposta.matriz);
					if (!WriteFile(gClients[i].hPipeJogo, &resposta, sizeof(ServerResponse), &n, NULL)) {
						_tprintf(TEXT("[ERRO] Ao escrever no pipe do cliente %d:%s\n"), gClients[i].id, gClients[i].nome);
						gClients[i].hp = 0;
					}

					//Player Activity
					RecoverPlayerStamina(&gClients[i]);
					//WaitForSingleObject(mutexLabirinto, INFINITE);
					AttackClosePlayers(&gClients[i]);
					//ReleaseMutex(mutexLabirinto);

				}
			}//fim for
			_tprintf(TEXT("[SERVER] Enviei %d bytes aos %d clientes... (WriteFile)\n"), n, activePlayers);
		}
	}
	printf("Thread %d exiting\n", GetCurrentThreadId());
	return 0;
}
