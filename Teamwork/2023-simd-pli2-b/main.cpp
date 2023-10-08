/*
 * Main.cpp
 *
 *  Created on: Fall 2019
 */

#include <stdio.h>
#include <immintrin.h> // Required to use intrinsic functions


// TODO: Example of use of intrinsic functions
// This example doesn't include any code about image processing


#define VECTOR_SIZE       18 // Array size. Note: It is not a multiple of 8
#define ITEMS_PER_PACKET (sizeof(__m256)/sizeof(float))


int main() {

	// Data arrays to sum. May be or not memory aligned to __m256 size (32 bytes)
    float a[VECTOR_SIZE], b[VECTOR_SIZE];

    // Calculation of the size of the resulting array
    // How many 256 bit packets fit in the array?
    int nPackets = (VECTOR_SIZE * sizeof(float)/sizeof(__m256));
   
    // Create an array aligned to 32 bytes (256 bits) memory boundaries to store the sum.
    // Aligned memory access improves performance    
    float *c = (float *)_mm_malloc(sizeof(float) * VECTOR_SIZE, sizeof(__m256));

    // 32 bytes (256 bits) packets. Used to stored aligned memory data
    __m256 va, vb; 

    // Initialize data arrays
    for (int i = 0; i < VECTOR_SIZE; i++) {
        *(a + i) = (float) i;       // a =  0, 1, 2, 3, …
        *(b + i) = (float) (2 * i); // b =  0, 2, 4, 6, …
    }

    // Set the initial c element's value to -1 using vector extensions
    *(__m256 *) c = _mm256_set1_ps(-1);
    *(__m256 *)(c + ITEMS_PER_PACKET)     = _mm256_set1_ps(-1);
    *(__m256 *)(c + ITEMS_PER_PACKET * 2) = _mm256_set1_ps(-1);

    // Data arrays a and b must not be memory aligned to __m256 data (32 bytes)
    // so we use intermediate variables to avoid execution errors.
    // We make an unaligned load of va and vb
    va = _mm256_loadu_ps(a);      // va = a[0][1]…[7] = 0, 1, 2, 3,  4,  5,  6,  7
    vb = _mm256_loadu_ps(b);      // vb = b[0][1]…[7] = 0, 2, 4, 6,  8, 10, 12, 14
    
    // Performs the addition of two aligned vectors, each vector containing 8 floats
    *(__m256 *)c = _mm256_add_ps(va, vb);// c = c[0][1]…[7] = 0, 3, 6, 9, 12, 15, 18, 21

    // Next packet
    // va = a[8][9]…[15] =  8,  9, 10, 11, 12, 13, 14, 15
    // vb = b[8][9]…[15] = 16, 18, 20, 22, 24, 26, 28, 30
    //  c = c[8][9]…[15] = 24, 27, 30, 33, 36, 39, 42, 45
    va = _mm256_loadu_ps((a + ITEMS_PER_PACKET)); 
    vb = _mm256_loadu_ps((b + ITEMS_PER_PACKET)); 
    *(__m256 *)(c + ITEMS_PER_PACKET) = _mm256_add_ps(va, vb);

    // If vectors va and vb have not a number of elements multiple of ITEMS_PER_PACKET 
    // it is necessary to differentiate the last iteration. 

    // Calculation of the elements in va and vb in excess
    int dataInExcess = (VECTOR_SIZE)%(sizeof(__m256)/sizeof(float));

    // Surplus data can be processed sequentially
    
    for (int i =0; i< dataInExcess; i++){
        *(c + 2 * ITEMS_PER_PACKET + i) = *(a + 2 * ITEMS_PER_PACKET + i) + *(b + 2 * ITEMS_PER_PACKET + i);
    }
    
    // Print resulting data from array addition
    for (int i = 0; i < VECTOR_SIZE; i++) {
        printf("\nc[%d]: %f", i, *(c + i));
	}
  
    // Free memory allocated using _mm_malloc
    // It has to be freed with _mm_free
    _mm_free(c);

	return 0;
}
