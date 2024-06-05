#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HEAP_SIZE 1024 // Size of the heap
#define BLOCK_HEADER_SIZE sizeof(header_t)

char heap[HEAP_SIZE];

// Heap header struct
typedef struct header {
    size_t size;
    bool free;
} header_t;


// This is the initial allocator. It covers the entire heap minus the block header size.
void init_allocator() {
    header_t *header = (header_t *)heap;
    header->size = HEAP_SIZE - BLOCK_HEADER_SIZE;
    header->free = true;
}


// This is a helper function used by my_malloc. It searches through the heap using block headers to find a free block.
header_t *find_free_block(size_t size) {
    
    int i = 0;
    header_t *iterator = (header_t *)&heap[i];
    
    while (HEAP_SIZE > i){

        if (iterator->free == true && size <= iterator->size){
            
            return iterator;

        } else {
            i = i + (int)iterator->size + BLOCK_HEADER_SIZE;
            if (HEAP_SIZE <= i + BLOCK_HEADER_SIZE){
                return NULL;
                
            } else {
                iterator = (header_t *)&heap[i];
            }
        }
    }
}


// This is another helper function used by my_malloc to split the blocks if necessary.
void split_block(header_t *block, size_t size) {

    if (block->size > size){

        
        size_t temp = block->size;
        block->size = size;
        header_t* address = block;
        address = (header_t*)((char*)address + size + BLOCK_HEADER_SIZE);
        header_t *newHead = (header_t *)address;  
        newHead->free = true;
        newHead->size = temp - size - BLOCK_HEADER_SIZE;
        


    }
}


// This is a helper function that coalesces blocks when freeing them.
// You need to implement this function.
void coalesce() {
    int i = 0;
    header_t *prev = (header_t *)&heap[i];

    i = i + (int)prev->size + BLOCK_HEADER_SIZE;
    
    header_t *next = (header_t *)&heap[i];

    while (HEAP_SIZE > i){
        if (prev->free == true && next->free == true){
            prev->size += next->size + BLOCK_HEADER_SIZE;
            i = i + (int)next->size + BLOCK_HEADER_SIZE;
            if (i < HEAP_SIZE){
                next = (header_t *)&heap[i];
            } else {
                break;
            }
        } else {
            i = i + (int)next->size + BLOCK_HEADER_SIZE;
            prev = next;
            if (i < HEAP_SIZE){
                next = (header_t *)&heap[i];
            } else {
                break;
            }
        }
    }




    

}

// The main malloc function that allocated memory, or returns Null if no block is found.
// This function has been implemented for you.
void *my_malloc(size_t size) {
    header_t *block = find_free_block(size);

    if (!block) {
        return NULL; // No free block found
    }
    if (block->size > size + BLOCK_HEADER_SIZE) {
        split_block(block, size);
    }
    block->free = false;
    return (char *)block + BLOCK_HEADER_SIZE;
}


// This function frees a pointer, pointing to a block on the heap.
// This function has been implemented for you.
void my_free(void *ptr) {
    if (!ptr) {
        return;
    }
    header_t *block = (header_t *)((char *)ptr - BLOCK_HEADER_SIZE);
    block->free = true;
    coalesce(); // Coalesce if possible
}


int main() {

    // Initialize the heap
    init_allocator();

    // Test the heap.
    void *ptr1 = my_malloc(500);
    printf("Allocated %ld bytes\n", ((header_t *)((char *)ptr1 - BLOCK_HEADER_SIZE))->size);

    void *ptr2 = my_malloc(50);
    printf("Allocated %ld bytes\n", ((header_t *)((char *)ptr2 - BLOCK_HEADER_SIZE))->size);

    void *ptr3 = my_malloc(1000);
    if (ptr3 == NULL){
        printf("Not enough space to allocate\n");
    }

    void *ptr4 = my_malloc(400);
    printf("Allocated %ld bytes\n", ((header_t *)((char *)ptr4 - BLOCK_HEADER_SIZE))->size);

    my_free(ptr1);
    my_free(ptr2);
    my_free(ptr3);
    my_free(ptr4);
    printf("Blocks freed\n");

    void *ptr5 = my_malloc(500);
    printf("Allocated %ld bytes\n", ((header_t *)((char *)ptr5 - BLOCK_HEADER_SIZE))->size);

    void *ptr6 = my_malloc(50);
    printf("Allocated %ld bytes\n", ((header_t *)((char *)ptr6 - BLOCK_HEADER_SIZE))->size);

    void *ptr7 = my_malloc(400);
    printf("Allocated %ld bytes\n", ((header_t *)((char *)ptr7 - BLOCK_HEADER_SIZE))->size);

    my_free(ptr5);
    my_free(ptr6);

    void *ptr8 = my_malloc(540);
    printf("Allocated %ld bytes\n", ((header_t *)((char *)ptr8 - BLOCK_HEADER_SIZE))->size);

    void *ptr9 = my_malloc(10);
    printf("Allocated %ld bytes\n", ((header_t *)((char *)ptr9 - BLOCK_HEADER_SIZE))->size);

    my_free(ptr7);
    my_free(ptr8);
    my_free(ptr9);
    printf("Blocks freed\n");




    return 0;
}