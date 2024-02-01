/*
 * Main.cpp
 *
 *  Created on: Fall 2019
 */

#include <stdio.h>
#include <math.h>
#include <pthread.h>
#include <CImg.h>
#include <time.h>

using namespace cimg_library;

typedef float data_t;

const char* SOURCE_IMG      = "bailarina.bmp";
const char* SOURCE_IMG2      = "flores_3.bmp";
const char* DESTINATION_IMG = "solution.bmp";

const int MAX_PIXEL_COLOR=255;
const int MIN_PIXEL_COLOR=0;

const int NUM_THREADS=4; //Number of threads of the processor
pthread_t thread[NUM_THREADS]; //Thread array
double times[NUM_THREADS]; //Array for the time that it takes to execute the task for each thread

// Filter argument data type
typedef struct {
	// Pointers to the color components of the first source image
	data_t *pRsrc;
	data_t *pGsrc;
	data_t *pBsrc;

	// Pointer to the color components of the second source image
	data_t *pRsrc2;
	data_t *pGsrc2;
	data_t *pBsrc2;

	// Pointer to the color components of the destination image
	data_t *pRdst;
	data_t *pGdst;
	data_t *pBdst;

	uint pixelCount; // Size of the image in pixels
} filter_args_t;

filter_args_t filter_args; //Arguments of the filter
float blendLevel; //Blend level for the images
float numberOfPackets; //Number of elements processed by each thread

/**
 * Checks that a pixel has a correct value for the color (between the MIN_PIXEL_COLOR and the MAX_PIXEL_COLOR).
 * If it's out of bounds, then it sets it to the MAX_PIXEL_COLOR or MIN_PIXEL_COLOR
 * @param pDstColor is the pointer to the pixel 
*/
void checkColorsInBound(data_t &pDstColor){
	if ((pDstColor)>MAX_PIXEL_COLOR){
		(pDstColor)=MAX_PIXEL_COLOR;
	}
	if ((pDstColor)<MIN_PIXEL_COLOR){
		(pDstColor)=MIN_PIXEL_COLOR;
	}	
}

/**
 * Applies the filter algorithm for each pixel
 * @param pSrc is the pointer to one color component of one pixel from the source image 
 * @param pSrc2 is the pointer to one color component of one pixel from the source image 2
 * @param pDst is the pointer to one color component of one pixel from the destination image 
 * @param c is the blend level. It must be in range [0,1]
*/
void addColors(data_t* pSrc, data_t* pSrc2, data_t* pDst, float c){
	*pDst = *(pSrc)*c + *(pSrc2)*(1-c);
	checkColorsInBound(*pDst);
}

/**
 * Applies the filter for blending the 2 source images into the destination image
 * @param args is the filter argument data type that contains all the pointers for each color for each image
 * @param c is the blend level. It must be in range [0,1]
 * @param numberThread is the number that identifies the thread and will determine which positions will process
 * @throws an exception if the blend level is invalid
*/
void filter (filter_args_t args, float c, int numberThread) {
	//Checks that c is correct
	if (c<0 || c>1){
		perror("The c parameter is invalid.");
		exit(-2);
	}

	//Calculates the maximum position that the thread will process
	uint maxPixel = (uint)numberOfPackets*numberThread + (uint)numberOfPackets;
	if (numberThread== NUM_THREADS-1){
		maxPixel= filter_args.pixelCount;
	}

	//Applies the filter for the minimum position and maximum position that the thread will process
    for (uint i = numberThread*(int)numberOfPackets; i < maxPixel; i++) {
		addColors((args.pRsrc + i),(args.pRsrc2 + i),(args.pRdst + i),c);
		addColors((args.pGsrc + i),(args.pGsrc2 + i),(args.pGdst + i),c);
		addColors((args.pBsrc + i),(args.pBsrc2 + i),(args.pBdst + i),c);
	}

}

/**
 * It's the task for each thread. It applies the filter to the images and calculates the time that it takes for each thread to do the task and stores it in the times array
 * @param i is the number for the thread that determines which positions of the images it will process 
*/
void * ThreadJob(void * i) {
	struct timespec tStartThread, tEndThread;
	double dElapsedTimeSThread;
	try{
		clock_gettime(CLOCK_REALTIME, &tStartThread);	//Measures the time at which starts the filter
		int numberThread = *((int *) i);
		filter(filter_args, blendLevel, numberThread);
		filter(filter_args, blendLevel, numberThread);
		filter(filter_args, blendLevel, numberThread);
		filter(filter_args, blendLevel, numberThread);

		filter(filter_args, blendLevel, numberThread);
		filter(filter_args, blendLevel, numberThread);
		filter(filter_args, blendLevel, numberThread);
		filter(filter_args, blendLevel, numberThread);

		filter(filter_args, blendLevel, numberThread);
		filter(filter_args, blendLevel, numberThread);
		filter(filter_args, blendLevel, numberThread);
		filter(filter_args, blendLevel, numberThread);
		
		filter(filter_args, blendLevel, numberThread);
		filter(filter_args, blendLevel, numberThread);
		filter(filter_args, blendLevel, numberThread);
		filter(filter_args, blendLevel, numberThread);
		
		
		clock_gettime(CLOCK_REALTIME, &tEndThread);	//Measures the time at which finishes the filter
		delete (int *)i;
		//Calculate the time it took to do the task and stores it in the times array
		dElapsedTimeSThread=(tEndThread.tv_sec -tStartThread.tv_sec); 
		dElapsedTimeSThread+=(tEndThread.tv_nsec -tStartThread.tv_nsec)/1e+9;
		times[numberThread]=dElapsedTimeSThread;
		return NULL;

	} catch (int error){ //Throws an error if there was an error with clock_gettime method
		perror("Error calculating the time.");
		exit(-2);
	}
}

