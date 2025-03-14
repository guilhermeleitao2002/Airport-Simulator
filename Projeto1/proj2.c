/*
* Arquivo: proj2.c
* Autor: Guilherme Leitão ist199951
* Descrição: Este programa simula um sistema de gestão de aeroportos com várias
* funcionalidades, como registar, listar e excluir novos aeroportos,
* voos e reservas, bem como a gestão de datas. Além disso, também é
* capaz de organizar essas informações em ordem crescente de acordo com
* determinadas datas e horas de partida ou chegada ou por ordem alfabética.
* O programa considera o calendário e seus dias de forma adequada.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_NUM_AEROPORTOS 40	/* número máximo de areoportos */
#define MAX_NUM_VOOS 30000	/* número máximo de voos */

#define MAX_CODIGO_AEROPORTO 4	/* dimensão do código do aeroporto */
#define MAX_NOME_PAIS 31	/* dimensão do nome do pais */
#define MAX_NOME_CIDADE 51	/* dimensão do nome da cidade */

#define MAX_CODIGO_VOO 7	/* dimensão do código do voo */
#define MAX_DATA 11		/* dimensão da data */
#define MAX_HORA 6		/* dimensão da hora */

#define NAO_EXISTE -1		/* código de erro */

#define ANO_INICIO 2022		/* ano inicial do sistema */
#define DIAS_ANO 365		/* número de dias no ano */
#define HORAS_DIA 24		/* número de horas por dia */
#define MINUTOS_HORA 60		/* número de minutos numa hora */
#define MINUTOS_DIA 1440	/* número de minutos do dia */

#define TRUE 1			/* verdadeiro */
#define FALSE 0			/* falso */

/* Tipos de Dados */

typedef struct {
	char id[MAX_CODIGO_AEROPORTO];
	char pais[MAX_NOME_PAIS];
	char cidade[MAX_NOME_CIDADE];
	int numVoos;
} Aeroporto;

typedef struct {
	int dia;
	int mes;
	int ano;
} Data;

typedef struct {
	int hora;
	int minuto;
} Hora;

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

struct reserva{
	Voo *voo;
	char *id;
	int numPassageiros;
	struct reserva *prox;
};

typedef struct reserva Reserva;

/* Variaveis Globais */

int _numAeroportos = 0;		/* número de aeroportos introduzidos */
Aeroporto _aeroportos[MAX_NUM_AEROPORTOS];	/* vetor de aeroportos */

int _numVoos = 0;		/* número de voos introduzidos */
Voo _voos[MAX_NUM_VOOS];	/* vetor de voos */

Reserva *_reservas = NULL;	/* ponteiro para o inicio da lista de reservas */

Data _hoje = { 1, 1, 2022 };	/* data atual do sistema */

const int _diasMesAc[] =	/* dias acumulados por mês (jan=1) */
	{ 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };


/* Funcoes Leitura */

Hora leHora() {
	Hora h;
	scanf("%d:%d", &h.hora, &h.minuto);
	return h;
}


Data leData() {
	Data d;
	scanf("%d-%d-%d", &d.dia, &d.mes, &d.ano);
	return d;
}


int leProximaPalavra(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != ' ' && c != '\t' && c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
	return (c == '\n');
}


void lePalavraAteFimDeLinha(char str[]) {
	char c = getchar();
	int i = 0;
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != '\n') {
		str[i++] = c;
		c = getchar();
	}
	str[i] = '\0';
}


void leAteFimDeLinha() {
	char c = getchar();
	while (c != '\n')
		c = getchar();
}


/* Funcoes Datas e Horas */

void mostraData(Data d) {
	if (d.dia < 10)
		printf("0");
	printf("%d-", d.dia);
	if (d.mes < 10)
		printf("0");
	printf("%d-%d", d.mes, d.ano);
}


void mostraHora(Hora h) {
	if (h.hora < 10)
		printf("0");
	printf("%d:", h.hora);
	if (h.minuto < 10)
		printf("0");
	printf("%d", h.minuto);
}


int converteDataNum(Data d) {
	return (d.ano - ANO_INICIO) * DIAS_ANO + _diasMesAc[d.mes - 1] +
		d.dia - 1;
}


int converteHoraNum(Hora h) {
	return ((h.hora * MINUTOS_HORA) + h.minuto);
}


