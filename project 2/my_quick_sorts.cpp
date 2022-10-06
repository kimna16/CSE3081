#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "my_quick_sorts.h"

void swapFunc(void* a, void* b) {
	ELEMENT temp;
	temp = *(ELEMENT*)a;
	*(ELEMENT*)a = *(ELEMENT*)b;
	*(ELEMENT*)b = temp;
}
size_t partition(void* array, size_t elementNum, _Cmpfun* func) {
	//pivot이 가장 왼쪽, 즉 정렬하려는 배열의 첫 원소여야 함. 
	size_t pivot = elementNum - 1;
	ELEMENT* startArray = (ELEMENT*)array;
	for (int i = elementNum - 1; i > 0; i--) {
		if (func(startArray, &startArray[i]) == -1) {
			swapFunc(&startArray[i], &startArray[pivot]);
			pivot--;
		}
	}
	swapFunc(startArray, &startArray[pivot]);
	//printf("pivot: %d\n", pivot);
	//for (int i = 0; i < elementNum; i++) {
	//	printf("%d ", startArray[i].score);
	//}
	//printf("\n");
	return pivot;
}
size_t median_partition(void* array, size_t elementNum, _Cmpfun* func) {
	//상수 개(5개)의 원소를 사용한 median 선택 방식의 pivot strategy
	ELEMENT* startArray = (ELEMENT*)array;
	int firstFive[5];
	for (int i = 0; i < 5; i++) {
		firstFive[i] = startArray[i].score;
	}
	qsort(firstFive, 5, sizeof(int), func);
	int index;
	for (int i = 0; i < 5; i++) {
		if (startArray[i].score == firstFive[2])
			index = i;
	}
	swapFunc(startArray, &startArray[index]);
	return partition(array, elementNum, func);
}
void qsort_orig(void* array, size_t elementNum, size_t elementSize, _Cmpfun* func) {
	size_t pivot;
	ELEMENT* startArray = (ELEMENT*)array;
	if (elementNum > 1) {
		pivot = partition(array, elementNum, func);
		qsort_orig(startArray, pivot, elementSize, func);
		qsort_orig(&startArray[pivot + 1], elementNum - pivot - 1, elementSize, func);
	}
}
void qsort_median_insert(void* array, size_t elementNum, size_t elementSize, _Cmpfun* func) {
	//elementNum이 10보다 같거나 작을 때는 qsort를 사용하지 않고 냅둔 후, insertion sort를 이용함 
	size_t pivot;
	ELEMENT* startArray = (ELEMENT*)array;
	if (elementNum > 10) {
		pivot = median_partition(array, elementNum, func);
		qsort_median_insert(startArray, pivot, elementSize, func);
		qsort_median_insert(&startArray[pivot + 1], elementNum - pivot - 1, elementSize, func);
	}
}
void qsort_median_insert_iter(void* array, size_t elementNum, size_t elementSize, _Cmpfun* func) {
	size_t pivot;
	ELEMENT* startArray = (ELEMENT*)array;
	size_t iter_elementNum = elementNum;

	while (iter_elementNum > 10) {
		pivot = median_partition(startArray, iter_elementNum, func);
		if (pivot < iter_elementNum - pivot - 1) {
			qsort_median_insert_iter(startArray, pivot, elementSize, func);
			iter_elementNum = iter_elementNum - pivot - 1;
			startArray = &startArray[pivot + 1];
		}
		else {
			qsort_median_insert_iter(&startArray[pivot + 1], iter_elementNum - pivot - 1, elementSize, func);
			iter_elementNum = pivot;
		}
	}
}
void isort(void* array, size_t elementNum, size_t elementSize, _Cmpfun* func) {
	//insertion sort
	ELEMENT* startArray = (ELEMENT*)array;
	ELEMENT temp;
	for (int i = 1; i < elementNum; i++) {
		temp = startArray[i];
		int j = i;
		while ((j > 0) && func(&temp, &startArray[j - 1]) == -1) {
			startArray[j] = startArray[j - 1];
			j--;
		}
		startArray[j] = temp;
	}
}