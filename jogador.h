#pragma once
#ifndef _JOGADORH_		//protectes from multiple including...
#define _JOGADORH_		//... needs an #endif at bottom

#include "baralho.h"

#define BOT		'B'
#define HUMAN	'H'
#define UNDEF	'U'

#define NOTVALID	-71

typedef struct JOGADOR {
	char nome[20+1];
	baralho mao,
		apostas;
	char tipo;
}jogador;

void createJogador(jogador* jog) {

	strcpy_s(jog->nome, sizeof(jog->nome), "");
	//inicializar mao
	createBaralho(&(jog->mao), 7);
	//inicializar apostas
	createBaralho(&(jog->apostas), 3);

	jog->tipo = UNDEF;
}

void freeJogador(jogador* jog) {
	freeBaralho(&(jog->mao));
	freeBaralho(&(jog->apostas));
	strcpy(jog->nome, "");
	jog->tipo = 0;
}
void printJogador(jogador jog) {
	printf("\n"); printf("tipo: %c - nome: %s", jog.tipo, jog.nome);
	printf("\n mao: ");		printBaralho(jog.mao);
	printf("\n apostas: ");	printBaralho(jog.apostas);
}
void saveJogador(FILE*fp, jogador jog) {
	fprintf(fp, "(%c,%s)", jog.tipo, jog.nome);
	saveBaralho(fp, jog.mao);
	saveBaralho(fp, jog.apostas);
}
void readJogador(FILE* fp, jogador* jog) {
	(void)fscanf_s(fp, "(%c,", &(jog->tipo), 1);
	(void)fscanf_s(fp, "%[^)])", jog->nome, sizeof(jog->nome)-1);
	readBaralho(fp, &(jog->mao));
	readBaralho(fp, &(jog->apostas));
}
int initJogador(jogador* jog, baralho* mainDeck, baralho* betDeck) {

	//inicializar tipo
	printf("\nInsira o tipo do jogador{B, H}: ");
	scanf_s(" %c", &(jog->tipo), 1);			//cuidado
	if (jog->tipo != BOT && jog->tipo != HUMAN) { return NOTVALID; }

	//pedir nome
	if (jog->tipo == HUMAN) {
		printf("\nInsira o nome do jogador: ");
		scanf_s(" %[^\n]", jog->nome, sizeof(jog->nome)-1);
	}
	else {
		strcpy_s(jog->nome, sizeof(jog->nome), " Bot ");
	}

	while (jog->mao.size < jog->mao.maxSize) {
		moveCarta(&jog->mao, mainDeck);
	}
	moveCarta(&jog->apostas, betDeck);	
	moveCarta(&jog->apostas, betDeck);			//com apenas 2 jogadores; aposta extra
	
	if (jog->tipo == HUMAN) {
		moveCarta(&jog->apostas, &jog->mao);		//se for um jogador devia escolher
	}
	else {
		moveCarta(&jog->apostas, &jog->mao);
	}

	return NOERROR;
}

#endif
