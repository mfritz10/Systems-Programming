#include "myarray.h"

void copyArray(int fromArray[], int toArray[], int size)
    {
        for (int i = 0; i < size; i++) {
            toArray[i] = fromArray[i];
        }
    }

void sortArray(int arr[], int size)
    {
        for (int i = 0; i < size - 1; i++){

            int swapped = 0;
            for (int j = 0; j < size - i - 1; j++ ){
                if (arr[j] > arr[j+1] ) {
                    swap(&arr[j], &arr[j+1]);
                    swapped = 1;
                }
            }
            if (swapped == 0){
                break;
            }
        }
    }

void swap(int *first, int *second) {

            int temp = *first;
            *first = *second;
            *second = temp;
        }

int linearSearch(int arr[], int size, int target, int* numComparisons)
    {
        *numComparisons = 0;
        for (int i = 0; i < size; i++) {
            if (arr[i] == target){
                return i;
            }
            (*numComparisons)++;
        }
        return -1;
    }

int binarySearch(int arr[], int size, int target, int *numComparisons)
    {
        int lower = 0;
        int upper = size -1;
        *numComparisons = 0;
        

        while (lower < upper){

            int middle = (upper + lower)/2;

            if (arr[middle] == target){
                return middle;
            }

            else if (target < arr[middle]){
                upper = middle;
            }

            else {
                lower = middle + 1;
            }

            (*numComparisons)++;
        }

        return -1;

    }