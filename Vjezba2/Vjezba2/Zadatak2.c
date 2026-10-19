/*2. Definirati strukturu osoba (ime, prezime, godina roðenja) i napisati program koji:
A. dinamièki dodaje novi element na poèetak liste,
B. ispisuje listu,
C. dinamièki dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše odreðeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Person;
typedef struct Person* Position;               //Pokazivac na struktur Person (struct Person*)

typedef struct Person {
	char name[50];
	char surname[50];
	int birthYear;
	Position next;                //Pokazivac koji sadrzi memorijsku adresu sljedeceg elementa u listi
}Person;

Person* insert();                                    //F-ja za unos osoba
int Print(Person*);                                  //F-ja za ispis liste
Person* Start(Person*, Person*);                     //F-ja za dodavanje na pocetak
Person* End(Person*, Person*);                       //F-ja za dodavanje na kraj
Person* Find(Person*, const char*);                  //F-ja za pronalazenje osobe
int sameString(const char*, const char*);            //F-ja za usporedbu dvaju stringova
Person* deleteSurname(Person*, const char*);         //F-ja za brisanje osobe
Person* clean(Person*);                              //F-ja za ciscenje memorije

int main() {
	Person Head = { .name = "",.surname = "",.birthYear = 0, .next = NULL };    //Kreiramo pocetni cvor nazvan Head. Njegov next pokazuje na prvi stvarni element u listi 
	char choice;
	int pozicija;
	Person* nova = NULL;

	do {  //Petlja za unos podataka
		printf("\nunos nove osobe:\n");
		nova = insert();  // 1. stvaranje novog cvora
		if (nova != NULL) {
			printf("Gdje zelite dodati osobu? (1 - na pocetak, 2 - na kraj): ");
			scanf("%d", &pozicija);
			if (pozicija == 1) {
				Head.next = Start(Head.next, nova);    //Dodavanje na pocetak liste
			}
			else {     //Dodavanje na kraj liste
				if (Head.next == NULL)
					Head.next = nova;
				else
					End(Head.next, nova);
			}
		}
		printf("\nzelite li unijeti jos osoba? (d-da/n-ne): ");
		scanf(" %c", &choice);
	} while (choice == 'd');

	char findSurname[50];    //Pretraga
	printf("\nUpisite prezime koje zelite naci: ");
	scanf(" %s", findSurname);
	Person* Found = Find(Head.next, findSurname);
	if (Found)
		printf("\ntrazena osoba je: %s %s %d", Found->name, Found->surname, Found->birthYear);
	else
		printf("\nosoba nije pronadena");

	char deleSurname[50];   //Brisanje
	printf("\nupisite prezime koje zelite izbrisati: ");
	scanf(" %s", deleSurname);
	Head.next = deleteSurname(Head.next, deleSurname);

	Print(Head.next);    //Ispis preostale liste

	Head.next = clean(Head.next);    //Ciscenje memorije

	return 0;
}

Person* insert() {
	Person* new = (Person*)malloc(sizeof(Person));   //Alokacija memorije
	if (!new) {
		printf("\ngreska pri alokaciji memorije.");
		return NULL;
	}
	printf("\nunesi ime: ");
	scanf(" %s", new->name);
	printf("\nunesi prezime: ");
	scanf(" %s", new->surname);
	printf("\nunesi godinu rodenja: ");
	scanf("%d", &new->birthYear);
	new->next = NULL;
	return new;           //Vraca pokazivac na stvorenu osobu
}

int Print(Person* head) {     
	if (head == NULL) {
		printf("\nLista je prazna.");
		return -1;
	}
	printf("\nLista osoba: ");
	Person* temp = head;      //Pomocu temp putujemo listom od pocetka do kraja
	while (temp != NULL) {    //Vrti se dok ne dode do kraja liste
		printf("\nIme: %s\nPrezime: %s\n Godina rodenja: %d", temp->name, temp->surname, temp->birthYear);
		temp = temp->next;     //Pomice se na sljedeci cvor
	}
	return 0;
}
Person* Start(Person* head, Person* nP) {
	nP->next = head;        //Novi element pokazuje na dosadasnji prvi element
	return nP;              //Novi element postat ce novi pocetak
}
Person* End(Person* head, Person* nP) {    //Prolazi cijelu listu od pocetka do kraja da pronade zadnji element i onda mu u next upise adresu novog elementa
	Person* temp = head;
	if (head == NULL)
		return nP;
	while (temp->next != NULL)     //Trazimo cvor ciji je next == NULL (zadnji cvor)
		temp = temp->next;
	temp->next = nP;             //Posljednji cvor sada pokazuje na novi element
	return head;
}
int sameString(const char* word1, const char* word2) {
	while (*word1 && *word2) {
		if (*word1 != *word2)
			return 0;         //Ako se znak ne poklapa, nisu isti
		word1++;
		word2++;
	}
	return 1;    //Isti su
}
Person* Find(Person* head, const char* findSurname) {   //Prolazi redom kroz listu, usporeduje prezime svakog cvora sa trazenim prezimenom pomocu sameString. Ako nade poklapanje vraca adresu tog cvora
	while (head != NULL) {
		if (sameString(head->surname, findSurname))
			return head;     //Pronaden cvor, vraca se pokazivac na njega
		head = head->next;
	}
	return NULL;     //Nije pronadeno
}
Person* deleteSurname(Person* head, const char* delSurname) {
	Person* temp = NULL;   //Prethodni element
	Person* anoth = head;  //Trenutni element
	while (anoth != NULL) {
		if (sameString(anoth->surname, delSurname)) {
			if (temp == NULL) {    //Ako brisemo prvi element u listi
				Person* newHead = anoth->next;   //Glava liste se pomice na sljedeci element
				free(anoth);    //Oslobadamo memoriju
				return newHead;
			}
			else {    //Ako brisemo element u sredini ili na kraju
				temp->next = anoth->next;   //Preskacemo cvor koji brisemo (prethodni spajamo na sljedeci)
				free(anoth);     //Oslobadamo memoriju
				return head;
			}
		}
		temp = anoth;    //Pamti prethodni cvor
		anoth = anoth->next;    //Ide na sljedeci
	}
	printf("\nprezime %s nije pronadeno", delSurname);
	return head;
}
Person* clean(Person* head) {
	Person* temp;
	while (head != NULL) {
		temp = head;
		head = head->next;
		free(temp);    //Brise cvor po cvor
	}
	return NULL;
}