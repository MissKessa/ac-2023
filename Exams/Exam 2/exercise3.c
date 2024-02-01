#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <math.h>
#include <atc/linmem.h>

int global = 0xABCFEF02;

int task()
{
	return 0xABCFEF03;
}

int type(int number, int n) {
    int aux = (number / (int)pow(10, n)) % 10;

	if (aux <= 3) 
	{
		return 1;
	}
	else if (aux <=6)
	{
		return 2;
	}
	else 
	{
		return 3;
	}
}

int main(void)
{
	// *** Start GAP 1 ***
	// Fill with your DNI without letter
	int DNI =71789634 ;
	// *** End GAP 1 ***
	
	unsigned int pte_value, ph_addr, f_vp;
	int local = 0xABCFEDF1;
	void *var;
	
	int examType = type(DNI, 2);
	if (examType == 1)
	{
		var = (void *)&local;
	}
	else if (examType == 2)
	{
		var = (void *)&task;
	}
	else 
	{
		var = (void *)&global;
	}


	// *** Start GAP 2 ***
	// Save in "pte_value" the page table entry associated with the virtual address that stores "var"
	if (get_pte(var, &pte_value))
	{
		perror("Linmem module error");
		return -1;
	}
	// *** End GAP 2 ***
	
	
	// *** Start GAP3 *** 
	// Check that the page is in memory
	if ((pte_value & 0x00000001) == 1)
	{
		// Get the flags associated to the memory page and store them in the variable "f_vp"
		f_vp = (unsigned int)(pte_value & 0x0000FFF);

		// Get the physical address of the memory and store them in the variable "ph_addr"
		ph_addr = (pte_value & 0xFFFF000)+((unsigned int) var & 0x0000FFF);
	}
	// *** End GAP 3 ***
	else
	{
		perror("The page has no page frame allocated\n");
		return -1;
	}	

	printf("Virtual Address:\t%.8Xh\n", (unsigned int) var);
	printf("Physical Address:\t%.8Xh\n", ph_addr);
	printf("Flags Virtual Page:\t%.3Xh\n", f_vp);
	
	// *** Start GAP 4 ***
	// Show ONLY the page table entry
	printf("Page Table Entry:\t%.3Xh\n", pte_value);
	// *** End GAP 4 ***
	
	
	printf("\nProcess Identifier (PID): %d\n", getpid());
	printf("\n---- Press [ENTER] to continue");
	getchar();
	
	return 0;
}
