#include "Profiler.h"
#define MAX_SIZE 10000

/* - Observatii pe baza operatiilor 

	Am implementat algoritmii stabili pentru metodele de sortare bubble sort si insertion sort, pentru selection sort am implementat 
varianta not in-place care am facut-o in pseudocod la seminar.
	Chiar daca acesti 3 algoritmi de sortare apartin clasei de algoritmi quadratici, din graficele obtinute se pot observa 
diferente in cele 3 cazuri:
	- worst case: din punct de vedere al nr de comparatiilor si al atribuirilor(la insertion si selection) putem spune ca cedeaza 
una dintre ele pentru cealalta. De exemplu, selection sort are un numar foarte mic de atribuiri 3(n-1) in timp ce numarul de
comparatii este foarte mare, invers insertion sort-ului. In timp ce bubble sort are un numar relativ mare si de comparatii si de
atribuirii fiind cel mai lent dintre cei 3 algoritmi. Pe baza totalului operatiilor efectuate de cei 3 algoritmi, selection sort 
are numarul minim de operatii efectuate (de 3 ori mai putine comparatii si atribuiri decat insertion), insertion ocupa locul 2 
iar bubble ultimul. 
	- average case: numarul de atribuiri pentru insertion si bubble sort sunt relativ egale, la fel ca numarul de comparatii
intre selection si bubble. Per total, selection sort ocupa din nou primul loc din punct de vedere al numarului total de operatii,
avand cu aprox 50% mai putine decat insertion sort, insertion sort pe cel de- al doilea loc cu aproximativ 70% mai putine operatii
decat bubble sort.
	- best case: cel mai important aspect in acest caz, numarul de atribuiri este nul. Din punct de vedere al timpului, doar 
insertion sort si bubble sort au complexitatea O(n), selection ramane la O(n^2). In acest caz, bubble sort ocupa primul loc (macar
o data si el :)) din punct de vedere al operatiilor alaturi de insertion sort.

PS: stiu ca in cod am incrementat vectorii cu 1000 dar si asa dureaza cam 5 minute pentru o rulare completa si graficele arata 
destul de ok si in cazul asta. Also, am separat graficele doar cand 2 linii par liniare pe ox in comparatie cu a 3.
*/
Profiler profiler("demo-power");

void swap(int *xp, int *yp )
{
	int temp = *xp;
	*xp = *yp;
	*yp = temp;
}

void print(int *arr, int n) {
	for (int i = 0; i < n ; i++)
		printf("%d ", arr[i]);
	printf("\n");
}


void selectionSort(int arr[], int size)
{
	int min;
	for (int i = 0; i < size - 1; i++)
	{
		min = i;
		for (int j = i + 1; j <= size; j++) {
			profiler.countOperation("Comparations_SelectionSort", size);
			profiler.countOperation("SUM_SelectionSort", size);
			if (arr[j] < arr[min]) {
				min = j;
			}
		}

		if (min != i) {
			swap(&arr[min], &arr[i]);
			profiler.countOperation("Assignments_SelectionSort", size,3);
			profiler.countOperation("SUM_SelectionSort", size,3);
		}
	}
}

void insertionSort(int *arr, int size) {
	for (int i = 1; i <= size; i++) {
		int st = 0, dr = i - 1;
		int mid = (st + dr) / 2;
	
		profiler.countOperation("Comparations_InsertionSort", size);
		profiler.countOperation("SUM_InsertionSort", size);
		if (arr[i] < arr[i - 1]) {		//pentru best case cand elementele sunt sortate, nu se va mai efectua cautarea binara
			profiler.countOperation("Comparations_InsertionSort", size);	//creste putin nr de comparatii in worst si average
			profiler.countOperation("SUM_InsertionSort", size);				//dar aduce nr de comparatii in best case la n
			while (arr[mid] != arr[i] && st < dr) {
				profiler.countOperation("Comparations_InsertionSort", size);
				profiler.countOperation("SUM_InsertionSort", size);
				if (arr[mid] > arr[i]) {
					dr = mid;
				}
				else {
					st = mid + 1;
				}
				mid = (st + dr) / 2;
			}
			while (arr[mid] == arr[i])		//realizeaza stabilitatea de ex cazul 1,3,3,3 si 3 - ultimul element de sortat
				mid++;						//in mod normal 3-ul se va insera dupa al doilea 3 nefiind in place.

			int temp = arr[i];
			if (i - 1 != mid)
				for (int k = i;k > mid; k--) {
					profiler.countOperation("SUM_InsertionSort", size, 3);
					profiler.countOperation("Assignments_InsertionSort", size, 3);
					arr[k] = arr[k - 1];
				}
			arr[mid] = temp;
		}
	}
	}


void bubbleSort(int *arr, int n) {
	bool swapped = false;
	for (int i = 0; i < n - 1; i++) {
		for (int j = 0; j < n - i - 1; j++) {
			profiler.countOperation("Comparations_BubbleSort",n);
			profiler.countOperation("SUM_BubbleSort", n);
			if (arr[j + 1] < arr[j]) {
				profiler.countOperation("Assignments_BubbleSort", n,3);
				profiler.countOperation("SUM_BubbleSort", n,3);
				swap(&arr[j + 1], &arr[j]);
				swapped = true;
			}	
		}
		if (swapped == false)
			break;
		swapped = false;
	}
}

