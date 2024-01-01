#define _DEFAULT_SOURCE
#include <unistd.h>
#include <my_allocator.h>
#include <stdio.h>
#include <../src/metadata.h>
#include <malloc.h>

Block* head = NULL;
Block* tail = NULL;

// This function is used to get a size which is a multiple of 8
int alignByte(int size)
{
    if (size % 8 == 0)
        return size;
    else
        return 8 - size % 8 + size;
}

// This function is used to divide data blocks into smaller blocks
void splitBlock(size_t size, Block* blockToSplit)
{
    // Here we use a char pointer to manipulate byte by byte our address
    char* newPtr = (char*)blockToSplit + size + sizeof(Block);

    // Then we convert it into a block ptr in order to use it
    Block* newFreeBlock = (Block*)newPtr;
    newFreeBlock->size = blockToSplit->size - alignByte(size) - sizeof(Block);
    newFreeBlock->available = true;
    newFreeBlock->prev = blockToSplit;
    newFreeBlock->next = blockToSplit->next;
    if (newFreeBlock->next != NULL)
        newFreeBlock->next->prev = newFreeBlock;
    newFreeBlock->dataPtr = newFreeBlock + 1;
    blockToSplit->next = newFreeBlock;
    if (blockToSplit == tail)
        tail = newFreeBlock;
}

// This function is used by malloc in order to see if there is a data block big enough to contain our
// new data and if yes we split the found block into two smaller blocks which one of them has a size
// equal to our new block size
void* getFreeBlock(size_t size)
{
    for (Block* cur = head; cur != NULL; cur = cur->next)
    {
        if (cur->available == false || (size_t)cur->size < size + sizeof(Block))
            continue;
        else if (((size_t)cur->size > size + sizeof(Block) || (cur == head && (size_t)cur->size > size)))
        {
            splitBlock(size, cur);
            cur->size = alignByte(size);
        } 
        cur->available = false;
        return cur->dataPtr;
    }
    return NULL;
}

// This is our main allocator function as it is used in calloc and realloc
void* my_malloc(size_t size){
    // We first check if there is a free block to use and if not continue
    void* freeBlock = getFreeBlock(size);
    if (freeBlock != NULL)
        return freeBlock;


    Block* newBlock = sbrk(sizeof(Block));
    newBlock->size = alignByte(size);
    newBlock->dataPtr = sbrk(newBlock->size);
    newBlock->next = NULL;
    newBlock->available = false;

    if(head == NULL)
    {
        head = newBlock;
        head->prev = NULL;
        tail = head;
    }
    else
    {
        newBlock->prev = tail;
        tail->next = newBlock;
        tail = newBlock;
    }
    return tail->dataPtr;
}

// This function is used to merge adjacent free blocks into a single data block adding their sizes together
Block* fusionBlock(Block* ptrToMerge)
{
    Block* ptrPrev = ptrToMerge->prev;
    Block* ptrNext = ptrToMerge->next;

    // This part is used to check if the previous block is free and if yes merge it with the current block
    if (ptrPrev != NULL && ptrPrev->available == true)
    {
        ptrPrev->size = alignByte(ptrPrev->size + ptrToMerge->size + sizeof(Block));
        ptrPrev->next = ptrNext;
        if (ptrNext != NULL)
            ptrNext->prev = ptrPrev;
        if (ptrToMerge == tail)
            tail = ptrPrev;
        ptrToMerge = ptrPrev;
    }
    // A double if is needed here as a previous block and a next block can be free and so available to merge
    // This part is used to check if the next block is free and if yes merge it with the current block
    if (ptrNext != NULL && ptrNext->available == true)
    {
        ptrToMerge->size = alignByte(ptrToMerge->size + ptrNext->size + sizeof(Block));
        ptrToMerge->next = ptrNext->next;
        if (ptrNext->next != NULL)
            ptrNext->next->prev = ptrToMerge;
        if (ptrNext == tail)
            tail = ptrToMerge;
    }
    return ptrToMerge;
}

// This function is used to free the memoy previously allocated and thus moving the break
void my_free(void* ptr){
    char* tmpPtr = ptr - sizeof(Block);
    Block* ptrToFree = (Block*)tmpPtr;
    ptrToFree->available = true;
    ptrToFree = fusionBlock(ptrToFree);
    if (ptrToFree == tail)
    {
        tail = tail->prev;
        if (tail != NULL)
            tail->next = NULL;
        sbrk(-(ptrToFree->size + sizeof(Block)));
        if (ptrToFree == head)
            head = NULL;
    }
}

// This function is used to grow a block, firstly by trying to merge adjacent blocks and if failed by allocating a new memory space
void* my_realloc(void* ptr, size_t size){
    char* tempPtr = ptr - sizeof(Block);
    Block* ptrToRealloc = (Block*)tempPtr;
    int initSize = ptrToRealloc->size;
    void* initPtr = ptrToRealloc;
    ptrToRealloc = fusionBlock(ptrToRealloc);
    if (ptrToRealloc == initPtr || (size_t)ptrToRealloc->size < size)
    {
        tempPtr = my_malloc(size) - sizeof(Block);
        ptrToRealloc = (Block*)tempPtr;
        tail = ptrToRealloc;
        my_free(ptr);
    }
    char* ptrDestination = ptrToRealloc->dataPtr;
    for (int i = 0; i < initSize; i++)  
        *(ptrDestination + i) = *(char*)(ptr + i);
    return ptrToRealloc->dataPtr;
}

// This function works exactly as malloc but it sets all data to 0 as well
void* my_calloc(size_t nb, size_t size){
    char* ptr = my_malloc(nb * size);
    for (int i = 0; (size_t)i < nb * size; i++)
        *(ptr + i) = 0;
    return ptr;
}

// This function is used to show the current state of our memory listing its allocations
void showDebugInfo()
{
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n");
    for (Block* cur = head; cur != NULL; cur = cur->next)
    {
        printf("Block address = %p\n", cur);
        if (cur->available == true)
            printf("Available = true\n");
        else
            printf("Available = false\n");
        printf("Size = %d\n"
               "Data address = %p\n"
               "Previous address = %p\n"
               "Next address = %p\n",
               cur->size, cur->dataPtr, cur->prev, cur->next);
        if (cur->next != NULL)
            printf("\n");
    }
    printf("+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+\n\n");
}

// This part is made to link our functions with the standard library functions
void* malloc_hook(size_t size, const void* caller)            { return my_malloc(size); }
void* realloc_hook(void* ptr, size_t size, const void* caller){ return my_realloc(ptr, size); }
void  free_hook(void* ptr, const void* caller)                { return my_free(ptr); }
void setupHooks()
{
   __malloc_hook = malloc_hook;
   __realloc_hook = realloc_hook;
   __free_hook = free_hook;
}
void registerAllocatorHook()
{
    setupHooks();
}