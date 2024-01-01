# Malloc

## Presentation
This project is about recreating the standard library memory allocation functions.

We have been asked to code our own malloc(), calloc(), realloc() and free() functions.

Here's the structure of the project :  
```bash
.
├── include
│   └── my_allocator.h
├── Makefile
├── README.md
├── src
│   ├── metadata.h
│   └── my_allocator.c
└── tests
    ├── test_allocator.c
    ├── test_hook.c
    └── test_sbrk.c
```
## malloc(size_t s)
This function is used to allocated a block of size s in memory  
## calloc(size_t nb, size_t s)
This function works exactly as malloc(nb * s) but initialize all elements to 0
## realloc(void* ptr, size_t s)
This function is used to allocate more space to a previously allocated block of memory
## free(void* ptr)
This function is used to deallocate the previoulsy allocated memory
## Code compilation and usage
Type this in a shell terminal
```bash
$ make && ./test_allocator
```
or if you want to use the hooked version
```bash
$ make && ./test_hook
```
Then to clean your repository
```bash
$ make clean
```
## Information
### Author
GOUPIL Kristian
### Project dates
From 10/06/2021 to 10/14/2021
### Current version
v 1.0, released on the 10/13/2021