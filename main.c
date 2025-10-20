#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define N 100000 // исходный размер памяти в байтах
char memory[N];
int count = 0;

struct block { // односвязный список, элемент которого хранит размер и состояние одного блока памяти
	size_t size; // размер области
	int free; // состояние 1 - свободно, 0 - занято
	struct block* next; // указатель на следующий блок памяти
};

struct block* freeList; // указатель на исходную кучу

void initialize();
void split(struct block* fitting_slot, size_t size);
void* my_malloc(size_t noOfBytes);
void merge();
void my_free(void* ptr);
void show();
void generator();

int main() {
	srand(time(NULL));
	freeList = (void*)malloc(memory); // используем в качестве кучи память, выделенную malloc
	initialize();
	clock_t begin = clock();
	generator();
	clock_t end = clock();
	double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
	printf("%f\n", time_spent);
	//int* p = (int)my_malloc(100 * sizeof(int));
	//show();
	//char* q = (char)my_malloc(250 * sizeof(char));
	//show();
	//int* r = (int)my_malloc(1000 * sizeof(int));
	//show();
	//my_free(p);
	//show();
	//char* w = (char)my_malloc(90000);
	//show();
	//my_free(r);
	//show();
	//int* k = (int)my_malloc(500 * sizeof(int));
	//show();
	printf("%d", count);
}


void split(struct block* fitting_slot, size_t size) { // выделение памяти из кучи
	struct block* new = (void*)((struct block*)fitting_slot + size + sizeof(struct block)); // инициализуруем новый свободный блок памяти 
	new->size = (fitting_slot->size) - size - sizeof(struct block); // размер за вычетом инициализируемого вместе с размерностью структуры
	new->free = 1; // объявляем свободным
	new->next = fitting_slot->next; 
	fitting_slot->size = size;
	fitting_slot->free = 0;
	fitting_slot->next = new;
}


void* my_malloc(size_t noOfBytes) {
	struct block* curr, * prev;
	void* result;
	curr = freeList;
	while ((((curr->size) < noOfBytes) || ((curr->free) == 0)) && (curr->next != NULL)) {
		prev = curr;
		curr = curr->next;
		count++;
	}
	if ((curr->size) == noOfBytes) {
		curr->free = 0;
		result = (void*)(++curr);
		return result;
	}
	else if ((curr->size) > (noOfBytes + sizeof(struct block))) {
		split(curr, noOfBytes);
		result = (void*)(++curr);
		//printf("Allocated with a split\n");
		return result;
	}
	else {
		result = NULL;
		printf("Not enough memory\n");
		return result;
	}
}

void merge() { // объединение соседних свободных участков памяти
	struct block* curr;
		//* prev;
	curr = freeList;
	while (curr->next != NULL) {
		if (curr == NULL) return;
		if ((curr->free) && (curr->next->free)) {
			curr->size += (curr->next->size) + sizeof(struct block);
			curr->next = curr->next->next;
		}
		//prev = curr;
		if (curr->next == NULL) return;
		curr = curr->next;
	}
}

void my_free(void* ptr) {
	if (((void*)freeList <= ptr) && (ptr <= (void*)(freeList + N))) {
		//struct block* tmp = freeList;
		struct block* curr = ptr;
		//while (freeList->next != ptr) {
		//	freeList = freeList->next;
		//};
		//freeList->free = 1; 
		//freeList = tmp;
		--curr;
		curr->free = 1;
		merge();
	}
	else printf("Invalid pointer\n");
}

void initialize() { // инициализация исходного списка с памятью
	freeList->size = sizeof(memory) - sizeof(struct block); // выделяем память под структуру из исходной кучи
	freeList->free = 1; // объявляем свободной 
	freeList->next = NULL; // указателя на следующего нет
}

void show() { // отображение текущего состояния ДРП
	struct block* tmp = freeList;
	while (tmp->next != NULL) {
		printf("Size of block: %d; Adress of block: %p; Status of block: %d\n", tmp->size, &(*tmp), tmp->free);
		tmp = tmp->next;
	}
	printf("Size of block: %d; Adress of block: %p; Status of block: %d\n", tmp->size, &(*tmp), tmp->free);
	system("pause");
	system("cls");
}

void generator() {
	int n = 10000;
	int* A[10000];
	for (int i = 0; i < n; ++i) {
		A[i] = (int*)my_malloc(sizeof(int));
	}
	for (int i = 0; i < n; ++i) {
		int x = rand() % n;
		my_free(A[x]);
		A[x] = (int*)my_malloc(sizeof(int));
	}
}
