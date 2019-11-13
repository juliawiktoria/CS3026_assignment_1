// CS3026 Operating Systems - Assignment 1
// author: Julia Wiktoria Zieba
// ID: 51768280


/* mymemory.c
 * 
 * provides interface to memory management
 * 
 */
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "mymemory.h"


// our memory
Byte        mymemory [MAXMEM] ;
Segment_t * segmenttable = NULL;


void initialize ()
{
   // set memory to 0
   for(int i = 0; i < MAXMEM + 1; i++)
   {
      mymemory[i] = '\0';
   }

   // create segment table
   // contains one segment description that declares the whole memory
   // as one free segment
   //
   // create a single segment descriptor
   Segment_t * new_segment = malloc(sizeof(Segment_t));

   // initialise the segment
   new_segment->allocated = 0;
   new_segment->size = MAXMEM;  // the first segment contains all the memory
   new_segment->start = &mymemory[0]; // CORRECT!!!!
   new_segment->next = NULL;

   // printf("attach newest segment to the segment table\n");
   segmenttable = new_segment;

}

void * mymalloc ( size_t size )
{
   printf ( "mymalloc> start\n");

   // pointer of the starting address of the allocated chunk of memory
   Segment_t *resultPtr;

   // find free segment in the memory
   resultPtr = findFree(segmenttable, size);

   // return NULL if memory cannot be allocated
   if(resultPtr == NULL)
   {
      printf("No free memory to allocate!\n");
      return NULL;
   }

   // if the free space size is the same as target size
   // do not split the segment in 2
   // just change the "allocated" attribute of the segment
   if(resultPtr->size == size)
   {
      printf("Free memory chunk is the same size as allocated data.\n");
      resultPtr->allocated = TRUE;
      return resultPtr->start;
   }
   // found segment is bigger than needed space
   // split it
   else
   {
      splitSegment(resultPtr, size);
      return resultPtr->start;
   }

}

void myfree ( void * ptr )
{
   printf ( "myfree> start\n");

   Segment_t * deallocateThis;

   // find pointer to the segment we want to deallocate
   deallocateThis = findSegment(segmenttable, ptr);

   // wipe memory which the segment is pointing to
   wipeMemory(deallocateThis->start, deallocateThis->size);

   deallocateThis->allocated = 0;

}

void  mydefrag ( void ** ptrList )
{
   printf ( "mydefrag> start\n");

   // move all the allocated segments of the list to the front
   // delete all the not allocated segments

   Segment_t * current = segmenttable, * prev;
   size_t allocated_size = 0;

   while(current != NULL && current->allocated == 0)
   {
      segmenttable = current->next;
      current = segmenttable;
   }
   while(current != NULL)
   {
      while(current != NULL && current->allocated == 1)
      {
         allocated_size = allocated_size + current->size;
         prev = current;
         current = current->next;
      }

      prev->next = current->next;
      current = prev->next;
   }

   // create a new, not allocated segment for the free memory
   Segment_t * newSegment = malloc(sizeof(Segment_t));

   current = segmenttable;
   while (current->next != NULL)
   {
      current = current->next;
   }
   
   current->next = newSegment;
   newSegment->size = MAXMEM - allocated_size;
   newSegment->start = current->start + current->size;
   newSegment->allocated = FALSE;
   newSegment->next = NULL;
   
   updateSegmentAddresses(ptrList);
   // return list of updated pointers
   // return ptrList;
   
}

// helper functions for management segmentation table
Segment_t * findFree ( Segment_t * list, size_t size )
{
   // printf ( "findFree> start\n");
   Segment_t * current = list;

   while(current != NULL)
   {
      if(current->allocated == 0 && current->size >= size)
      {
         // if criteria are fullfilled return the current segment
         return current;
      }
      current = current->next;
   }

   return NULL;

}

void insertAfter ( Segment_t * oldSegment, Segment_t * newSegment )
{
   // just attach the new segment to the last taken one
   oldSegment->next = newSegment;
  // this function implementation assumes
  // that the new segment has been properly
  // initialised (with newSegment->next = NULL)
}