int converteDataHoraNum(Data d, Hora h) {
	return converteDataNum(d) * MINUTOS_DIA + converteHoraNum(h);
}


Hora converteNumHora(int num) {
	Hora h;
	h.minuto = num % MINUTOS_HORA;
	h.hora = ((num - h.minuto) / MINUTOS_HORA) % HORAS_DIA;
	return h;
}


Data converteNumData(int num) {
	Data d;
	int i = 0;
	num = (num - (num % MINUTOS_DIA)) / MINUTOS_DIA;
	d.ano = (num / DIAS_ANO) + ANO_INICIO;
	num = num - ((d.ano - ANO_INICIO) * DIAS_ANO);
	while (i <= 11 && num >= _diasMesAc[i])
		i++;
	d.mes = i;
	d.dia = num - _diasMesAc[i - 1] + 1;
	return d;
}


int validaData(Data d) {
	int numData = converteDataNum(d);
	Data proximoAno = _hoje;
	proximoAno.ano++;
	return !(numData < converteDataNum(_hoje)
		 || numData > converteDataNum(proximoAno));
}


int validaHora(Hora h) {
	return !(h.hora > 12 || (h.hora == 12 && h.minuto > 0));
}


int cmpData(Data a, Data b) {
	if(a.dia == b.dia && a.mes == b.mes && a.ano == b.ano)
		return 1;
	return 0;
}


/* Algoritmo de ordenação BubbleSort */

void bubbleSort(int indexes[], int size, int (*cmpFunc) (int a, int b)) {
  int i, j, done;
  
  for (i = 0; i < size-1; i++){
    done=1;
    for (j = size-1; j > i; j--) 
      if ((*cmpFunc)(indexes[j-1], indexes[j])) {
	int aux = indexes[j];
	indexes[j] = indexes[j-1];
	indexes[j-1] = aux;
	done=0;
      }
    if (done) break;
  }
}


/* Funcoes Aeroportos */

