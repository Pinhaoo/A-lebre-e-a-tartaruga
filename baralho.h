#pragma once
#ifndef _BARALHOH_		//protectes from multiple including...
#define _BARALHOH_		//... needs an #endif at bottom

#define DECKMISSING			-29 // erro: falta um deck (ou mais)
#define DECKNOTEMPTY		-30	// erro: o deck n�o est� vazio
#define EMPTYDECK			-31	// erro: size = 0
#define FULLDECK			-32	// erro: size = maxSize
#define ALLOCATION_ERROR	-33 // erro: falha de aloca��o de mem�ria


#define NOCARD	'x'

//#define NOERROR	0	// general no ERROR code

typedef struct BARALHO {
	char* cartas;	//ponteiro para um conjunto de cartas (baralho)
	int size;		//numero de cartas v�lidas no baralho
	int maxSize;	//numero m�ximo possivel de cartas no baralho
}baralho;

//Cria um baralho vazio de um tamanho qualquer: pode ser usado para: 
// - o baralho principal, (mainDeck)
// - o baralho de descarte,(discardDeck)
// - as cartas jogadas para a mesa, (tableDeck)
// - o baralho de apostas, 	(betDeck)
// - a mao do Jogador, (.handDeck)
// - as apostas do Jogador. (.betDeck)
// 
// � criado vazio, deve ser inicializado com uma fun��o conveniente
int createBaralho(baralho* myB, int maxSize) {
	myB->size = 0;
	myB->maxSize = maxSize;
	myB->cartas = (char*)calloc((myB->maxSize + 1), sizeof(char));
	if (myB->cartas == NULL) { return ALLOCATION_ERROR; }

	int i = 0;
	for (i = 0; i < myB->maxSize + 1; i++) {
		myB->cartas[i] = NOCARD;
	}
	myB->size = 0;//myB->maxSize; //
	return NOERROR;
}
void freeBaralho(baralho* myB) {
	if (myB->cartas != NULL) {
		free(myB->cartas);
	}
	myB->cartas = NULL;
}
void printBaralho(baralho myB) {
	int i = 0;
	printf("(%d,%d)", myB.size, myB.maxSize);
	printf("(");
	for (i = 0; i < myB.size - 1; i++) {
		printf("%c,", myB.cartas[i]);
	}
	if (myB.size) { printf("%c", myB.cartas[i]); }
	printf(")");
}
void saveBaralho(FILE* fp, baralho myB) {
	//assume-se que o ficheiro vem aberto e � v�lido
	int i = 0;
	fprintf(fp, "(%d,%d)", myB.maxSize, myB.size);
	fprintf(fp, "(");
	for (i = 0; i < myB.maxSize - 1; i++) {
		fprintf(fp, "%c,", myB.cartas[i]);
	}
	fprintf(fp, "%c", myB.cartas[i]);
	fprintf(fp, ")");
}
int readBaralho(FILE* fp, baralho* myB) {
	//assume-se que o ficheiro vem aberto e � v�lido
	//o baralho � criado aqui
	if (myB->cartas != NULL) {
		freeBaralho(myB);
	}
	int i = 0;
	(void)fscanf(fp, "(%d,", &(myB->maxSize));
	if(createBaralho(myB, myB->maxSize) == ALLOCATION_ERROR) { return ALLOCATION_ERROR; } 
	(void)fscanf(fp, "%d)", &(myB->size));
	fgetc(fp); //descarta o '('
	for (i = 0; i < myB->maxSize-1; i++) {
		myB->cartas[i] = fgetc(fp);
		fgetc(fp); //descarta o ','
	}
	myB->cartas[i] = fgetc(fp);
	fgetc(fp); //descarta o ')'
	return NOERROR;
}
//baralha as cartas v�lidas num baralho
int shuffleBaralho(baralho* myB) {
	int i = 0;
	if (myB->size == 0) { return EMPTYDECK; }

	for (i = 0; i < myB->size; i++) {	//s� baralha as cartas v�lidas
		trocaChars(&(myB->cartas[i]), &(myB->cartas[rand() % myB->size]));
	}
	return NOERROR;
}

//passa a �ltima carta do baralho de origem para 
// a �ltima posi��o dispon�vel no baralho de destino
// devolve erros se n�o for poss�vel.
int moveCarta(baralho* dest, baralho* ori) {

	int i = 0;
	if (dest == NULL || ori == NULL) { return DECKMISSING; }
	if (ori->size == 0) { return EMPTYDECK; }
	if (dest->size >= dest->maxSize) { return FULLDECK; }

	dest->cartas[dest->size] = ori->cartas[ori->size - 1]; //copia a carta
	dest->size++;						//incrementa o tamanho do baralho

	ori->cartas[ori->size - 1] = NOCARD;	//invalida a carta copiada
	ori->size--;						//decrementa o tamanho do baralho
	return NOERROR;
}

#endif