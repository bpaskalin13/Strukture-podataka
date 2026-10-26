/*3. Prethodnom zadatku dodati funkcije:
A. dinamièki dodaje novi element iza odreðenog elementa,
B. dinamièki dodaje novi element ispred odreðenog elementa,
C. sortira listu po prezimenima osoba,
D. upisuje listu u datoteku,
E. èita listu iz datoteke.*/

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
Person* insertAfter(Person*, const char*);
Person* insertBefore(Person*, const char*);
Person* sortList(Person*);
int saveInFile(Person*, const char*);
Person* readFromFile(const char*);


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

	char newAfter[50];
	printf("\nIza kojeg prezimena zelite upisati novu osobu?");
	scanf(" %s", newAfter);
	Head.next = insertAfter(Head.next, newAfter);
	Print(Head.next);

	char newBefore[50];
	printf("\nIspred kojeg prezimena zelite upisati novu osobu?");
	scanf(" %s", newBefore);
	Head.next = insertBefore(Head.next, newBefore);
	Print(Head.next);

	Head.next = sortList(Head.next);
	printf("\nLista sortirana po prezimenima je: \n");
	Print(Head.next);

	saveInFile(Head.next, "zadatak3.txt");
	Head.next = clean(Head.next);

	printf("\nCitanje liste iz datoteke: \n");
	Head.next = readFromFile("zadatak3.txt");
	Print(Head.next);

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

Person* insertAfter(Person* head, const char* surname) {
	Person* targeted = Find(head, surname);   //Trazi cvor sa trazenim prezimenom
	if (targeted != NULL) {  //Ako osoba postoji
		Person* newPerson = insert();  //Alocira novu osobu
		newPerson->next = targeted->next;
		targeted->next = newPerson;
	}
	return head;
}

Person* insertBefore(Person* head, const char* surname) {
	Person* targeted = Find(head, surname);
	if (!targeted) {
		printf("\nOsoba nije pronadena!");
		return head;
	}

	Person* newPerson = insert();
	if (targeted == head) {   //Ako je ciljani element prvi u listi
		newPerson->next = head;
		return newPerson;  //Mora postat novi pocetak liste pa vracamo newPerson a ne head
	}

	Person* prev = head;
	while (prev->next != NULL && prev->next != targeted)
		prev = prev->next;
	newPerson->next = targeted;
	prev->next = newPerson;
	return head;
}

Person* sortList(Person* head) {
	int swap;
	Person* end = NULL;
	do {
		swap = 0;
		Person* prev = NULL;
		Person* curr = head;
		while (curr->next != end) {
			if (strcmp(curr->surname, curr->next->surname) > 0) {
				Person* temp = curr->next;
				curr->next = temp->next;
				temp->next = curr;
				if (prev == NULL)
					head = temp;
				else
					prev->next = temp;
				prev = temp;
				swap = 1;
			}
			else {
				prev = curr;
				curr = curr->next;
			}
		}
		end = curr;
	} while (swap);
	return head;
}

int saveInFile(Person* head, const char* fileName) {
	FILE* f = fopen(fileName, "w");  //Otvara datoteku u write
	if (!f) {
		printf("\Greska pri otvaranju datoteke!");
		return -1;
	}
	Person* temp = head;
	while (temp != NULL) {  //Prolazak redom kroz listu dok ne dodemo do kraja
		fprintf(f, "%s %s %d\n", temp->name, temp->surname, temp->birthYear);  //zapis za trenutnu osobu
		temp = temp->next;  //Prelazak na iducu osobu
	}
	fclose(f);
	printf("\nLista je spremljena u datoteku!");
	return 0;
}

Person* readFromFile(const char* fileName) {
	FILE* f = fopen(fileName, "r");  //Otvara datoteku za citanje
	if (!f) {
		printf("\Greska pri otvaranju datoteke!");
		return NULL;
	}
	Person* head = NULL;  //Pamti adresu prvog elementa u novoj listi
	Person* last = NULL;  //Pamti adresu posljednjeg dodanog elementa
	while (1) {
		Person* newPerson = (Person*)malloc(sizeof(Person));
		if (!newPerson) {
			printf("\Greska!");
			fclose(f);
			return head;
		}
		int r = fscanf(f, " %s %s %d\n", newPerson->name, newPerson->surname, &newPerson->birthYear);  //Citanje iz datoteke (r pamti broj uspjesno procitanih vrijednosti)
		if (r != 3) {  //Kad petlja dode do kraja datoteke
			free(newPerson);  //Oslobadanje zadnje osobe jer ne postoji
			break;
		}
		newPerson->next = NULL;  //Novi element je sad zadnji pa pokazuje na NULL
		if (head == NULL) {  //Ako je prva osoba koju ucitavamo
			head = newPerson;
		}
		else {   //Ako lista vec ima elemenata, spaja na zadnji
			last->next = newPerson;
		}
		last = newPerson;  //last pokazuje na novu zadnje dodanu osobu
	}
	fclose(f);
	printf("\nLista je ucitana iz datoteke!");
	return head;
}