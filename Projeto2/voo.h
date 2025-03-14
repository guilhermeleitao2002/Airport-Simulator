#ifndef _VOO_H_
#define _VOO_H_

#include "myutils.h"
#include "aeroporto.h"

#define MAX_NUM_VOOS 30000		/* número máximo de voos */
#define MAX_CODIGO_VOO 9		/* dimensão do código do voo */

/* Tipos de Dados */

typedef struct {
	char id[MAX_CODIGO_VOO];
	char partida[MAX_CODIGO_AEROPORTO];
	char chegada[MAX_CODIGO_AEROPORTO];
	Data data;
	Hora hora;
	Hora duracao;
	int capacidade;
	int vagas;
	int horaPartida;
	int horaChegada;
} Voo;

/* Variaveis Globais */

extern int _numVoos;		/* número de voos introduzidos */
extern Voo _voos[MAX_NUM_VOOS];	/* vetor de voos */

/* Funcoes Voos */

void mostraVoo(int);

void mostraVooPartida(int);

void mostraVooChegada(int);

int encontraVooData(char *, Data);

int encontraVoo(char *);

int validaIDVoo(char *);

int validaVoo(Voo);

void criaVoo(Voo);

void adicionaListaVoos();

int cmpVoosPartida(int, int);

int cmpVoosChegada(int, int);

void listaVoosPartida();

void listaVoosChegada();

#endif