Segment_t * findSegment ( Segment_t * list, void * ptr )
{
   printf("findSegment>start\n");
   Segment_t* current = list;

   // iterate through the sement table until a segment
   // with a correct starting address is found
   printf("while loop > start\n");
   while(current != NULL)
   {
      if(current->start == ptr)
      {
         // returns a pointer to a segment
         return current;
      }
      current = current->next;
   }
   // return NULL if no such segment exists
   return NULL;
}

void splitSegment ( Segment_t * oldSegment, size_t size)
{

   // create a new segment
   Segment_t * newSegment = malloc(sizeof(Segment_t));

   // newSegment = oldSegment->start + size;
   
   // initialise segment to be the size of the rest of the available memory
   // and not allocated
   newSegment->size = oldSegment->size - size;
   newSegment->start = oldSegment->start + size;
   newSegment->allocated = FALSE;
   newSegment->next = oldSegment->next;
   
   oldSegment->size = size;
   oldSegment->allocated = TRUE;
   oldSegment->next = newSegment;

}

void mergeSegment ( Segment_t * currentSegment, Segment_t * nextSegment)
{
   printf("merging two segments\n");
   currentSegment->size = currentSegment->size + nextSegment->size;
   currentSegment->next = nextSegment->next;
   nextSegment->next = NULL;
}

// helper functions for other things
int isPrintable ( int c )
{
   // checks if char is printable but in hex
   if ( c >= 0x20 && c <= 0x7e ) return c ;

   return 0 ;
}

void printmemory ()
{
   printf("\t\t==============================MEMORY CONTENT==============================\n");
   int a = 0;
   printf("[    %d ] ", 0);
   for(int i = 0; i < MAXMEM + 1; i++)
   {
      if(isPrintable(mymemory[i]) != 0)
      {
         printf("\t%02x", isPrintable(mymemory[i]));
         if((i+1) % 10 != 0)
         {
            printf(" ");
         }
         else
         {
            if(i + 1 < 100)
            {
               printf("\n[   %d ] ", i + 1);
            }
            else if (i + 1 < 1000)
            {
               printf("\n[  %d ] ", i + 1);
            }
            else
            {
               printf("\n[ %d ] ", i + 1);
            }
         }  
      } 
      else 
      {
         printf("\t.");
         if((i+1) % 10 != 0)
         {
            printf(" ");
         }
         else
         {
            if(i + 1 < 100)
            {
               printf("\n[   %d ] ", i + 1);
            }
            else if (i + 1 < 1000)
            {
               printf("\n[  %d ] ", i + 1);
            }
            else
            {
               printf("\n[ %d ] ", i + 1);
            }
         }  
      }
   }
   printf("\n\t\t=========================================================================\n");
}

void printsegmenttable()
{
   printf("print segment table\n");

   Segment_t * current = segmenttable;
   int segment_num = 0;

   if(current == NULL)
   {
      printf("the segment table is empty\n");
      return;
   }

   while(current != NULL)
   {
      printf("Segment %d\n", segment_num);
      printsegmentdescriptor(current);

      segment_num = segment_num + 1;
      current = current->next;
   } 

}

void printsegmentdescriptor ( Segment_t * descriptor )
{
      printf ( "\tallocated = %s\n" , (descriptor->allocated == FALSE ? "FALSE" : "TRUE" ) ) ;
      printf ( "\tstart     = %p\n" , descriptor->start ) ;
      printf ( "\tsize      = %lu\n", descriptor->size  ) ;
}

void wipeMemory(void * ptr, size_t size)
{
   for(int i; i < size; i++)
   {
      strcpy(ptr, "\0");
      ptr++;
   }
}

void updateSegmentAddresses(void ** ptrList)
{
   Segment_t * current = segmenttable;
   // point the first segment to the start of the memory array
   current->start = &mymemory[0];
   ptrList = current->start;

   while(current->next != NULL)
   {
      ptrList++;
      current->next->start = current->start + current->size;
      ptrList = current->next->start;
      current = current->next;
   }
}