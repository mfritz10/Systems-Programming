#ifndef MYARRAY_H
#define MYARRAY_H

void copyArray(int fromArray[], int toArray[], int size);
void sortArray(int arr[], int size);
int linearSearch(int arr[], int size, int target, int* numComparisons);
int binarySearch(int arr[], int size, int target, int *numComparisons);
void swap(int *first, int *second);

#endif