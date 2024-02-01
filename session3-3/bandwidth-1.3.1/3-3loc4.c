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
        // Print address for first repetition
        // and the first 4 rows
        for(j = 0; j < NCOLS; j++)
        {
                for (i = 0; i < NROWS; i++)
                {
                        matrix[i][j] = 0;
			// Print address for first repetition
			// and the first 4 columns
			if (rep == 0 && j < 4)
			{
			    if (i == 0)
			        printf("Begin column\n");

			    printf("%p\n", &(matrix[i][j]));
			}
                }
        }
    }
}

