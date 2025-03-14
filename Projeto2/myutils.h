#ifndef _MY_UTILS_H_
#define _MY_UTILS_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h> 

#define MAX_DATA 11				/* dimensão da data */
#define MAX_HORA 6				/* dimensão da hora */
#define ANO_INICIO 2022			/* ano inicial do sistema */
#define DIAS_ANO 365			/* número de dias no ano */
#define HORAS_DIA 24			/* número de horas por dia */
#define MINUTOS_HORA 60			/* número de minutos numa hora */
#define MINUTOS_DIA 1440		/* número de minutos do dia */
#define NAO_EXISTE -1			/* código de erro */
#define TRUE 1					/* verdadeiro */
#define FALSE 0					/* falso */

typedef struct {
	int dia;
	int mes;
	int ano;
} Data;

typedef struct {
	int hora;
	int minuto;
} Hora;

extern Data _hoje;	/* data atual do sistema */

extern const int _diasMesAc[]/* dias acumulados por mês (jan=1) */;

/* Funcoes Leitura */

Hora leHora();

Data leData();

int leProximaPalavra(char *);

void lePalavraAteFimDeLinha(char *);

void leAteFimDeLinha();

/* Funcoes Datas e Horas */

void mostraData(Data);

void mostraHora(Hora);

int converteDataNum(Data);

int converteHoraNum(Hora);

int converteDataHoraNum(Data, Hora);

Hora converteNumHora(int);

Data converteNumData(int);

int validaData(Data);

int validaHora(Hora);

int cmpData(Data, Data);

void alteraData();

/* Algoritmo de ordenação BubbleSort */

void bubbleSort(int *, int, int (*cmpFunc) (int a, int b));

#endif