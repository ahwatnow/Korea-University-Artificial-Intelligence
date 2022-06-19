typedef struct
{
	void **heapArr;
	int	last;
	int	capacity;
	int (*compare) (void *arg1, void *arg2);
} HEAP;

/* Allocates memory for heap and returns address of heap head structure
if memory overflow, NULL returned
*/
HEAP *heap_Create( int capacity, int (*compare) (void *arg1, void *arg2))
{
	HEAP* heap = (HEAP*)malloc(sizeof(HEAP));
	if (heap == NULL)return NULL;
	heap->heapArr = (void**)malloc(capacity * sizeof(void*));
	heap->last = -1;
	heap->capacity = 20;
	heap->compare = compare;
	return heap;
}
/* Free memory for heap
*/
void heap_Destroy( HEAP *heap)
{
	void* dataOut;
	while(heap->last>=0){
		dataOut = heap->heapArr[0];
		heap->heapArr[0] = heap->heapArr[heap->last];
		heap->last--;
		free(dataOut);
	}
	free(heap->heapArr);
	free(heap);
}

/* Inserts data into heap
return 1 if successful; 0 if heap full
*/
int heap_Insert(HEAP *heap, void *dataPtr)
{
	(heap->last)++;
	if (20<=(heap->last))
	{
		return 0;
	}
	heap->heapArr[heap->last] = dataPtr;
	int index = heap->last;
	while ((index != 0) && (heap->compare(heap->heapArr[index],heap->heapArr[(index-1)/2])>0)) {
		void* temp;
		temp = (heap->heapArr[index]);
		heap->heapArr[index] = heap->heapArr[(index - 1) / 2];
		heap->heapArr[(index - 1) / 2] = temp;
		index = (index - 1) / 2;
	return 1;
	}
}
/* Deletes root of heap and passes data back to caller
return 1 if successful; 0 if heap empty
*/
int heap_Delete( HEAP *heap, void **dataOutPtr)
{
	if (-1==heap->last) 
	{
		return 0;
	}
	*dataOutPtr = heap->heapArr[0];
	heap->heapArr[0] = heap->heapArr[heap->last];
	heap->last--;

	int left = 0;
	int right = 0;
	int large = 0;
	void* rightsub;
	void* leftsub;
	int index = heap->last;
	if (index * 2 + 1 <= heap->last) {
		leftsub = heap->heapArr[index * 2 + 1];
		if (index * 2 + 2 <= heap->last) {
			rightsub = heap->heapArr[index * 2 + 2];
			if (heap->compare(leftsub, rightsub) > 0) {
				large = index * 2 + 1;
			}
			else if (heap->compare(leftsub, rightsub) <= 0) {
				large = index * 2 + 2;
			}
		}
		else {
			large = index * 2 + 1;
		}

		if (heap->compare(heap->heapArr[index], heap->heapArr[large]) < 0) {
			void* temp;
			temp = heap->heapArr[index];
			heap->heapArr[index] = heap->heapArr[large];
			heap->heapArr[large] = temp;
		}
	}
	return 1;
}
/*
return 1 if the heap is empty; 0 if not
*/
int heap_Empty(  HEAP *heap)
{
	int last = heap->last;	
	if (last == -1) return 1;
	else return 0;
}

/* Print heap array */
void heap_Print( HEAP *heap, void (*print_func) (void *data))
{
	int c;
	int last = heap->last;
	for(c = 0; c <= last; c++)
	{
		void *value = (heap->heapArr[c]);
		print_func(&value);
	}
	printf( "\n");
}

