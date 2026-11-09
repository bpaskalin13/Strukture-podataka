/*5. Napisati program koji iz datoteke èita postfiks izraz i zatim korištenjem stoga raèuna
rezultat. Stog je potrebno realizirati preko vezane liste.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct _postfix* Position;
typedef struct _postfix {
	double number;
	Position next;
}postfix;

int push(Position, double);
int pop(Position, double*);
double operation(double, double, char);
double calculatePostfix(Position, char*);

int main() {
	postfix Head = { .number = 0, .next = NULL };
	char fileName[] = "zad5.txt";
	double result;

	result = calculatePostfix(&Head, fileName);
	printf("\nRezultat je: %.2lf", result);
	return 0;
}

int push(Position head, double value) {
	Position newEl = (Position)malloc(sizeof(postfix));
	Position curr = head;
	if (!newEl) {
		printf("\ngreska pri alokaciji memorije.");
		return -1;
	}
	newEl->number = value;
	newEl->next = curr->next;
	curr->next = newEl;
	return 0;
}
int pop(Position head, double* value) {
	Position curr = head;
	Position temp = curr->next;
	if (!temp) {
		printf("\nstog je prazan");
		return -1;
	}
	*value = temp->number;
	curr->next = temp->next;
	free(temp);
	return 0;
}
double operation(double a, double b, char sign) {
	switch (sign) {
	case '+': return a + b;
	case'-':return a - b;
	case'*':return a * b;
	case'/':
		if (b == 0) {
			printf("\ndijeli se sa nulom");
			exit(1);
		}
		else
			return a / b;
	default:
		printf("\nnepoznat operator: %c", sign);
		exit(1);
	}
}
double calculatePostfix(Position head, char* fileName) {
	FILE* fp = fopen(fileName, "r");
	if (!fp) {
		printf("\ndatoteka se ne moze otvorit");
		return -1;
	}
	char buffer[1024];
	if (!fgets(buffer, sizeof(buffer), fp)) {
		printf("\ngreska pri citanju datoteke");
		fclose(fp);
		return -1;
	}
	fclose(fp);
	double a, b;
	char token;
	for (int i = 0; buffer[i] != '\0'; i++) {
		token = buffer[i];
		if (token >= '0' && token <= '9')
			push(head, token - '0');
		else if (token == '+' || token == '-' || token == '*' || token == '/') {
			if (pop(head, &b) == -1 || pop(head, &a) == -1) {
				printf("\nnedovoljno operanada za %c", token);
				exit(1);
			}
			push(head, operation(a, b, token));
		}
	}
	if (head->next && head->next->next == NULL) {
		double result = head->next->number;
		free(head->next);
		head->next = NULL;
		return result;
	}
	else {
		printf("\nkrivi postfix izraz");
		return -1;
	}
}