int main() {
	try{	
		CImg<data_t> srcImage(SOURCE_IMG);
		CImg<data_t> srcImage2(SOURCE_IMG2);

		//Initialization of the time variables
		struct timespec tStart, tEnd;
		double dElapsedTimeS;

		blendLevel = 0.5;				//Initialization of the blend level for the filter

		srcImage.display(); 				
		uint width = srcImage.width();		
		uint height = srcImage.height();	
		uint nComp = srcImage.spectrum(); 	

		srcImage2.display();				
		uint width2 = srcImage2.width();	
		uint height2 = srcImage2.height();	
		uint nComp2 = srcImage2.spectrum();

		//Check that both images have the same size
		if (width!=width2 || height!=height2 || nComp!=nComp2){ 
			perror("The images do not match.");
			exit(-2);
		}

		filter_args.pixelCount = width * height; // Calculating image size in pixels

		// Allocate memory space for the destination image components
		data_t* pDstImage = (data_t *) malloc (filter_args.pixelCount * nComp * sizeof(data_t));

		//Check the destination image is not null
		if (pDstImage == NULL) {
			perror("Allocating destination image.");
			exit(-2);
		}

		// Initialize the pointers to the components arrays of the first source image
		filter_args.pRsrc = srcImage.data();
		filter_args.pGsrc = filter_args.pRsrc + filter_args.pixelCount;
		filter_args.pBsrc = filter_args.pGsrc + filter_args.pixelCount;

		// Initialize the pointers to the components arrays of the second source image
		filter_args.pRsrc2 = srcImage2.data();
		filter_args.pGsrc2 = filter_args.pRsrc2 + filter_args.pixelCount;
		filter_args.pBsrc2 = filter_args.pGsrc2 + filter_args.pixelCount;
		
		// Initialize the pointers to the components arrays of the destination image
		filter_args.pRdst = pDstImage;
		filter_args.pGdst = filter_args.pRdst + filter_args.pixelCount;
		filter_args.pBdst = filter_args.pGdst + filter_args.pixelCount;

		numberOfPackets = filter_args.pixelCount/NUM_THREADS;
		try{
			clock_gettime(CLOCK_REALTIME, &tStart);	//Measures the time at which starts the creation of the threads

			//Creates the threads according to the NUM_THREADS and to each it assigns the variable i as the number of thread
			for (int i = 0; i < NUM_THREADS; i++ ) {
				int *numberThread = new int(i);
				if (pthread_create( &thread[i], NULL, ThreadJob, (void *)numberThread) != 0) {
					fprintf(stderr, "ERROR: Creating thread %d\n", i);
					sleep(1);
				}
			}

			clock_gettime(CLOCK_REALTIME, &tEnd);	//Measures the time at which finishes the creation of the threads

		} catch (int error){ //Throws an error if there was an error with clock_gettime method
			perror("Error calculating the time.");
			exit(-2);
		}

		// Joins the threads
		for ( int i = 0; i < NUM_THREADS; i++ ) {
			pthread_join(thread[i], NULL );
		} 

		//Calculates the time difference between the start and the finish time for the creation of the threads
		dElapsedTimeS=(tEnd.tv_sec -tStart.tv_sec); 
		dElapsedTimeS+=(tEnd.tv_nsec -tStart.tv_nsec)/1e+9;
		
		//Calculates the maximum time from all the threads
		double maxTimeThread=times[0];
		for (int i=1; i<NUM_THREADS;i++){
			if (times[i]>maxTimeThread)
				maxTimeThread=times[i];
		}

		//Adds the maximum time from all the threads to the time for creating the threads 
		dElapsedTimeS+=maxTimeThread;

		printf("The time was: %f\n", dElapsedTimeS); //Prints the time it took to execute the filter
		

		//Creates the destination image, saves it and displays it
		CImg<data_t> dstImage(pDstImage, width, height, 1, nComp);
		dstImage.save(DESTINATION_IMG);
		dstImage.display();
		
		// Free memory
		free(pDstImage);
	} catch (CImgException& e){ //Throws an error if there was an error by reading the source images
		perror("At least one of the source images is invalid. The program will be stopped now");
		exit(-2);
	}
	return 0;
}
