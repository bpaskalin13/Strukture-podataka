#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Poly* Position;

typedef struct Poly {
	int coeff;
	int pow;
	Position next;
}polynom;

Position newElement(int coeff, int pow);
int InsertAfter(Position p, Position newPoly);
int sortedInsert(Position Head, int coeff, int pow);
int readFromFile(Position Head, const char* fileName);
int printPoly(Position Head);
int addPolys(Position head1, Position head2, Position resultHead);
int multiplyPolys(Position head1, Position head2, Position resultHead);
int freeMemory(Position Head);

int main() {
	polynom Head = { .coeff = 0, .pow = 0, .next = NULL };//def Head
	polynom headFirst = Head;
	polynom headSecond = Head;
	polynom headSum = Head;
	polynom headProduct = Head;

	if (readFromFile(&headFirst, "poly1.txt") != 0) {  //Otvori prvu datoteku i ucitaj
		printf("Greska pri citanju prve datoteke!\n");
		return 1;
	}
	if (readFromFile(&headSecond, "poly2.txt") != 0) {  //Otvori drugu i ucitaj
		printf("Greska pri citanju druge datoteke!\n");
		freeMemory(&headFirst);
		return 1;
	}

	printf("Prvi polinom:   ");  //Ispis ucitanih polinoma
	printPoly(&headFirst);

	printf("Drugi polinom:   ");
	printPoly(&headSecond);

	addPolys(&headFirst, &headSecond, &headSum);  //Zbrajanje
	printf("Zbroj:     ");
	printPoly(&headSum);

	multiplyPolys(&headFirst, &headSecond, &headProduct);  //Mnozenje
	printf("Umnozak:    ");
	printPoly(&headProduct);

	freeMemory(&headFirst);  //Oslobadanje memorije
	freeMemory(&headSecond);
	freeMemory(&headSum);
	freeMemory(&headProduct);

	return 0;
}

Position newElement(int coeff, int pow) {  //Alokacija novog cvora
	Position newPoly = (Position)malloc(sizeof(polynom));
	if (!newPoly) {
		printf("Greska pri alokaciji memorije!\n");
		return NULL;
	}
	newPoly->coeff = coeff;
	newPoly->pow = pow;
	newPoly->next = NULL;
	return newPoly;
}

int InsertAfter(Position p, Position newPoly) {  //Umetanje nakon nekog elementa
	newPoly->next = p->next;
	p->next = newPoly;
	return 0;
}

int sortedInsert(Position head, int coeff, int pow) {
	if (coeff == 0) return 0;  //Clanovi sa coeff 0 se ne unose

	Position q = head;
	Position newPoly = newElement(coeff, pow);
	if (!newPoly) return 1;

	while (1) {

		if (q->next == NULL) {  //a) Provjera jel lista prazna ili kraj liste
			InsertAfter(q, newPoly);
			break;
		}

		else if (q->next != NULL && newPoly->pow > q->next->pow) {
			InsertAfter(q, newPoly);
			break;
		}

		else if (q->next != NULL && newPoly->pow < q->next->pow) {
			q = q->next;
		}

		else if (q->next != NULL && newPoly->pow == q->next->pow) {  //Ista potencija => zbroji koeficijente
			q->next->coeff += newPoly->coeff;

			free(newPoly);  //Novi cvor vise ne triba jer smo zbrojili u postojeci

			if (q->next->coeff == 0) {
				Position temp = q->next;
				q->next = temp->next;
				free(temp);
			}
			break;
		}
	}
	return 0;
}

int readFromFile(Position head, const char* fileName) {
	FILE* file = fopen(fileName, "r");

	if (!file) {
		return -1;
	}

	int coeff = 0, pow = 0;
	while (fscanf(file, "%d %d", &coeff, &pow) == 2) {
		sortedInsert(head, coeff, pow);  //2) Sortirani unos neiskoristene potencije
	}

	fclose(file);
	return 0;
}

int addPolys(Position head1, Position head2, Position resultHead) {  //Zbrajanje polinoma
	Position p1 = head1->next;
	Position p2 = head2->next;

	while (p1 != NULL) {
		sortedInsert(resultHead, p1->coeff, p1->pow);
		p1 = p1->next;
	}

	while (p2 != NULL) {
		sortedInsert(resultHead, p2->coeff, p2->pow);
		p2 = p2->next;
	}

	return 0;
}

int multiplyPolys(Position head1, Position head2, Position resultHead) {  //Mnozenje polinoma
	Position p1 = head1->next;

	while (p1 != NULL) {
		Position p2 = head2->next;
		while (p2 != NULL) {
			//Mnozenje => prvi element headFirst mnozit sa svakin headSecond
			int newCoeff = p1->coeff * p2->coeff;
			int newPow = p1->pow + p2->pow;

			//Za buduca mnozenja => provjera zbrajanja
			sortedInsert(resultHead, newCoeff, newPow);

			p2 = p2->next;
		}
		p1 = p1->next;
	}
	return 0;
}

int printPoly(Position head) {  //Ispis polinoma
	Position p = head->next;
	while (p) {
		printf(" %dx^%d", p->coeff, p->pow);
		if (p->next && p->next->coeff > 0)
			printf(" + ");
		else if (p->next)
			printf(" ");
		p = p->next;
	}
	printf("\n");
	return 0;
}

int freeMemory(Position head) {  //Oslobadanje memorije
	Position temp = NULL;
	while (head->next != NULL) {
		temp = head->next;
		head->next = temp->next;
		free(temp);
	}
	return 0;
}