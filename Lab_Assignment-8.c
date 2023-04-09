#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int extraMemoryAllocated;

// implements heap sort
// extraMemoryAllocated counts bytes of memory allocated


// Heapify or sorting a tree first and making sure the parent node is greater than children nodes
void heapify(int pData[], int n, int index){
    int leftC = 2 * index + 1; // index of left child
    int rightC = 2 * index + 2; // index of right child
    int largest = index; //Initially, largest is assumed to be the index, but may change if child nodes are larger

    if (leftC < n && pData[largest] < pData[leftC]) {
        largest = leftC;
    }

    if (rightC < n && pData[largest] < pData[rightC]) {
        largest = rightC;
    }

    if (largest != index){

        int tmp = pData[index]; // This last block of code swaps the items

        pData[index] = pData[largest];
        pData[largest] = tmp;

        heapify(pData, n, largest);
    }

}

void heapSort(int pData[], int n)
{
    // This builds the max heap using the heapify function
    // n / 2 - 1 is the index of the last internal node in the heap
    int index = (n / 2) - 1;
    for (index; index >= 0; index--){
        heapify(pData, n, index);
    }
    // Sorting and moving parent node to end
    int newIndex = n - 1;
    for (newIndex; newIndex >= 0; newIndex--){
        int tmp = pData[0]; // This last block of code swaps the items

        pData[0] = pData[newIndex];
        pData[newIndex] = tmp;

        heapify(pData, newIndex, 0);
    }
}


// implement merge sort
// extraMemoryAllocated counts bytes of extra memory allocated
void merge(int pData[], int l, int middle, int r) {

    int i = 0;
    int j = 0;
    int k = 0;
    int lengthLeft = middle - l + 1;
    int lengthRight = r - middle;

    // temporary arrays
    int *leftArray = malloc(lengthLeft * sizeof(int));
    extraMemoryAllocated += lengthLeft * sizeof(int);
    int *rightArray = malloc(lengthRight * sizeof(int));
    extraMemoryAllocated += lengthRight * sizeof(int);

    for (i = 0; i < lengthLeft; i++){
        leftArray[i] = pData[l + i];
    }
    for (j = 0; j < lengthRight; j++){
        rightArray[j] = pData[middle + 1 + j];
    }

    i = 0, j = 0, k = l;

    while (i < lengthLeft && j < lengthRight) {
        if (leftArray[i] > rightArray[j]) {
            pData[k] = rightArray[j];
            j++;
            k++;
        } else {
            pData[k] = leftArray[i];
            i++;
            k++;
        }
    }
    while (i < lengthLeft) {
        pData[k] = leftArray[i];
        i++;
        k++;
    }
    while (j < lengthRight) {
        pData[k] = rightArray[j];
        j++;
        k++;
    }
}
void mergeSort(int pData[], int l, int r) {
    if (r > l) {
        int middle = (l + r) / 2;
        mergeSort(pData, l, middle);
        mergeSort(pData, middle + 1, r);
        merge(pData, l, middle, r);
    }
}

// parses input file to an integer array
int parseData(char *inputFileName, int **ppData)
{
    FILE* inFile = fopen(inputFileName,"r");
    int dataSz = 0;
    int i, n, *data;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile,"%d\n",&dataSz);
        *ppData = (int *)malloc(sizeof(int) * dataSz);
        // Implement parse data block
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i=0;i<dataSz;++i)
        {
            fscanf(inFile, "%d ",&n);
            data = *ppData + i;
            *data = n;
        }

        fclose(inFile);
    }

    return dataSz;
}

// prints first and last 100 items in the data array
void printArray(int pData[], int dataSz)
{
    int i, sz = dataSz - 100;
    printf("\tData:\n\t");


    for (i=0;i<100;++i)
    {
        if(i >= dataSz){
            printf("\n\n");
            return;
        }
        printf("%d ",pData[i]);
    }
    printf("\n\t");

    for (i=sz;i<dataSz;++i)
    {
        printf("%d ",pData[i]);
    }
    printf("\n\n");
}

int main(void)
{
    clock_t start, end;
    int i;
    double cpu_time_used;
    char* fileNames[] = {  "input1.txt", "input2.txt", "input3.txt", "input4.txt" };

    for (i=0;i<4;++i)
    {
        int *pDataSrc, *pDataCopy;
        int dataSz = parseData(fileNames[i], &pDataSrc);

        if (dataSz <= 0)
            continue;

        pDataCopy = (int *)malloc(sizeof(int)*dataSz);

        printf("---------------------------\n");
        printf("Dataset Size : %d\n",dataSz);
        printf("---------------------------\n");

        printf("Heap Sort:\n");
        memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
        extraMemoryAllocated = 0;
        start = clock();
        heapSort(pDataCopy, dataSz);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
        printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
        printArray(pDataCopy, dataSz);

        printf("Merge Sort:\n");
        memcpy(pDataCopy, pDataSrc, dataSz*sizeof(int));
        extraMemoryAllocated = 0;
        start = clock();
        mergeSort(pDataCopy, 0, dataSz - 1);
        end = clock();
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\truntime\t\t\t: %.1lf\n",cpu_time_used);
        printf("\textra memory allocated\t: %d\n",extraMemoryAllocated);
        printArray(pDataCopy, dataSz);

        free(pDataCopy);
        free(pDataSrc);
    }

}
