#include <stdlib.h>
#include <stdio.h>
#include "Profiler.h"
#define LEFT(i) 2*i+1
#define RIGHT(i) 2*i+2
Profiler p("k_lists");

typedef struct Node {
	int key;
	struct Node *next;
}nodus;

#define MAX_SIZE 10000
void swap(nodus *xp, nodus *yp)
{
	nodus temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void print(nodus *arr) {
	while (arr) {
		printf("%d ", arr->key);
		arr = arr->next;
	}
	printf("\n");
}

void printArr(int *arr, int n) {
	for (int i = 0; i < n; ++i)
		printf("%d ", arr[i]);
	printf("\n");
}

void minHeapify(nodus **arr, int i, int n,Operation op) {
	int l = LEFT(i);
	int r = RIGHT(i);
	int min;
	op.count();
	if (l <= n && arr[l]->key < arr[i]->key )
		min = l;
	else min = i;
	op.count();
	if (r <= n && arr[r]->key < arr[min]->key)
		min = r;

	if (min != i) {
		op.count(3);
		swap(arr[i], arr[min]);
		minHeapify(arr, min, n, op);
	}

}

void maxHeapBottomUp(nodus **arr,int n,Operation op) {
	for (int i = n / 2; i >= 0; --i) {
		minHeapify(arr, i, n , op);
	}
}
void generateRandList(nodus **list, int n) {
	int *x = (int *)malloc(n * 4);
	FillRandomArray(x, n, 1, 30000, true, 1);

	nodus *nod = NULL; // !!!!
	for (int i = 0; i < n; ++i) {
		
		if (nod == NULL) {
			nodus *newNode = (nodus*)malloc(sizeof(nodus));
			newNode->key = x[i];
			newNode->next = NULL;
			(*list) = newNode;
			nod = *list;

		}
		else
		{
			nodus *newNode = (nodus*)malloc(sizeof(nodus));
			newNode->key = x[i];
			newNode->next = NULL;
			nod->next = newNode;
			nod = nod->next;
		}
	}
}

int * sortLists(nodus **list, int k,int n, Operation op) {
	int *arr = (int*)malloc(n * 4);
	
	int ind = 0;
	while (k > 1 || (*list)->next != NULL) {
		arr[ind++] = list[0]->key;
		nodus *nod = list[0];
		list[0] = list[0]->next;
		free(nod);
		if (list[0] == 0) {
			k--;
			list[0] = list[k];
		}
		minHeapify(list, 0, k-1, op);
		
	}
	arr[ind] = list[0]->key;
	return arr;
}

void generateList(int k, char * nume) {
	for (int j = 0; j < 5; j++) {
		for (int i = 100; i <= 10000; i += 100) {
			nodus **list = (nodus**)calloc(k + 1, sizeof(nodus*));
			int n = i;
			for (int j = 0; j < k; ++j) {
				int av = n / k;
				if (n % k > 0) {
					av++;
					n--;
				}
				generateRandList(&list[j], av);
			}
			Operation k_5 = p.createOperation(nume, i);
			maxHeapBottomUp(list, k - 1, k_5);
			int *x = sortLists(list, k, i, k_5);

		//	if (IsSorted(x, i))
			//	printf("Sirul este sortat.\n");
			//else
			//	printf("Sirul nu e sortat.\n");

			free(list);

		}
	}
}


void generateListK(char * nume) {
	
		for (int k = 10; k <= 500; k += 10) {
			nodus **list = (nodus**)calloc(k + 1, sizeof(nodus*));
			int n = 10000;
			for (int j = 0; j < k; ++j) {
				int av = n / k;
				if (n % k > 0) {
					av++;
					n--;
				}
				generateRandList(&list[j], av);
			}
			Operation k_5 = p.createOperation(nume, k);
			maxHeapBottomUp(list, k - 1, k_5);
			int *x = sortLists(list, k, 10000, k_5);

			//if (IsSorted(x, 10000))
		//		printf("Sirul este sortat.\n");
			//else
			//	printf("Sirul nu e sortat.\n");

			free(list);

	}
}

void first() {
	generateList(5, "5 Liste");
	generateList(10, "10 Liste");
	generateList(100, "100 Liste");
	p.createGroup("3 liste", "5 Liste", "10 Liste", "100 Liste");
	p.divideValues("5 Liste", 5);
	p.divideValues("10 Liste", 5);
	p.divideValues("100 Liste", 5);
	
}

void second() {
	generateListK("luci");
	p.showReport();
}

void exemple() {
	nodus **list = (nodus**)calloc(3, sizeof(nodus*));
	generateRandList(&list[0], 4);
	generateRandList(&list[1], 4);
	print(list[0]);
	print(list[1]);
	Operation op = p.createOperation("nume", 4);
	maxHeapBottomUp(list, 1, op);
	int *arr = sortLists(list, 2, 8, op);
	printArr(arr, 8);
}
nodus list[5];
int main() {
	first();
	p.reset("k_varies");
	second();
	exemple();
}