﻿# SO2-DungeonRPG
Projecto Final - Sistemas Operativos 2 - Engenharia Informática
________________________

### Servidor:

//Finished

[EXTRA] criar novo objecto chamado crate que pode sair um item aleatóriamente ou armadilha

[EXTRA] Colocar o servidor como serviço

[IMPORTANTE] Sincronizar jogadores e monstros

### Cliente:

//Finished

### Monstros:

[feito]Memória partilhada

Mostro Distraído - Anda aleatóriamente pelo labirinto, lentidão 3, agressive = 0;

Mostro Bully (Agressivo) - Tem LOS(mesmo do player?) para detectar jogador, quando detectar, move-se na direcção dele, caso contrário, anda aleatóriamente. Lentidão 7, agressivo = 1;

Algoritmo aleatório: O monstro anda N posições numa direcção (N é recebido por argumento da função main), depois de N passos, tem 75% de mudar de direcção, (Sortear as 4 posições a 25% cada).

O mostro colide com paredes (pode-se usar o código que já existe no server dos jogadores)

Os monstros têm lifeDrain/lifeSteal, 1 hp do jogador para o monstro

Quando atingem 160% dividem-se, lançam um novo monstro (criam um novo processo) numa casa adjacente e ficam com 80% de vida cada um. O novo processo é lançado pelo monstro original que atingiu 160% de hp.

Primeiros monstros lançados pelo servidor

Têm de ter a capacidade de lançar um processo(monstro)

Usar mutex entre processos para sincronizar os monstros



