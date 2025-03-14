#ifndef _RESERVA_H_
#define _RESERVA_H_

#include "myutils.h"
#include "voo.h"

#define MAX_HASH 30000

/* Tipos de Dados */

struct reserva{
	Voo *voo;
	char *id;
	int numPassageiros;
	struct reserva *prox;
};

typedef struct reserva Reserva;

/* Variáveis Globais */

extern Reserva *_head;	/* ponteiro para o inicio da lista de reservas */

/* Função final */

void limpaTudo();

/* Funções Reservas */

int encontraReserva(char *);

int validaIDReserva(char *);

int validaReserva(Reserva *, char *, Data);

void mostraReservas(char *, Data);

void criaReserva(Reserva *);

int leIDReserva(Reserva *);

char *leID();

void adicionaListaReservas();

void eliminaReservasVoo(char *);

void eliminaVoo(char *);

void eliminaReserva(char *);

void eliminaVooReserva();

#endif