#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "3-4print-pte.h"

int main(void)
{
  char string[30]; /* String with the memory area name */
  void *p;         /* Pointer to the requested memory area */
  int memd;        /* Descriptor of the requested memory area */

  /////////////////////////////////////
  // A 16-KiB area memory is requested
  ////////////////////////////////////

  /* Open a memory descriptor */
  if ((memd = shm_open("/mem-1504", O_RDWR|O_EXCL, S_IRWXU)) < 0)
     return -1; /* Finish the program in case of error */
  
  


  /* Defines the size of the memory area */
  if (ftruncate(memd, 16*1024) < 0)
    {
      shm_unlink(string); /* Closes the memory descriptor */
      return(-1); /* If an error occurs the task is finished */
    }

  /* Maps the memory area in the virtual address space of the task */
  if ((p = mmap(NULL, 16*1024, PROT_READ|PROT_WRITE,
                MAP_SHARED, memd, 0)) == MAP_FAILED)
    {
      
      shm_unlink(string); /* Closes the memory descriptor */
      return(-1); /* If an error occurs the task is finished */
    }
  /* Read a character from the first location in the allocated memory */
  printf("The character written by the other task is: %c\n", *((char *)p));
  ////////////////////////////////////////////////////////////////
  // What is the physical address associated with the memory area?
  ////////////////////////////////////////////////////////////////
  print_virtual_physical_pte(p, "The requested memory area\n"
                                 "-----------------------------");

  printf("\nProcess Identifier (PID): %d\n", getpid());

  printf("\n---- Press [ENTER] to continue");
  getchar();

  return 0;
}
