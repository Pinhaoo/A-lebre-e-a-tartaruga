#include "lab.h"
#include "baralho.h"
#include "jogador.h"

void espera() {
	gotoxy(0, 22);	system("pause");
}

int initBetBaralho(baralho* myB) {
	if (myB->cartas == NULL) {
		createBaralho(myB, 5);
		if (myB->cartas == NULL) {
			return ALLOCATION_ERROR;
		}
	}
	if (myB->size != 0) { return DECKNOTEMPTY; }

	int i = 0;
	myB->cartas[myB->size] = 'L'; myB->size++;
	myB->cartas[myB->size] = 'T'; myB->size++;
	myB->cartas[myB->size] = 'W'; myB->size++;
	myB->cartas[myB->size] = 'R'; myB->size++;
	myB->cartas[myB->size] = 'C'; myB->size++;

	return NOERROR;
}
int initBaralhoPrincipal(baralho* myB) {
	if (myB->cartas == NULL) {
		createBaralho(myB, 81);
		if (myB->cartas == NULL) {
			return ALLOCATION_ERROR;
		}
	}
	if (myB->size != 0) { return DECKNOTEMPTY; }

	int i = 0;
	//L, T, W, w, R, C
	for (i = 0; i < myB->maxSize; i++) {
		if (i < 18) { myB->cartas[i] = 'L'; }
		if (i >= 18 && i < 35) { myB->cartas[i] = 'T'; }
		if (i >= 35 && i < 48) { myB->cartas[i] = 'W'; }
		if (i >= 48 && i < 51) { myB->cartas[i] = 'w'; }
		if (i >= 51 && i < 66) { myB->cartas[i] = 'R'; }
		if (i >= 66) { myB->cartas[i] = 'C'; }
	}
	myB->size = myB->maxSize;
	return NOERROR;
}

void jogoNovo() {
	system("cls");
	
	//Inicialização
	baralho mainDeck, betDeck, discardDeck, playDeck;
	createBaralho(&mainDeck, 81);
	initBaralhoPrincipal(&mainDeck);	//inicializar o baralho de 81 cartas
	createBaralho(&betDeck, 5);
	initBetBaralho(&betDeck);			//inicializar o baralho de 5 cartas
	createBaralho(&discardDeck, 81);	//não precisa de 81...
	createBaralho(&playDeck, 8);		//baralho de mesa; para jogar as cartas

	jogador players[2];
	int i = 0;
	for ( i = 0; i < 2; i++){
		createJogador(&players[i]);
		initJogador(&players[i], &mainDeck, &betDeck); // inicializa a mão e as apostas
	}

	//jogo :D



	// salva tudo
	FILE* fp = NULL;
	fopen_s(&fp, "game000.txt", "w");
	if (fp) {
		fprintf(fp, "mainDeck:");
		saveBaralho(fp, mainDeck);
		fprintf(fp, "\nbetDeck:");
		saveBaralho(fp, betDeck);
		fprintf(fp, "\ndiscardDeck:");
		saveBaralho(fp, discardDeck);
		fprintf(fp, "\nplayDeck:");
		saveBaralho(fp, playDeck);
		for (i = 0; i < 2; i++) {
			fprintf(fp, "\nplayer%d:", i);
			saveJogador(fp, players[i]);
		}
		fclose(fp);
	}

	//free all allocated memory
	freeBaralho(&mainDeck);
	freeBaralho(&betDeck);
	freeBaralho(&discardDeck);
	freeBaralho(&playDeck);
	for (i = 0; i < 2; i++) {
		freeJogador(&players[i]);
	}
}
void carregarJogo() {

	char aux[20] = { '\0' };
	int i = 0;
	//Inicialização
	baralho mainDeck, betDeck, discardDeck, playDeck;
	createBaralho(&mainDeck, 1);
	createBaralho(&betDeck, 1);
	createBaralho(&discardDeck, 1);
	createBaralho(&playDeck, 1);

	jogador players[2];
	for (i = 0; i < 2; i++) {
		createJogador(&players[i]);
	}

	// leitura de ficheiro
	FILE* fp = NULL;
	fopen_s(&fp, "game000.txt", "r");
	if (fp) {
		fscanf_s(fp, " %[^:]:", aux, sizeof(aux) - 1); //lê "mainDeck:"
		readBaralho(fp, &mainDeck);
		fscanf_s(fp, " %[^:]:", aux, sizeof(aux) - 1); //lê "betDeck:"
		readBaralho(fp, &betDeck);
		fscanf_s(fp, " %[^:]:", aux, sizeof(aux) - 1); //lê "discardDeck:"
		readBaralho(fp, &discardDeck);
		fscanf_s(fp, " %[^:]:", aux, sizeof(aux) - 1); //lê "playDeck:"
		readBaralho(fp, &playDeck);
		for (i = 0; i < 2; i++) {
			fscanf_s(fp, " %[^:]:", aux, sizeof(aux) - 1); //lê "player#:"
			readJogador(fp, &players[i]);
		}
		fclose(fp);
	}

	//jogo :D


	//print all:  just to check
	system("cls");
	gotoxy(2, 3);
	printf("mainDeck:");
	printBaralho(mainDeck);
	printf("\nbetDeck:");
	printBaralho(betDeck);
	printf("\ndiscardDeck:");
	printBaralho(discardDeck);
	printf("\nplayDeck:");
	printBaralho(playDeck);
	for (i = 0; i < 2; i++) {
		printf("\nplayer%d:", i);
		printJogador(players[i]);
	}

	//free all allocated memory
	freeBaralho(&mainDeck);
	freeBaralho(&betDeck);
	freeBaralho(&discardDeck);
	freeBaralho(&playDeck);
	for (i = 0; i < 2; i++) {
		freeJogador(&players[i]);
	}
}
char menu() {
	setlocale(LC_ALL, ""); //Escrever carateres especiais
	char opt = '0';
	do{
		system("cls");		//limpa o ecrã
		gotoxy(10, 8);	printf("Menu");

		gotoxy(10, 10);	printf("A - Jogar uma Partida nova");
		gotoxy(10, 11);	printf("B - Jogar uma Partida gravada");
		gotoxy(10, 12);	printf("C - Regras do jogo");
		gotoxy(10, 13);	printf("D - Sair");

		gotoxy(10, 15);	printf("Escolha uma opção: "); 
		(void)scanf("%c", &opt);
		opt = toupper(opt);
	} while (opt != 'A'&& opt != 'B'&& opt != 'C'&& opt != 'D');
	setlocale(LC_ALL, "C");		// retorna ao modo de consola
	return opt;
}
void paginainicial() {
	system("cls");

	gotoxy(10, 10);	printf("A Lebre e a Tartaruga");
	gotoxy(10, 12);	printf("by Daniel 2023_24");
	espera();
}
int main() {

	initRandom();
	setFullScreen(true);
	setColor(MY_COLOR_CYAN, MY_COLOR_GRAY);

	paginainicial();

	char opt = '0';

	do{
		opt = menu();
		switch (opt){
		case 'A':
			jogoNovo();
			break;
		case 'B':
			printf("\tAqui vou carregar o jogo");
			carregarJogo();
			break;
		case 'C':
			printf("\tFalta imprimir as regras");
			break;
		case 'D':
			printf("\tSair");
			break;
		default:
			printf("\tERRO");
			break;
		}
		espera();
	} while (opt != 'D');
	

	//Libertar memória

	return 0; // no error
}