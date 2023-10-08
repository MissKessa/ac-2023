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

void swap(int* x, int* y)
{
  int temp = *x;
  *x = *y;
  *y = temp;
}

void sort(int array[], int start, int end)
{
  int l, r, p;
  if (start < end) {
    l = start +1;
    r = end;
    p = array[start];
    while (l < r) {
      if (array[l] <= p)
        l++;
      else if (array[r] >= p)
        r--;
      else
        swap(&array[l], &array[r]);
    }
    if (array[l] < p)
    {
      swap(&array[l], &array[start]);
      l--;
    }
    else
    {
      l--;
      swap(&array[l], &array[start]);
    }
    sort(array, start, l);
    sort(array, r, end);
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
  sort(vector1, 0, VECTOR_SIZE-1);

  return EXIT_SUCCESS;
}