void generateWorst() {
	int *v, *x, *y;
	v = (int*)malloc(MAX_SIZE * sizeof(int));
	x = (int*)malloc(MAX_SIZE * sizeof(int));
	y = (int*)malloc(MAX_SIZE * sizeof(int));
	//Worst case
	printf("Worst case: \n");
	for (int i = 1000; i < MAX_SIZE ; i += 1000) {
		FillRandomArray(v, MAX_SIZE, 1, 12000, true, 2);
		FillRandomArray(x, MAX_SIZE, 1, 12000, true, 1);
		memcpy(y, v, i * 4);

		insertionSort(v, i);
		if (IsSorted(v, i))
			printf("Insertion is working...\n");
		else
			printf("Insertion is NOT working...\n");

		int temp = x[0];
		for (int j = 0;j < i; j++)
			x[j] = x[j + 1];
		x[i - 1] = temp;
		selectionSort(x, i);
		if (IsSorted(x, i))
			printf("Selection is working...\n");
		else
			printf("Selection is NOT working...\n");

		bubbleSort(y, i);
		if (IsSorted(y, i))
			printf("Bubble sort is working...\n");
		else
			printf("Bubble sort is NOT working...\n");
	}
	profiler.createGroup("SUM_SORTINGS", "SUM_InsertionSort", "SUM_SelectionSort", "SUM_BubbleSort");
	profiler.createGroup("Assignments_SORTINGS", "Assignments_InsertionSort", "Assignments_SelectionSort", "Assignments_BubbleSort");
	profiler.createGroup("Comparations_SORTINGS", "Comparations_InsertionSort", "Comparations_SelectionSort", "Comparations_BubbleSort");

	free(x);
	free(y);
	free(v);
}

void generateAverage() {
	//  Average CASE
	int *v, *x, *y;
	v = (int*)malloc(MAX_SIZE * sizeof(int));
	x = (int*)malloc(MAX_SIZE * sizeof(int));
	y = (int*)malloc(MAX_SIZE * sizeof(int));
	printf("\n Average: \n");
	for (int j = 0; j < 5; j++) {
		for (int i = 1000; i < MAX_SIZE ; i += 1000) {
			FillRandomArray(v, MAX_SIZE);
			memcpy(x, v, i * 4);
			memcpy(y, v, i * 4);
			
			
			insertionSort(v, i);
			if (IsSorted(v, i))
				printf("Insertion is working...\n");
			else
				printf("Insertion is NOT working...\n");

			selectionSort(x, i);
			if (IsSorted(x, i))
				printf("Selection is working...\n");
			else
				printf("Selection is NOT working...\n");


			bubbleSort(y, i);
			if (IsSorted(y, i))
				printf("Bubble sort is working...\n");
			else
				printf("Bubble sort is NOT working...\n");
	}
	}
	profiler.divideValues("SUM_InsertionSort", 5);
	profiler.divideValues("SUM_SelectionSort", 5);
	profiler.divideValues("SUM_BubbleSort", 5);
	profiler.createGroup("SUM_SORTINGS", "SUM_InsertionSort", "SUM_SelectionSort", "SUM_BubbleSort");
	profiler.divideValues("Assignments_InsertionSort", 5);
	profiler.divideValues("Assignments_SelectionSort", 5);
	profiler.divideValues("Assignments_BubbleSort", 5);
	profiler.createGroup("Assignments_SORTINGS", "Assignments_InsertionSort", "Assignments_SelectionSort", "Assignments_BubbleSort");
	profiler.divideValues("Comparations_InsertionSort", 5);
	profiler.divideValues("Comparations_SelectionSort", 5);
	profiler.divideValues("Comparations_BubbleSort", 5);
	profiler.createGroup("Comparations_SORTINGS", "Comparations_InsertionSort", "Comparations_SelectionSort", "Comparations_BubbleSort");
	free(x);
	free(y);
	free(v);
}

void generateBest() {
	int *v, *x, *y;
	v = (int*)malloc(MAX_SIZE * sizeof(int));
	x = (int*)malloc(MAX_SIZE * sizeof(int));
	y = (int*)malloc(MAX_SIZE * sizeof(int));
	printf("\n Best: \n");

	for (int i = 1000; i < MAX_SIZE ; i += 1000) {
			FillRandomArray(v, MAX_SIZE, 1, 12000, true, 1);
			FillRandomArray(x, MAX_SIZE, 1, 12000, true, 1);
			FillRandomArray(y, MAX_SIZE, 1, 12000, true, 1);

			insertionSort(v, i);
			if (IsSorted(v, i))
				printf("Insertion is working...\n");
			else
				printf("Insertion is NOT working...\n");

			selectionSort(x, i);
			if (IsSorted(x, i))
				printf("Selection is working...\n");
			else
				printf("Selection is NOT working...\n");


			bubbleSort(y, i);
			if (IsSorted(y, i))
				printf("Bubble sort is working...\n");
			else
				printf("Bubble sort is NOT working...\n");
	}
	profiler.createGroup("SUM_SORTINGS", "SUM_InsertionSort", "SUM_BubbleSort");
	profiler.createGroup("Assignments_SORTINGS", "Assignments_InsertionSort", "Assignments_SelectionSort", "Assignments_BubbleSort");
	profiler.createGroup("Comparations_SORTINGS", "Comparations_InsertionSort", "Comparations_BubbleSort");
	free(x);
	free(y);
	free(v);
}

int main(void) {

	generateWorst();
	profiler.reset("Average");
	generateAverage();
	profiler.reset("Best");
	generateBest();
	
	profiler.showReport();
	return 0;
}
