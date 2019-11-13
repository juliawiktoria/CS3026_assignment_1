// CS3026 Operating Systems - Assignment 1
// author: Julia Wiktoria Zieba
// ID: 51768280

#include <stdio.h>
#include <string.h>
#include "mymemory.h"

int main()
{
    printf ( "shell> start\n");

    initialize() ;

    printf ("=== PRINT EMPTY MEMORY ===\n");
    printmemory();
    printf ("=== PRINT EMPTY SEGMENT TABLE ===\n");
    printsegmenttable();
    printf("___________________________________________________________\n");

    char * ptr1 = (char *) mymalloc ( 10 ) ;
    if(ptr1 == NULL)
    {
        printf("malloc error!!!\n");
    }
    else
    {
        strcpy (ptr1, "this test");
        printf( "shell> content of allocated memory: %s\n", ptr1 ) ;
    }

    printf ("=== PRINT MEMORY AFTER ALLOCATING PTR 1 ===\n");
    printmemory();
    printf ("=== PRINT SEGMENT TABLE AFTER ALLOCATING PTR 1 ===\n");
    printsegmenttable();
    printf("___________________________________________________________\n");

    char * ptr2 = (char *) mymalloc ( 12 ) ;
    if(ptr2 == NULL)
    {
        printf("malloc error!!!\n");
    }
    else
    {
        strcpy (ptr2, "second test");
        printf( "shell> content of allocated memory: %s\n", ptr2 ) ;
    }

    printf ("=== PRINT MEMORY AFTER ALLOCATING PTR 2 ===\n");
    printmemory();
    printf ("=== PRINT SEGMENT TABLE AFTER ALLOCATING PTR 2 ===\n");
    printsegmenttable();
    printf("___________________________________________________________\n");

    char * ptr3 = (char *) mymalloc ( 11 ) ;
    if(ptr3 == NULL)
    {
        printf("malloc error!!!\n");
    }
    else
    {
        strcpy (ptr3, "third test");
        printf( "shell> content of allocated memory: %s\n", ptr3 ) ;
    }

    printf ("=== PRINT MEMORY AFTER ALLOCATING PTR 3 ===\n");
    printmemory();
    printf ("=== PRINT SEGMENT TABLE AFTER ALLOCATING PTR 3 ===\n");
    printsegmenttable();
    printf("___________________________________________________________\n");
    
    myfree ( ptr1 ) ;
    
    printf ("=== PRINT MEMORY AFTER FREEING PTR 1 ===\n");
    printmemory();
    printf ("=== PRINT SEGMENT TABLE AFTER FREEING PTR 1 ===\n");
    printsegmenttable();
    printf("___________________________________________________________\n");

    void * pointerList [] = {ptr2, ptr3};

    mydefrag(pointerList);
    
    printf ("=== PRINT MEMORY AFTER DEFRAGMENTATION ===\n");
    printmemory();
    printf ("=== PRINT SEGMENT TABLE AFTER DEFRAGMENTATION ===\n");
    printsegmenttable();
    printf("___________________________________________________________\n");

    printf ( "shell> end\n");
    
    return 0;
}