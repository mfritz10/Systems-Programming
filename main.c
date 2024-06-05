#include <stdio.h>
#include <stdlib.h>
#include "myarray.h"

int main() {

    int size = 10;
    int *arr = (int*)malloc(size * sizeof(int));

    int index = 0;
    int userInput;

    printf("Enter numbers to add to array: ");

    while (1) {
        scanf("%d", &userInput);
        if (userInput == -1) {
            break;
        }

        arr[index] = userInput;
        index++;
        if (index >= size -1) {
            size = size * 2;
            int *temp = (int*)realloc(arr, (size)*sizeof(int));
            if(temp == NULL) {
                fprintf(stderr, "Memory reallocation failed!\n");
                free(arr);
                return 1;
            }

            arr = temp;
        }
    }


    int *arrCopy = (int*)malloc(size * sizeof(int));
    copyArray(arr, arrCopy, size);

    sortArray(arrCopy, size);

    int userSearch = 0;
    int numComparisons = 0;

    printf("Enter a number to be searched for: ");
    scanf("%d", &userSearch);

    //linear search
    printf("\nLINEAR SEARCH\n");
    printf("--------------\n");
    printf("The value being searched for is %d\n", userSearch);
    int res = linearSearch(arr, size, userSearch, &numComparisons);
    if (res == -1){
        printf("The number was not found\n");
    } else {
        printf("The item was found at index %d\n", res);
    }

    printf("It took %d comparisons\n", numComparisons);

    

    //binary search
    printf("\nBINARY SEARCH\n");
    printf("--------------\n");

    printf("The value being searched for is %d\n", userSearch);
    res = binarySearch(arrCopy, size, userSearch, &numComparisons);
    if (res == -1){
        printf("The number was not found\n");
    } else {
        printf("The item was found at index %d\n", res);
    }

    printf("It took %d comparisons\n", numComparisons);



    free(arr);
    free(arrCopy);
    return 0;
}