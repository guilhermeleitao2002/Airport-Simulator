/*
* Arquivo: proj2.c
* Autor: Guilherme Leitão 99951
* Descrição: Este programa simula um sistema de gestão de aeroportos com várias
* 			 funcionalidades, como registar, listar e excluir novos aeroportos,
* 			 voos e reservas, bem como a gestão de datas. Além disso, também é
* 			 capaz de organizar essas informações em ordem crescente de acordo
* 			 com determinadas datas e horas de partida ou chegada ou por ordem
* 			 alfabética. O programa gere o calendário adequadamente.
*/

#include "myutils.h"
#include "aeroporto.h"
#include "voo.h"
#include "reserva.h"

/* Função main */

int main() {
	char c;

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
		default: /* Faz nada */;
		}
	}

	return 0;
}