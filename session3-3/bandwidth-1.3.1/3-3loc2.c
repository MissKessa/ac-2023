#include <stdint.h>
#include <stdio.h>

#define NROWS    8192 // 2^13 rows
#define NCOLS    8192 // 2^13 cols
#define NTIMES   10   // Repeat 10 times

// Matrix size 2^26 = 64 MiBytes
uint8_t matrix[NROWS][NCOLS];

int main(void)
{
    int i, j, rep;

    for (rep = 0; rep < NTIMES; rep++)
    {
        for (i = 0; i < NROWS; i++)
        {
            for(j = 0; j < NCOLS; j++)
            {
                matrix[i][j] = 0;
		// Prints the address accessed in the first iteration
		// for the first 4 rows
		if (rep == 0 && i < 4)
		{
			if (j == 0)
	        		printf("Begin row\n");

			printf("%p\n", &(matrix[i][j]));
		}		
            }
        }
    }
}
