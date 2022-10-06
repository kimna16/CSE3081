#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GOOD 0
#define BAD 1
#define testSize 2000
#define inputArrSize 1000

int dp[inputArrSize][inputArrSize];
int r[inputArrSize];
int l[inputArrSize];
int whatToCheck[testSize];

int inputSize;
char input[100000];
int numr, numl;

void initialize(FILE* inFile, int* intArr, int* num) {
	char ch, arr[2000];
	int idx = 0;
	while (1) {
		ch = fgetc(inFile);
		if (ch == '\n' || ch == EOF) {
			arr[idx++] = '\0';
			idx = 0;
			break;
		}
		else
			arr[idx++] = ch;
	}
	char* c;
	c = strtok(arr, " ");
	int n = 0;
	while (c != NULL) {
		if (n == 0) 
			*num = atoi(c);
		else
			intArr[idx++] = atoi(c);
		c = strtok(NULL, " ");
		n++;
	}
}
int checkPossibility(int currIdx, int idx1, int idx2) {
	if (dp[idx1][idx2] == BAD) return BAD;
	if (currIdx == inputSize) {
		return GOOD;
	}
	if (whatToCheck[currIdx] == r[idx1]) {
		if (!checkPossibility(currIdx + 1, idx1 + 1, idx2)) {
			return dp[idx1][idx2] = GOOD;
		}
		else {
			dp[idx1][idx2] = BAD;
		}
	}
	if (whatToCheck[currIdx] == l[idx2]) {
		if (!checkPossibility(currIdx + 1, idx1, idx2 + 1)) {
			return dp[idx1][idx2] = GOOD;
		}
		else {
			dp[idx1][idx2] = BAD;
		}
	}
	return BAD;
}

int main() {
	FILE* in = fopen("commands_3_2.txt", "r");
	FILE* out = fopen("outputs_3_2.txt", "w");
	FILE* inFile;
	int idx, ch, check = 1, check1, check2, num;
	while (1) {
		idx = 0;
		while (1) {
			ch = fgetc(in);
			if (ch == '\n') {
				input[idx] = '\0';
				idx = 0;
				break;
			}
			else if (ch == EOF) {
				check = 0;
				break;
			}
			input[idx++] = ch;
		}
		printf("%s\n", input);
		inFile = fopen(input, "r");
		initialize(inFile, r, &numr);
		initialize(inFile, l, &numl);
		inputSize = numr + numl;
		initialize(inFile, whatToCheck, &num);
		//printf("%d %d %d\n", numr, numl, num);
		if (num != numr + numl)
			fprintf(out, "0");
		else {
			for (int i = 0; i < 1000; i++) {
				for (int j = 0; j < 1000; j++) {
					dp[i][j] = 0;
				}
			}
			check1 = !checkPossibility(0, 0, 0);
			fprintf(out, "%d", check1);
			printf("%d", check1);
		}	
		initialize(inFile, whatToCheck, &num);
		if (num != numr + numl)
			fprintf(out, "0\n");
		else {
			for (int i = 0; i < 1000; i++) {
				for (int j = 0; j < 1000; j++) {
					dp[i][j] = 0;
				}
			}
			check2 = !checkPossibility(0, 0, 0);
			fprintf(out, "%d\n", check2);
			printf("%d\n", check2);
		}
		fclose(inFile);
		if (check == 0)
			break;
		for (int i = 0; i < numr; i++) {
			r[i] = 0;
		}
		for (int i = 0; i < numl; i++) {
			l[i] = 0;
		}
		for (int i = 0; i < num; i++) {
			whatToCheck[i] = 0;
		}
	}
	fclose(in);
	fclose(out);
	return 0;
}