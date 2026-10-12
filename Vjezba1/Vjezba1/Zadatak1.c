#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct _Student {
	char ime[30];
	char prezime[30];
	int bodovi;
}stud;

int main() {
	FILE* dat = fopen("popis.txt", "r");
	if (!dat) {
		printf("Greska");
		return -1;
	}

	int brojac = 0;
	char buffer[100];

	while (fgets(buffer, sizeof(buffer), dat) != NULL){
		brojac++;
	}

	rewind(dat);  //vracamo pokazivac na pocetak da mozemo opet citat podatke

	stud* studenti = NULL;

	studenti = (stud*)malloc(brojac * sizeof(stud)); 

	if (!studenti) {
		printf("Greska pri alokaciji memorije!\n");
		fclose(dat);
		return -1;
	}

	int br = 0;
	
	while (br < brojac && fscanf(dat, " %s %s %d", studenti[br].ime, studenti[br].prezime, &studenti[br].bodovi) == 3) {
		br++;  //koristimo scanf u petlji da provjerimo vraca li 3 podatka uspjesno procitana
	}

	fclose(dat);

	if (br == 0) {
		printf("Datoteka je prazna ili nema ispravnih zapisa.\n");
		free(studenti);
		return 0;
	}

	//pronalazenje maks br bodova
	int max_bodova = studenti[0].bodovi;
	for (int i = 1; i < br; i++) {
		if (studenti[i].bodovi > max_bodova) {
			max_bodova = studenti[i].bodovi;
		}
	}

	//zastita da nebi dijelili s nula
	if (max_bodova == 0) {
		max_bodova = 1;
	}

	printf("\n%-15s %-15s %-15s %-15s\n", "Ime", "Prezime", "Abs. bodovi", "Rel. bodovi");
	printf("------------------------------------------------------------------\n");

	for (int i = 0; i < br; i++) {
		double relativni_bodovi = ((double)studenti[i].bodovi / max_bodova) * 100.0; 
		printf("%-15s %-15s %-15d %-15.2f%%\n", studenti[i].ime, studenti[i].prezime, studenti[i].bodovi, relativni_bodovi);
	}

	free(studenti);  //oslobadanje memorije
	return 0;
}