#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>

/////////////////////////////////////////////////////////
// UNCOMMENT FOR MULTITHREADED VERSION
/////////////////////////////////////////////////////////
//#define MULTI_THREADED

/////////////////////////////////////////////////////////
// CHANGE IT WITH YOUR OWN ID
/////////////////////////////////////////////////////////
const unsigned int DNI = 7178934;


#define TRUE 1
#define FALSE 0

const unsigned int N = 650;
void Task(unsigned int);

void MatrixMultiplication(unsigned int, double **, double **, double **);
double ** MatrixInitialization(unsigned int, int, unsigned int);
void DestroyMatrix(unsigned int, double **);

#ifdef MULTI_THREADED
#include <pthread.h>


	void * ThreadJob(void * arg)
	{
		unsigned long nElements = *((unsigned long*)arg);
		Task(nElements);
		return NULL;
	}
#endif


int main(int argc, char* argv[])
{
	unsigned long nElements = DNI/50000;
	unsigned int nThreads = DNI%10;
	
	struct timespec tStart, tEnd;
	double dElapsedTimeS;
	
	#ifdef MULTI_THREADED
	if (nThreads<2) 
		nThreads = 7;
	printf("Running with %d thread(s)...\n", nThreads);
	#endif

	printf("Running task    : ");
	
	//Start time measurement
	clock_gettime(CLOCK_REALTIME, &tStart);

	
#ifdef MULTI_THREADED
	pthread_t Threads[nThreads];
	// Initializing threads
	for (int i = 0; i < nThreads; i++)
	{
		int pthread_ret = pthread_create(&Threads[i], NULL, ThreadJob, &nElements);
		if (pthread_ret)
		{
			fprintf(stderr, "ERROR: pthread_create error code: %d.\n", pthread_ret);
			exit(EXIT_FAILURE);
		}
	}

	// Wait until all threads are finished
	for (int i = 0; i < nThreads; i++)
	{
		pthread_join(Threads[i], NULL);
	}
#else
	// Executed in the same thread as the program
	Task(nElements);
#endif

	// Finish time measurement
	clock_gettime(CLOCK_REALTIME, &tEnd);

	// Show the execution time
	dElapsedTimeS = (tEnd.tv_sec - tStart.tv_sec);
	dElapsedTimeS += (tEnd.tv_nsec - tStart.tv_nsec) / 1e+9;
	
	return 0;
}

void Task(unsigned int nElements)
{
	double	** matrixA,
		** matrixB,
		** matrixResult;


	// Initialization
	matrixA = MatrixInitialization(nElements, TRUE, time(NULL));
	matrixB = MatrixInitialization(nElements, TRUE, time(NULL));
	matrixResult = MatrixInitialization(nElements, FALSE, 0);
	if (!matrixA || !matrixB || !matrixResult)
	{
		fprintf(stderr, "ERROR in Task: Cannot allocate memory\n");
		exit(EXIT_FAILURE);
	}

	// Multiplication
	MatrixMultiplication(nElements, matrixA, matrixB, matrixResult);

	// Desctruction
	DestroyMatrix(nElements, matrixA);
	DestroyMatrix(nElements, matrixB);
 	DestroyMatrix(nElements, matrixResult);
}

double getRandom(unsigned int min, unsigned int max)
{
	// Pseudo random numbers in the interval [min,max]
	return min + (max - min) * ((double)rand()/(double)RAND_MAX);
}


double ** MatrixInitialization(unsigned int nElements, int random, unsigned int seed)
{
	double ** matrix = (double **) malloc(nElements * sizeof(double *));
	if (!matrix)
		return NULL;

	for (int i = 0; i < nElements; i++)
	{
		matrix[i] = (double *) malloc(nElements * sizeof(double));
		if (!matrix)
			return NULL;

		if (random)
		{
			for (int j = 0; j < nElements; j++)
			{
				// Pseudo random numbers in the interval [1.0-2.0]
				matrix[i][j] = getRandom(1,2);
			}
		}
	}

	return matrix;
}

void MatrixMultiplication(unsigned int nElements, double ** A, double ** B, double ** R)
{
	for (int i = 0; i < nElements; i++)
	{
		for (int j = 0; j < nElements; j++)
		{
			R[i][j] = 0;
			for (int k = 0; k < nElements; k++)
			{
				R[i][j] += A[i][k] * B[k][j];
			}
		}
	}
}

void DestroyMatrix(unsigned int nElements, double ** M)
{
	for (int i = 0; i < nElements; i++)
	{
		free(M[i]);
	}
    free(M);
}
