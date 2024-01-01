#include <my_allocator.h>

#define USE_MY_ALLOCATOR

#ifdef USE_MY_ALLOCATOR
#define MALLOC my_malloc
#define REALLOC my_realloc
#define CALLOC my_calloc
#define FREE my_free
#else
#define ALLOC malloc
#define REALLOC realloc
#define CALLOC calloc
#define FREE free
#endif

void testMalloc()
{
    int* anInt = my_malloc(sizeof(int));
    *anInt = 24;
    printf("Address = %p, anInt = %d\n", anInt, *anInt);

    showDebugInfo();

    int* anotherInt = my_malloc(sizeof(int));
    *anotherInt = 12;
    printf("Address = %p, anotherInt = %d\n", anotherInt, *anotherInt);

    showDebugInfo();
}

void testFree()
{
    char* myStr = my_malloc(41 * sizeof(char));

    int* fisrtInt = my_malloc(sizeof(int));

    int* secondInt = my_malloc(sizeof(int));

    showDebugInfo();

    my_free(secondInt);

    showDebugInfo();

    my_free(myStr);

    showDebugInfo();

    my_free(fisrtInt);
}

void testSplit()
{
    char* myStr = my_malloc(41 * sizeof(char));

    int* fisrtInt = my_malloc(sizeof(int));

    my_free(myStr);

    showDebugInfo();

    int* anInt = my_malloc(sizeof(int));
    *anInt = 24;

    showDebugInfo();

    my_free(fisrtInt);
    my_free(anInt);
}

void testFusion()
{
    char* myStr = my_malloc(27 * sizeof(char));

    int* fisrtInt = my_malloc(sizeof(int));

    my_free(myStr);

    int* anInt = my_malloc(sizeof(int));
    *anInt = 24;

    showDebugInfo();

    my_free(fisrtInt);

    showDebugInfo();

    my_free(anInt);
}

typedef struct testStruct
{
    int x;
    int y;
    char name[20];
} testStruct;

void testCalloc()
{
    testStruct* test = my_calloc(1, sizeof(testStruct));
    printf("x = %d, y = %d, name = %s\n", test->x, test->y, test->name);

    showDebugInfo();
    my_free(test);
}

void testRealloc()
{
    int* a = my_malloc(sizeof(int) * 3);
    a[0] = 1;
    a[1] = 2;
    a[2] = 3;
    int* b = my_malloc(sizeof(int));
    int* c = my_malloc(sizeof(int));

    printf("a = { ");
    for (int i = 0; i < 3; i++)
        printf("%d ", a[i]);
    printf("}\n");

    showDebugInfo();

    my_free(b);

    showDebugInfo();

    a = my_realloc(a, 8 * sizeof(int));
    a[3] = 4;
    a[4] = 5;
    a[5] = 6;
    a[6] = 7;
    a[7] = 8;

    printf("a = { ");
    for (int i = 0; i < 8; i++)
        printf("%d ", a[i]);
    printf("}\n");

    showDebugInfo();
    my_free(a);
    my_free(c);
}

void testAlignBytes()
{
    char* threeChar = my_malloc(sizeof(char) * 3);
    showDebugInfo();
    char* sixChar = my_malloc(sizeof(char) * 6);
    showDebugInfo();
    char* sixteenChar = my_realloc(threeChar, sizeof(char) * 16);
    showDebugInfo();
    char* eightChar = my_calloc(8, sizeof(char));
    showDebugInfo();
    my_free(threeChar);
    my_free(sixChar);
    my_free(sixteenChar);
    my_free(eightChar);
}

int main()
{
    //testMalloc();

    // testFree();

    // testSplit();

    // testFusion();

    // testCalloc();    

    // testRealloc();

    // testAlignBytes();

    return 0;
}