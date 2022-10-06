#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "my_quick_sorts.h"

__int64 start, freq, end;
#define CHECK_STARTING_TIME QueryPerformanceFrequency((LARGE_INTEGER*)&freq); QueryPerformanceCounter((LARGE_INTEGER*)&start)
#define CHECK_ENDING_TIME(a) QueryPerformanceCounter((LARGE_INTEGER*)&end); a = (float)((float)(end - start) / (freq / 1000.0f))
float computing_time;

ELEMENT* array;

int compareFunc(const void* a, const void* b) {
	ELEMENT* element_a = (ELEMENT*)a;
	ELEMENT* element_b = (ELEMENT*)b;

	if (element_a->score == element_b->score)
		return 0;
	else if (element_a->score < element_b->score)
		return -1;
	else return 1;
}

int main() {
	FILE* fp = fopen("HW2_commands.txt", "r");
	if (fp == NULL)
		return 1;
	int funcNum, elementNum, index = 0;
	char ch, inputFile[50], outputFile[50];
	fscanf(fp, "%d%d", &funcNum, &elementNum);
	//printf("%d %d\n", funcNum, elementNum);
	fscanf(fp, "%c", &ch);
	while (1) {
		fscanf(fp, "%c", &ch);
		if (ch == '\n') {
			inputFile[index] = '\0';
			break;
		}
		else
			inputFile[index++] = ch;
	}
	index = 0;
	while (1) {
		fscanf(fp, "%c", &ch);
		if (ch == '\n') {
			outputFile[index] = '\0';
			break;
		}
		else
			outputFile[index++] = ch;
	}
	fclose(fp);

	array = (ELEMENT*)malloc(sizeof(ELEMENT) * elementNum);

	FILE* unsorted = fopen(inputFile, "rb");
	fread(array, sizeof(ELEMENT), elementNum, unsorted);
	fclose(unsorted);

	//TEST begin
	//printf("UNSORTED ARRAY\n");
	//for (int i = 0; i < elementNum; i++) {
	//	printf("%d ", (array + i)->score);
	//}
	//printf("\n\n");
	//TEST end

	if (funcNum == 1) {
		CHECK_STARTING_TIME;
		qsort(array, elementNum, sizeof(ELEMENT), compareFunc);
		CHECK_ENDING_TIME(computing_time);
		printf("***Time taken for sorting array by qsort() method is %.3fms***\n\n", computing_time);
	}
	else if (funcNum == 21) {
		CHECK_STARTING_TIME;
		qsort_orig(array, elementNum, sizeof(ELEMENT), compareFunc);
		CHECK_ENDING_TIME(computing_time);
		printf("***Time taken for sorting array by qsort_orig() method is %.3fms***\n\n", computing_time);
	}
	else if (funcNum == 22) {
		CHECK_STARTING_TIME;
		qsort_median_insert(array, elementNum, sizeof(ELEMENT), compareFunc);
		isort(array, elementNum, sizeof(ELEMENT), compareFunc);
		CHECK_ENDING_TIME(computing_time);
		printf("***Time taken for sorting array by qsort_median_insert() method is %.3fms***\n\n", computing_time);
	}
	else if (funcNum == 23) {
		CHECK_STARTING_TIME;
		qsort_median_insert_iter(array, elementNum, sizeof(ELEMENT), compareFunc);
		isort(array, elementNum, sizeof(ELEMENT), compareFunc);
		CHECK_ENDING_TIME(computing_time);
		printf("***Time taken for sorting array by qsort_median_insert_iter() method is %.3fms***\n\n", computing_time);
	}

	//TEST begin
	//printf("SORTED ARRAY\n");
	//for (int i = 0; i < elementNum; i++) {
	//	printf("%d ", (array + i)->score);
	//}
	//printf("\n\n");
	//TEST end

	FILE* sorted = fopen(outputFile, "wb");
	fwrite(array, sizeof(ELEMENT), elementNum, sorted);
	fclose(sorted);

	free(array); 

	return 0;
}