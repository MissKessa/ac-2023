#include <stdio.h>
#include <atc/linmem.h>
// Given a virtual address, the values of the flags of the page
// are printed on the screen. On top of these, the title passed
// as argument is also displayed.
void print_pte_flags(void *virtual_addr, char *title) {
	unsigned int pte;
	unsigned int flags_vp;
	const unsigned int FLAG_MASK = 0x7F;
	const unsigned int PRESENT_BIT_MASK = 0x1;
	const unsigned int WRITE_BIT_MASK = 0x2;
	const unsigned int USER_BIT_MASK = 0x4;
	const unsigned int WRITE_THROUGH_BIT_MASK = 0x8;
	const unsigned int CACHE_DISABLED_BIT_MASK = 0x10;
	const unsigned int ACCESSED_BIT_MASK = 0x20;
	const unsigned int DIRTY_BIT_MASK = 0x40;

	// Print the title
	printf("%s\n", title);
	// Retrieve the entry in the page table for the virtual address
	if (get_pte(virtual_addr, &pte)) {
		perror("Linmem module error");
		return;
	}
	// Is there PTE?
	if (pte){ // OK
		// Store the flags associated at the memory page
		flags_vp = pte & FLAG_MASK;
		////////
		/// Print the status of the flags associated to the page
		////////
		if (flags_vp & PRESENT_BIT_MASK)
            		printf("Present bit = 1\tPage present in memory\n");
       		else
		        printf("Present bit = 0\tPage is not present in memory\n");

        	if ((flags_vp & WRITE_BIT_MASK) == WRITE_BIT_MASK)
            		printf("Write bit = 1\tPage can be read and written\n");
       		else
	          	printf("Write bit = 0\tPage can be only read\n");

        	if ((flags_vp & USER_BIT_MASK) == USER_BIT_MASK)
            		printf("User bit = 1\tPage can be accessed by the task\n");
	        else
            		printf("User bit = 0\tPage is only for supervisor mode\n");

        	if ((flags_vp & WRITE_THROUGH_BIT_MASK) == WRITE_THROUGH_BIT_MASK)
            		printf("Write through bit = 1\tPage uses a write-through policy\n");
        	else
            		printf("Write through bit = 0\tPage uses a write-back policy\n");

        	if ((flags_vp & CACHE_DISABLED_BIT_MASK) == CACHE_DISABLED_BIT_MASK)
            		printf("Cache disabled bit = 1\tThe cache is disabled\n");
        	else
            		printf("Cache disabled bit = 0\tThe cache is enabled\n");

        	if ((flags_vp & ACCESSED_BIT_MASK) == ACCESSED_BIT_MASK)
            		printf("Accessed bit = 1\tPage has been accessed\n");
        	else
            		printf("Accessed bit = 0\tPage hasn't been accessed\n");

        	if ((flags_vp & DIRTY_BIT_MASK) == DIRTY_BIT_MASK)
            		printf("Dirty bit = 1\tPage has been written\n");
        	else
            		printf("Dirty bit = 0\tPage hasn't been written\n");

	} else {
		fprintf(stderr, "Page %.5Xh does not have a page table entry\n",
			(unsigned int)virtual_addr >> 12);
	}
}
int main(void) {
	int local = 0x0804866F;
	print_pte_flags(&local, "Local variable\n-----------------");
	printf("\n---- Press [ENTER] to continue");
	getchar();
	return 0;
}
