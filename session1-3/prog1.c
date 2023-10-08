#include <stdlib.h>

#define VECTOR_SIZE 100000

#define BOOLEAN int
#define TRUE 1
#define FALSE 0

void fillWithRandomNumbers(int array[], int n) {
  int i;

  // Random generator initialization
  static int seed = 0;
  srand(seed++);

  // Generate numbers between 1 and 1000
  for (i = 0; i < n ; i++) 
    array[i] = rand() % 1000 + 1;
}

void arraySort(int* vector, int vectorSize)
{
  int i, j, iMin, temp;

  // Selection sort method
  for (i = 0; i < vectorSize; i++)
  {
    for (j = i+1; j < vectorSize; j++)
    {
      iMin = i;
      if (vector[j] < vector[iMin])
      {
        iMin = j;
      }
      if (iMin != i) {
        temp = vector[i];
        vector[i] = vector[iMin];
        vector[j] = temp;
      }
    }
  }
}

BOOLEAN isInArray(int num, int vector[], int vectorSize) {
  int i;

  for (i = 0; i < vectorSize; i++) { 
    if (vector[i] == num) { 
      return TRUE;
    }
  }
  return FALSE;
}

BOOLEAN isPrime(int n) {
  int i;

  for (i = 2; i < n; i++) {
    if (n % i == 0) {
      return FALSE;
    }
  }
  return TRUE;
}

int countSharedPrimes(int vector1[], int vectorSize1, int vector2[], int vectorSize2) {
  int sharedPrimeCount = 0;
  int i;

  for (i = 0; i < vectorSize1; i++) { 
    if (isInArray(vector1[i], vector2, vectorSize2) && isPrime(vector1[i])) {
      sharedPrimeCount++;
    }
  }

  return sharedPrimeCount;
}
  
int main() {

  int vector1[VECTOR_SIZE];
  int vector2[VECTOR_SIZE];

  fillWithRandomNumbers(vector1, VECTOR_SIZE);
  fillWithRandomNumbers(vector2, VECTOR_SIZE);

  countSharedPrimes(vector1, VECTOR_SIZE, vector2, VECTOR_SIZE);
  arraySort(vector1, VECTOR_SIZE);

  return EXIT_SUCCESS;
}

