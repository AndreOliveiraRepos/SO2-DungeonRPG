#include "Server.h"
#include "Jogador.h"

//SignUp and connect clients
DWORD WINAPI RecebeClientes(LPVOID param) {
	DWORD n;
	while (!fim && totalConnections < MAX_CLIENTS) {

		_tprintf(TEXT("[SERVER] Vou passar � cria��o de uma c�pia do pipe '%s' ... \n"), PIPE_NAME);
		if ((gClients[totalConnections].hPipe = CreateNamedPipe(PIPE_NAME, PIPE_ACCESS_DUPLEX, PIPE_WAIT | PIPE_TYPE_MESSAGE
			| PIPE_READMODE_MESSAGE, MAX_CLIENTS, BUFFERSIZE * sizeof(TCHAR), BUFFERSIZE * sizeof(TCHAR),
			1000, NULL)) == INVALID_HANDLE_VALUE){
			_tperror(TEXT("Erro ao criar named pipe!"));
			exit(-1);
		}

		//Espera a liga��o de um cliente (ConnectNamedPipe � bloqueante)
		_tprintf(TEXT("[SERVER] Esperar liga��o de um cliente...\n"));
		if (!ConnectNamedPipe(gClients[totalConnections].hPipe, NULL)) {
			_tperror(TEXT("Erro na liga��o!"));
			exit(-1);
		}

		//Atende o cliente que se ligou
		gClients[totalConnections].hThread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AtendeCliente, (LPVOID)totalConnections, 0, &n);
		NovoJogador(&gClients[totalConnections]); //prepara os dados do jogador
		totalConnections++;
	}
	//DesligarNamedPipes(); //depois de fim //VERIFICAR!!
	return 0;
}

DWORD WINAPI AtendeCliente(LPVOID param) {
	//ide buscar o handle do cliente que est� no array global
	HANDLE hPipeCliente = gClients[(int)param].hPipe;

	TCHAR buf[256];
	DWORD n;
	BOOL ret;

	ClientRequest pedido;
	ServerResponse resposta;

	do {
		ret = ReadFile(hPipeCliente, &pedido, sizeof(ClientRequest), &n, NULL);
		buf[n / sizeof(TCHAR)] = '\0';
		if (!ret || !n)
			break;
		_tprintf(TEXT("[Server] Recebi %d bytes: \'%s\'... (ReadFile)\n"),n,buf);

		if (!start) {
			switch (pedido.command)
			{
			case SETNAME:
				_tcscpy(gClients[(int)param].nome, pedido.msg);

				_tcscpy(resposta.msg, "O teu nome � %s", pedido.msg);
				
				if (!WriteFile(hPipeCliente, &resposta, _tcslen(buf)*sizeof(TCHAR), &n, NULL)) {
					_tperror(TEXT("[ERRO] Escrever no pipe... (WriteFile)\n"));
					exit(-1);
				}
				break;
			default:
				break;
			}
		}
		else {

		}
		/* Enviar para todos */
		ServerBroadcasting();
	}
	while (pedido.command != QUITGAME);

	_tprintf(TEXT("[SERVIDOR] Cliente [%d] desligou-se!\n"), hPipeCliente);
	return 0;
}

//Disconnect from all pipes
void DesligarNamedPipes() {
	for (int i = 0; i < totalConnections; i++) {
		DisconnectNamedPipe(gClients[i].hPipe);
		_tprintf(TEXT("[ESCRITOR] Vou desligar o pipe... (CloseHandle)\n"));
		CloseHandle(gClients[i].hPipe);
	}
}

void ServerBroadcasting() { //Depois passa-se a estrutura por par�metro
	TCHAR buf[BUFFERSIZE];
	DWORD n;
	//do {
		_tcscpy(buf, TEXT("Isto � o jogo vindo do servidor"));
		//Escrever para todos os clientes inscritos
		for (int i = 0; i < totalConnections; i++)
			if (!WriteFile(gClients[i].hPipe, buf, _tcslen(buf)*sizeof(TCHAR), &n, NULL)) {
				_tperror(TEXT("[ERRO] Escrever no pipe... (WriteFile)\n"));
				exit(-1);
			}

		/*_tprintf(TEXT("[SERVER] Enviei %d bytes aos %d clientes... (WriteFile)\n"), n, totalConnections);
	} while (_tcsncmp(buf, TEXT("fim"), 3));*/
}
//2230