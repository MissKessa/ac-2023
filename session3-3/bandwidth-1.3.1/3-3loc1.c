#include <stdint.h>

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
            }
        }
    }
}
