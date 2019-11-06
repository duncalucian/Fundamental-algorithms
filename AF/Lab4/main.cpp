#include <stdlib.h>
#include <stdio.h>
#include "Profiler.h"
#define MAX_SIZE 12000
#define LEFT(i) 2*i
#define RIGHT(i) 2*i+1
Profiler p("demo");

void swap(int *xp, int *yp)
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void print(int *arr,int n) {
	for (int i = 0; i < n; ++i)
		printf("%d ", arr[i]);
	printf("\n");
}


int partition(int *arr, int l, int r) {
	int x = arr[r];
	int i = l - 1;
	Operation quickSort_SUM = p.createOperation("QuickSort", arr[0]);
	for (int j = l; j <= r - 1; ++j) {
		quickSort_SUM.count();
		if (arr[j] <= x) {
			++i;
			swap(&arr[j], &arr[i]);
			quickSort_SUM.count(3);
		}	
	}
	quickSort_SUM.count();
	swap(&arr[i + 1], &arr[r]);
	return i + 1;
}
int randomPartition(int *arr, int l, int r) {
	srand(time(0));
	int i = rand() % (r - l + 1) + l;

	swap(&arr[r], &arr[i]);

	return partition(arr, l, r);
}

void quickSort(int (*f) (int *,int, int), int *arr, int l, int r) {
	if (l < r) {
		int pivot = f(arr, l, r);
		quickSort(f,arr, l, pivot - 1);
		quickSort(f,arr, pivot + 1, r);
	}
}
void bestCaseQuickSort(int *arr, int l, int r) {
	if (l < r) {
		int mid = (l + r) / 2;
		swap(&arr[mid], &arr[r]);
		int pivot = partition(arr, l, r);
		bestCaseQuickSort(arr, l, pivot - 1);
		bestCaseQuickSort(arr, pivot + 1, r);
	}
}

//Heapsort
void maxHeapify(int *arr, int i, int n) {
	Operation mHeap_SUM = p.createOperation("HeapSort", n);

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

//Random quicksort


int randomizedSelect(int *arr, int l, int r, int i) {
	if (l == r)
		return arr[l];
	int q = randomPartition(arr, l, r);
	int k = q - l + 1 ;
	if (i == k)
		return arr[q];
	else if (i < k)
		return randomizedSelect(arr, l, q - 1, i);
	else
		return randomizedSelect(arr, q + 1, r, i - k);

}
void genAverage(int(*f) (int *, int, int)) {
	int *x = (int*)malloc(MAX_SIZE * 4);
	int *y = (int*)malloc(MAX_SIZE * 4);
	for (int i = 1; i <= 5; ++i) {
		for (int j = 100; j <= 10000; j += 100) {
			FillRandomArray(x, j+1);
			x[0] = j;
			memcpy(y, x, j*4 + 4);
			quickSort(f,x, 1, j);
			heapSort(y);
		}
	}
	p.divideValues("QuickSort", 5);
	p.divideValues("HeapSort", 5);
	p.createGroup("Sorts", "QuickSort", "HeapSort");
}
void quickCases() {
	int *x = (int*)malloc(MAX_SIZE * 4);

	for (int j = 100; j <= 10000; j += 100) {
			FillRandomArray(x, j + 1, 1, j+1, true, 1);
			x[0] = j;
			bestCaseQuickSort(x, 1, j);
			
	}
	p.reset("worst");
	for ( int j = 100; j <= 10000; j += 100) {
		FillRandomArray(x, j + 1, 1, 30000, true, 1);
		x[0] = j;
		quickSort(partition, x, 1, j);  //WORST CASE

	}
}

int main() {
	int arr[] = { 5,1,2,6,3 };
	for (int i = 1; i <= 5;i++) {
		int n = randomizedSelect(arr, 0, 4,i);
		printf("Elementul minim %d este: %d\n", i, n);
	}
	
	genAverage(partition);
	p.reset("Random partitioning");
	genAverage(randomPartition);
	p.reset("Best") ;
	quickCases();
	p.showReport();
	
	}