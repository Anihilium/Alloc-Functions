#define _DEFAULT_SOURCE
#include <unistd.h>
#include <stdio.h>
 
int main()
{
    void* ptr = sbrk(0);

    printf("Init adress : %p\n", ptr);

    ptr = sbrk(0);
    printf("After first printf adress : %p\n", ptr);

    // On alloue l'espace de la taille d'un int
    int* number = sbrk(sizeof(int));
    *number = 42;

    ptr = sbrk(0);
    printf("After int adress : %p\n", ptr);

    // On retire l'espace precedement alloue
    sbrk(-sizeof(int));

    ptr = sbrk(0);
    printf("Final adress : %p\n", ptr);
}