#include <stdbool.h>
typedef struct Metadata
{
    void* dataPtr;
    int size;
    bool available;
    struct Metadata* prev;
    struct Metadata* next;
} Block;