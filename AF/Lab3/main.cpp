#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "Profiler.h"
#define LEFT(i) 2*i
#define RIGHT(i) 2*i+1
#define PARENT(i) i/2
#define MAX_SIZE 10000
Profiler p("heaps");

/*	Observatii
	Se poate observa din grafic ca constructia Bottom Up este mai eficienta decat constructia Top Down. In implementare Bottom UP
se reduce numarul de noduri verificate prin considerarea frunzelor ca fiind deja heap uri astfel complexitatea fiind O(n). La 
metoda Top Down complexitatea este in medie O(n logN).
	In worst case se observa mult mai bine diferenta dintre cele 2 tipuri de algoritmi, Bottom up (O(n)) avand un numar mult mai 
mic de operatii fata de top down (O(n logN)). 
	
*/
void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void print(int *arr) {
	for (int i = 1; i <= arr[0]; ++i)
		printf("%d ", arr[i]);
	printf("\n");
}



//Bottom UP
void maxHeapify(int *arr, int i, int n) {
	Operation mHeap_SUM = p.createOperation("BottomUp_SUM", n);

	int l = LEFT(i);
	int r = RIGHT(i);
	int max;
	
	mHeap_SUM.count(1);
	if (l <= arr[0] && arr[l] > arr[i])
		max = l;
	else max = i;

	mHeap_SUM.count(1);
	if (r <= arr[0] && arr[r] > arr[max])
		max = r;

	if (max != i) {
		mHeap_SUM.count(3);
		swap(&arr[i], &arr[max]);
		maxHeapify(arr, max, n);
	}
	
}
void maxHeapBottomUp(int *arr) {
	for (int i = arr[0] / 2; i >= 1; --i) {
		maxHeapify(arr, i, arr[0]);
	}
}

//TOP_DOWN
void heapIncreaseKey(int *arr, int i, int key, int n) {
	Operation mHeapInsert_SUM = p.createOperation("TopDown_SUM", n);

	arr[i] = key;
	mHeapInsert_SUM.count(2);
	while (i > 1 && arr[PARENT(i)] < arr[i]) {
		mHeapInsert_SUM.count(3);
		swap(&arr[i], &arr[PARENT(i)]);
		i = PARENT(i);
	}
}

void maxHeapInsert(int *arr, int key, int n) {
	++arr[0];
	arr[arr[0]] =(int)-INFINITY;
	heapIncreaseKey(arr, arr[0], key, n);
}

void maxHeapTopBottom(int *arr) {
	int n = arr[0]; 
	arr[0] = 1;
	for (int i = 2; i <= n; ++i)	
		maxHeapInsert(arr, arr[i], n);
}

//HeapSort
void heapSort(int *arr) {
	maxHeapBottomUp(arr);
	int n = arr[0];
	for (int i = n; i >= 2; --i) {
		swap(&arr[1], &arr[i]);
		--arr[0];
		maxHeapify(arr, 1, n);
	}
	arr[0] = n;
}
//Average
void generateAverage() {
	int *x = (int*)malloc(MAX_SIZE * 4);
	int *y = (int*)malloc(MAX_SIZE * 4);

	for (int i = 1; i <= 5; ++i) {
		for (int j = 100; j <= MAX_SIZE; j += 100) {
			FillRandomArray(x, j + 1);
			x[0] = j ;
			memcpy(y, x, j * 4);
			maxHeapBottomUp(x);
			maxHeapTopBottom(y);
		}
	}

	p.divideValues("BottomUp_SUM", 5);
	p.divideValues("TopDown_SUM", 5);

	p.createGroup("SUM_av", "TopDown_SUM","BottomUp_SUM");
	free(x);
	free(y);
}

void generateWorst() {

	int *x = (int*)malloc(MAX_SIZE * 4);
	int *y = (int*)malloc(MAX_SIZE * 4);


		for (int j = 100; j <= MAX_SIZE; j += 100) {
			FillRandomArray(x, j+1,1,12000,true,1);
			x[0] = j ;
			memcpy(y, x, j * 4);
			maxHeapBottomUp(x);
			maxHeapTopBottom(y);
		}
	
	p.createGroup("Comparations_worst", "TopDown_Comp", "BottomUp_Comp");
	p.createGroup("Assignments_worst", "TopDown_Assign", "BottomUp_Assign");
	p.createGroup("SUM_worst", "TopDown_SUM", "BottomUp_SUM");
	free(x);
	free(y);
}

void generateSorted() {
	int x[30] = { 14,2,8,14,23,55,15,21,43,23,61,82,34,57,18 };
	print(x);
	heapSort(x);
	print(x);
}

int main() {
	generateAverage();
	p.reset("Worst");
	generateWorst();
	p.showReport();

	generateSorted();
}