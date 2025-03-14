#include "reserva.h"
#include "voo.h"

/* Variáveis Globais */

Reserva *_head = NULL;	/* ponteiro para o inicio da lista de reservas */

/* Função final */

void limpaTudo() {
	Reserva *reserva;

	for(reserva = _head; reserva != NULL; reserva = reserva->prox){
		free(reserva->id);
		free(reserva);
	}
}

/* Funções Reservas */

int encontraReserva(char *id) {
	Reserva *reserva;
	for(reserva = _head; reserva != NULL; reserva = reserva->prox)
		if(strcmp(reserva->id, id) == 0)
			return TRUE;
	return FALSE;
}

int validaIDReserva(char *id) {
	int i;
	for (i = 0; id[i] != '\0'; i++)
		if (!(id[i] >= 'A' && id[i] <= 'Z') && !(id[i] >= '0' && id[i] <= '9'))
			return FALSE;
	if(i < 10) return FALSE;
	return TRUE;
}

int validaReserva(Reserva *r, char idVoo[], Data data) {
	int indexVoo;
	if (!validaIDReserva(r->id))
		printf("invalid reservation code\n");
	else if ((indexVoo = encontraVooData(idVoo, data)) == NAO_EXISTE)
		printf("%s: flight does not exist\n", idVoo);
	else if (encontraReserva(r->id))
		printf("%s: flight reservation already used\n", r->id);
	else if (_voos[indexVoo].vagas < r->numPassageiros)
		printf("too many reservations\n");
	else if (!validaData(data))
		printf("invalid date\n");
	else if (r->numPassageiros < 1)
		printf("invalid passenger number\n");
	else
		return indexVoo;
	return NAO_EXISTE;
}

void mostraReservas(char id[], Data data) {
	Reserva *reserva;

	if (encontraVooData(id, data) == NAO_EXISTE)
		printf("%s: flight does not exist\n", id);
	else if (!validaData(data))
		printf("invalid date\n");
	else
		for(reserva = _head; reserva != NULL; reserva = reserva->prox)
			if(!strcmp(reserva->voo->id, id) &&\
			   cmpData(reserva->voo->data, data)){
				printf("%s %d\n", reserva->id, reserva->numPassageiros);
			}
}

void criaReserva(Reserva *r) {
	Reserva *reserva;

	if(_head == NULL){
		r->prox = NULL;
		_head = r;
		return;
	}
	if(strcmp(_head->id, r->id) > 0){
		r->prox = _head;
		_head = r;
		return;
	}
	for(reserva = _head; reserva->prox != NULL; reserva = reserva->prox)
		if(strcmp(reserva->id, r->id) < 0 &&\
		   strcmp(r->id, reserva->prox->id) < 0){
			r->prox = reserva->prox;
			reserva->prox = r;
			return;
		}
	r->prox = NULL;
	reserva->prox = r;
}

int leIDReserva(Reserva *r) {
	char c = getchar();
	int i = 1;
	if((r->id = (char *) malloc(sizeof(char))) == NULL){
		printf("No memory\n");
		limpaTudo();
		exit(1);
	}
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != ' ' && c != '\t' && c != '\n') {
		r->id[i-1] = c;
		c = getchar();
		if((r->id = (char *) realloc(r->id, ++i)) == NULL){
			printf("No memory\n");
			limpaTudo();
			exit(1);
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
		exit(1);
	}
	while (c == ' ' || c == '\t')
		c = getchar();
	while (c != '\n') {
		s[i-1] = c;
		c = getchar();
		if((s = (char *) realloc(s, ++i)) == NULL){
			printf("No memory\n");
			limpaTudo();
			exit(1);
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
		limpaTudo(_head);
		exit(1);
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
	if ((indexVoo = validaReserva(r, idVoo, data)) != NAO_EXISTE){
		_voos[indexVoo].vagas -= r->numPassageiros;
		r->voo = &(_voos[indexVoo]);
		criaReserva(r);
		return;
	}
	free(r->id);
	free(r);
}

void eliminaReservasVoo(char id[]) {
	Reserva *reserva = _head, *reservaAux;

	while(reserva != NULL && strcmp(reserva->voo->id, id) == 0){
		_head = reserva->prox;
		free(reserva->id);
		free(reserva);
		reserva = _head;
	}
	if(reserva == NULL) return;
	reservaAux = reserva;
	for(reserva = _head->prox; reserva != NULL; reserva = reserva->prox){
		if(strcmp(reserva->voo->id, id) == 0){
			reservaAux->prox = reserva->prox;
			free(reserva->id);
			free(reserva);
			continue;
		}
		reservaAux = reserva;
	}
}

void eliminaVoo(char *id) {
	int i, eliminado = FALSE, j, k;
	while(strcmp(_voos[_numVoos - 1].id, id) == 0 && _voos[_numVoos - 1].horaChegada < converteDataNum(_hoje)*MINUTOS_DIA){
		_numVoos--;
		eliminado = TRUE;
	}
	for(i = 0; i < _numVoos; i++)
		if(strcmp(_voos[i].id, id) == 0 && _voos[i].horaChegada < converteDataNum(_hoje)*MINUTOS_DIA){
			for(j = i, k = i + 1; k < _numVoos; j++, k++)
				_voos[j] = _voos[k];
			_numVoos--;
			i--;
			eliminado = TRUE;
		}

	if(eliminado) eliminaReservasVoo(id);
	else printf("not found\n");
}

void eliminaReserva(char *id) {
	Reserva *reserva = _head, *reservaAux;

	if(reserva == NULL){
		printf("not found\n");
		return;
	}
	if(strcmp(reserva->id, id) == 0){
		reserva->voo->vagas += reserva->numPassageiros;
		_head = reserva->prox;
		free(reserva->id);
		free(reserva);
		return;
	}
	reservaAux = reserva;
	for(reserva = _head->prox;reserva != NULL;reserva = reserva->prox){
		if(strcmp(reserva->id, id) == 0){
			reserva->voo->vagas += reserva->numPassageiros;
			reservaAux->prox = reserva->prox;
			free(reserva->id);
			free(reserva);
			return;
		}
		reservaAux = reserva;
	}
	printf("not found\n");
}

void eliminaVooReserva() {
	char *id = leID();

	if(strlen(id) < 10) eliminaVoo(id);
	else eliminaReserva(id);
	
	free(id);
}