int aeroportoInvalido(char id[]) {
	int i;
	for (i = 0; id[i] != '\0'; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z'))
			return TRUE;
	return FALSE;
}


int encontraAeroporto(char id[]) {
	int i;
	for (i = 0; i < _numAeroportos; i++)
		if (!strcmp(id, _aeroportos[i].id))
			return i;
	return NAO_EXISTE;
}


void adicionaAeroporto() {
	Aeroporto a;

	leProximaPalavra(a.id);
	leProximaPalavra(a.pais);
	lePalavraAteFimDeLinha(a.cidade);

	if (aeroportoInvalido(a.id))
		printf("invalid airport ID\n");
	else if (_numAeroportos == MAX_NUM_AEROPORTOS)
		printf("too many airports\n");
	else if (encontraAeroporto(a.id) != NAO_EXISTE)
		printf("duplicate airport\n");
	else {
		strcpy(_aeroportos[_numAeroportos].id, a.id);
		strcpy(_aeroportos[_numAeroportos].pais, a.pais);
		strcpy(_aeroportos[_numAeroportos].cidade, a.cidade);
		_aeroportos[_numAeroportos].numVoos = 0;
		_numAeroportos++;
		printf("airport %s\n", a.id);
	}
}


void mostraAeroporto(int index) {
	printf("%s %s %s %d\n", _aeroportos[index].id,
	       _aeroportos[index].cidade, _aeroportos[index].pais,
	       _aeroportos[index].numVoos);
}


int cmpAeroportos(int a, int b) {
  return (strcmp(_aeroportos[a].id, _aeroportos[b].id) > 0);
}


void listaTodosAeroportos() {
	int i;
	int indexAeroportos[MAX_NUM_AEROPORTOS];

	for (i = 0; i < _numAeroportos; i++)
		indexAeroportos[i] = i;

	bubbleSort(indexAeroportos, _numAeroportos, cmpAeroportos);

	for (i = 0; i < _numAeroportos; i++)
		mostraAeroporto(indexAeroportos[i]);
}


void listaAeroportos() {
	char id[MAX_CODIGO_AEROPORTO];
	int indexAeroporto, ultima = 0;

	ultima = leProximaPalavra(id);
	if (strlen(id) == 0)
		listaTodosAeroportos();
	else {
		while (strlen(id) != 0) {
			indexAeroporto = encontraAeroporto(id);
			if (indexAeroporto == NAO_EXISTE)
				printf("%s: no such airport ID\n", id);
			else
				mostraAeroporto(indexAeroporto);
			if (!ultima)
				ultima = leProximaPalavra(id);
			else
				break;
		}
	}
}


/* Funcoes Voos */

void mostraVoo(int index) {
	printf("%s %s %s ", _voos[index].id, _voos[index].partida,
	       _voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
}


void mostraVooPartida(int index) {
	printf("%s %s ", _voos[index].id, _voos[index].chegada);
	mostraData(_voos[index].data);
	printf(" ");
	mostraHora(_voos[index].hora);
	printf("\n");
}


void mostraVooChegada(int index) {
	Hora h = converteNumHora(_voos[index].horaChegada);
	printf("%s %s ", _voos[index].id, _voos[index].partida);
	mostraData(converteNumData(_voos[index].horaChegada));
	printf(" ");
	mostraHora(h);
	printf("\n");
}


int encontraVooData(char id[], Data d) {
	int numData = converteDataNum(d);
	int i;

	for (i = 0; i < _numVoos; i++)
		if (!strcmp(id, _voos[i].id)
		    && numData == converteDataNum(_voos[i].data))
			return i;
	return NAO_EXISTE;
}


int encontraVoo(char id[]) {
	int i;

	for (i = 0; i < _numVoos; i++)
		if (!strcmp(id, _voos[i].id)){
			return i;
		}
	return NAO_EXISTE;
}


int validaIDVoo(char id[]) {
	int i = 0, l = strlen(id);
	if (l < 3)
		return FALSE;
	for (i = 0; i < 2; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z'))
			return FALSE;

	while (id[i] != '\0') {
		if (!(id[i] >= '0' && id[i] <= '9'))
			return FALSE;
		i++;
	}
	return TRUE;
}


int validaVoo(Voo v) {
	if (validaIDVoo(v.id) == FALSE)
		printf("invalid flight code\n");
	else if (encontraVooData(v.id, v.data) != NAO_EXISTE)
		printf("flight already exists\n");
	else if (encontraAeroporto(v.partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.partida);
	else if (encontraAeroporto(v.chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", v.chegada);
	else if (_numVoos == MAX_NUM_VOOS)
		printf("too many flihts\n");
	else if (validaData(v.data) == FALSE)
		printf("invalid date\n");
	else if (validaHora(v.duracao) == FALSE)
		printf("invalid duration\n");
	else if (v.capacidade < 10)
		printf("invalid capacity\n");
	else
		return TRUE;
	return FALSE;
}


void criaVoo(Voo v) {
	strcpy(_voos[_numVoos].id, v.id);
	strcpy(_voos[_numVoos].partida, v.partida);
	strcpy(_voos[_numVoos].chegada, v.chegada);
	_voos[_numVoos].data = v.data;
	_voos[_numVoos].hora = v.hora;
	_voos[_numVoos].duracao = v.duracao;
	_voos[_numVoos].capacidade = v.capacidade;
	_voos[_numVoos].vagas = v.capacidade;
	_voos[_numVoos].horaPartida =
		converteDataHoraNum(_voos[_numVoos].data,
				    _voos[_numVoos].hora);
	_voos[_numVoos].horaChegada =
		_voos[_numVoos].horaPartida +
		converteHoraNum(_voos[_numVoos].duracao);
	_numVoos++;
}


void adicionaListaVoos() {
	Voo v;
	int i;

	if (leProximaPalavra(v.id)) {
		for (i = 0; i < _numVoos; i++)
			mostraVoo(i);
		return;
	} else {
		leProximaPalavra(v.partida);
		leProximaPalavra(v.chegada);
		v.data = leData();
		v.hora = leHora();
		v.duracao = leHora();
		scanf("%d", &v.capacidade);
		leAteFimDeLinha();
	}

	if (validaVoo(v))
		criaVoo(v);
}


int cmpVoosPartida(int a, int b) {
	return (_voos[a].horaPartida > _voos[b].horaPartida);
}


int cmpVoosChegada(int a, int b) {
	return (_voos[a].horaChegada > _voos[b].horaChegada);
}


void listaVoosPartida() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char partida[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(partida);

	if (encontraAeroporto(partida) == NAO_EXISTE)
		printf("%s: no such airport ID\n", partida);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].partida, partida))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosPartida);

	for (i = 0; i < n; i++)
		mostraVooPartida(indexVoos[i]);
}


void listaVoosChegada() {
	int indexVoos[MAX_NUM_VOOS], i, n = 0;
	char chegada[MAX_CODIGO_AEROPORTO];

	lePalavraAteFimDeLinha(chegada);

	if (encontraAeroporto(chegada) == NAO_EXISTE)
		printf("%s: no such airport ID\n", chegada);

	for (i = 0; i < _numVoos; i++) {
		if (!strcmp(_voos[i].chegada, chegada))
			indexVoos[n++] = i;
	}

	bubbleSort(indexVoos, n, cmpVoosChegada);

	for (i = 0; i < n; i++)
		mostraVooChegada(indexVoos[i]);
}


void alteraData() {
	Data d;

	d = leData();
	leAteFimDeLinha();
	if (validaData(d) == FALSE)
		printf("invalid date\n");
	else {
		_hoje = d;
		mostraData(_hoje);
		printf("\n");
	}
}


/* Função final */

void limpaTudo() {
	Reserva *reserva;

	for(reserva = _reservas; reserva != NULL; reserva = reserva->prox){
		free(reserva->id);
		free(reserva);
	}
}


/* Funções Reservas */

int encontraReserva(char id[]) {
	Reserva *reserva;
	for(reserva = _reservas; reserva != NULL; reserva = reserva->prox)
		if(strcmp(reserva->id, id) == 0)
			return TRUE;
	return NAO_EXISTE;
}


int validaIDReserva(char *id) {
	int i = 0, l = strlen(id);
	if(l < 10) return FALSE;
	for (i = 0; i < l; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z') && !(id[i] >= '0' && id[i] <= '9'))
			return FALSE;
	return TRUE;
}


int validaReserva(Reserva *r, char idVoo[], Data data) {
	int indexVoo;
	if (validaIDReserva(r->id) == FALSE)
		printf("invalid reservation code\n");
	else if ((indexVoo = encontraVoo(idVoo)) == NAO_EXISTE)
		printf("%s: flight does not exist\n", idVoo);
	else if (encontraReserva(r->id) == TRUE)
		printf("%s: flight reservation already used\n", r->id);
	else if (_voos[indexVoo].vagas < r->numPassageiros)
		printf("too many reservations\n");
	else if (validaData(data) == FALSE)
		printf("invalid date\n");
	else if (r->numPassageiros <= 0)
		printf("invalid passenger number\n");
	else
		return indexVoo;
	return NAO_EXISTE;
}


void mostraReserva(Reserva *r) {
	printf("%s %d\n", r->id, r->numPassageiros);
}


void mostraReservas(char id[], Data data) {
	Reserva *reserva;

	for(reserva = _reservas; reserva != NULL; reserva = reserva->prox)
		if(!strcmp(reserva->voo->id, id) && cmpData(reserva->voo->data, data))
			mostraReserva(reserva);

	if (validaData(data) == FALSE)
		printf("invalid date\n");
}


void criaReserva(Reserva *r) {
	Reserva *reserva;

	if(_reservas == NULL){
		_reservas = r;
		r->prox = NULL;
		return;
	}
	if(strcmp(_reservas->id, r->id) > 0){
		r->prox = _reservas;
		_reservas = r;
		return;
	}
	for(reserva = _reservas; reserva->prox != NULL; reserva = reserva->prox){
		if(strcmp(reserva->id, r->id) < 0 &&\
		   strcmp(r->id, reserva->prox->id) < 0){
			r->prox = reserva->prox;
			reserva->prox = r;
			return;
		}
	}
	reserva->prox = r;
	r->prox = NULL;
}


int leIDReserva(Reserva *r) {
	char c = getchar();
	int i = 1;
	if((r->id = (char *) malloc(sizeof(char))) == NULL){
		printf("No memory\n");
		limpaTudo();
		exit(0);
	}
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != ' ' && c != '\t' && c != '\n') {
		r->id[i-1] = c;
		c = getchar();
		if((r->id = (char *) realloc(r->id, ++i)) == NULL){
			printf("No memory\n");
			limpaTudo();
			exit(0);
		}
	}
	r->id[i-1] = '\0';
	return (c == '\n');
}


char *leID() {
	char c = getchar(), *s;
	int i = 1;
	if((s = (char *) malloc(sizeof(char))) == NULL){
		printf("No memory\n");
		limpaTudo();
		exit(0);
	}
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != ' ' && c != '\t' && c != '\n') {
		s[i-1] = c;
		c = getchar();
		if((s = (char *) realloc(s, ++i)) == NULL){
			printf("No memory\n");
			limpaTudo();
			exit(0);
		}
	}
	s[i-1] = '\0';
	return s;
}


void adicionaListaReservas() {
	Reserva *r = (Reserva *) malloc(sizeof(Reserva));
	int indexVoo;
	char idVoo[MAX_CODIGO_VOO];
	Data data;
	if(r == NULL){
		printf("No memory\n");
		limpaTudo();
		exit(0);
	}

	leProximaPalavra(idVoo);
	data = leData();
	
	if (leIDReserva(r)) {
		mostraReservas(idVoo, data);
		free(r->id);
		free(r);
		return;
	} else
		scanf("%d", &(r->numPassageiros));
	leAteFimDeLinha();
	if ((indexVoo = validaReserva(r, idVoo, data)) != NAO_EXISTE) {
		_voos[indexVoo].vagas -= r->numPassageiros;
		r->voo = &(_voos[indexVoo]);
		criaReserva(r);
	}
}


void eliminaReservasVoo(char id[]) {
	Reserva *reserva1 = _reservas, *reserva2 = _reservas;
	if(_reservas == NULL) return;
	while(reserva1 != NULL && strcmp(reserva1->voo->id, id) == 0){
		_reservas = reserva1->prox;
		free(reserva1->id);
		free(reserva1);
		reserva1 = _reservas;	
	}
	if(reserva1 == NULL) return;
	for(reserva1 = _reservas->prox; reserva1 != NULL; reserva1 = reserva1->prox){
		if(strcmp(reserva1->voo->id, id) == 0){
			reserva2->prox = reserva1->prox;
			free(reserva1->id);
			free(reserva1);
			continue;
		}
		reserva2 = reserva1;
	}
}


int eliminaVoo(char *id) {
	int i = 0;

	if(strcmp(_voos[_numVoos-1].id, id) == 0){
		_numVoos--;
		eliminaReservasVoo(id);
		return TRUE;
	}
	for(i = 0; i < _numVoos - 1; i++)
		if(strcmp(_voos[i].id, id) == 0){
			_voos[i] = _voos[--_numVoos];
			eliminaReservasVoo(id);
			return TRUE;
		}

	return FALSE;
}


int eliminaReserva(char *id) {
	Reserva *reserva1 = _reservas, *reserva2 = _reservas;
	if(_reservas == NULL) return FALSE;
	if(strcmp(reserva1->id, id) == 0){
		_reservas = reserva1->prox;
		free(reserva1->id);
		free(reserva1);
		return TRUE;
	}
	for(reserva1 = _reservas->prox; reserva1 != NULL; reserva1 = reserva1->prox){
		if(strcmp(reserva1->id, id) == 0){
			reserva2->prox = reserva1->prox;
			free(reserva1->id);
			free(reserva1);
			return TRUE;
		}
		reserva2 = reserva1;
	}

	return FALSE;
}


void eliminaVooReserva() {
	char *id = leID();

	if(eliminaVoo(id)){
		free(id);
		return;
	}
	if(!eliminaReserva(id)) printf("not found\n");
	free(id);
}


/* Função main */

int main() {
	int c;

	while ((c = getchar()) != EOF) {
		switch (c) {
		case 'q': limpaTudo();
			return 0;
		case 'a': adicionaAeroporto();
			break;
		case 'l': listaAeroportos();
			break;
		case 'v': adicionaListaVoos();
			break;
		case 'p': listaVoosPartida();
			break;
		case 'c': listaVoosChegada();
			break;
		case 't': alteraData();
			break;
		case 'r': adicionaListaReservas();
			break;
		case 'e': eliminaVooReserva();
			break;
		default: printf("Invalid comand: %c\n", c);
		}
	}

	return 0;
}
