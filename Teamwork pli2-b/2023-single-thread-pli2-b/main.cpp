/*
* Main.cpp
*
* Created on: Fall 2019
*/

#include <stdio.h>
#include <math.h>
#include <CImg.h>
#include <time.h>

using namespace cimg_library;

// Data type for image components
typedef float data_t;

//Name of the file that is one of the source images
const char* SOURCE_IMG      = "bailarina.bmp";

//Name of the file that is the other source images
const char* SOURCE_IMG2      = "flores_3.bmp";

//Name of the file of the destination image
const char* DESTINATION_IMG = "solution.bmp";

//Maximum value for a color pixel
const int MAX_PIXEL_COLOR=255;
//Minimum value for a color pixel
const int MIN_PIXEL_COLOR=0;

// Filter argument data type
typedef struct {
	// Pointer to the Red component of the first source image
	data_t *pRsrc;
	// Pointer to the Green component of the first source image
	data_t *pGsrc;
	// Pointer to the Blue component of the first source image
	data_t *pBsrc;

	// Pointer to the Red component of the second source image
	data_t *pRsrc2;
	// Pointer to the Green component of the second source image
	data_t *pGsrc2;
	// Pointer to the Blue component of the second source image
	data_t *pBsrc2;

	// Pointer to the Red component of the destination image
	data_t *pRdst;
	// Pointer to the Green component of the destination image
	data_t *pGdst;
	// Pointer to the Blue component of the destination image
	data_t *pBdst;

	// Size of the image in pixels
	uint pixelCount; 
} filter_args_t;

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
 * @param pSrc is the pointer to one color component of one pixel from the source image 2
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
 * @throws an exception if the blend level is invalid
*/
void filter (filter_args_t args, float c) {
	if (c<0 || c>1){
		perror("The c parameter is invalid.");
		exit(-2);
	}
    for (uint i = 0; i < args.pixelCount; i++) {
		addColors((args.pRsrc + i),(args.pRsrc2 + i),(args.pRdst + i),c);
		// *(args.pRdst + i) = *(args.pRsrc + i)*c + *(args.pRsrc2 + i)*(1-c);
		// checkColumnsInBound(*(args.pRdst + i));

		addColors((args.pGsrc + i),(args.pGsrc2 + i),(args.pGdst + i),c);
		// *(args.pGdst + i) = *(args.pGsrc + i)*c + *(args.pGsrc2 + i)*(1-c);
		// checkColumnsInBound(*(args.pGdst + i));

		addColors((args.pBsrc + i),(args.pBsrc2 + i),(args.pBdst + i),c);
		// *(args.pBdst + i) = *(args.pBsrc + i)*c + *(args.pBsrc2 + i)*(1-c);
		// checkColumnsInBound(*(args.pBdst + i));
	}

}

int main() {
	try{
		// Open file and object initialization
		CImg<data_t> srcImage(SOURCE_IMG);
		CImg<data_t> srcImage2(SOURCE_IMG2);
		filter_args_t filter_args;
		data_t *pDstImage; 					// Pointer to the new image pixels

		//Initialization of the time variables
		struct timespec tStart, tEnd;
		double dElapsedTimeS;

		float blendLevel = 0.5;				//Initialization of the blend level for the filter
		srcImage.display(); 				// Displays the first source image
		uint width = srcImage.width();		// Gets the width of the first source image
		uint height = srcImage.height();	// Gets the height of the first source image
		uint nComp = srcImage.spectrum(); 	// Gets the number of components of the first source image

		srcImage2.display();				// Displays the second source image
		uint width2 = srcImage2.width();	// Gets the width of the second source image
		uint height2 = srcImage2.height();	// Gets the height of the second source image
		uint nComp2 = srcImage2.spectrum(); // Gets the number of components of the second source image

		//Check that both images have the same size
		if (width!=width2 || height!=height2 || nComp!=nComp2){ 
			perror("The images do not match.");
			exit(-2);
		}

		// Calculating image size in pixels
		filter_args.pixelCount = width * height;
		
		// Allocate memory space for destination image components
		pDstImage = (data_t *) malloc (filter_args.pixelCount * nComp * sizeof(data_t));
		//Check the destination image is not null
		if (pDstImage == NULL) {
			perror("Allocating destination image.");
			exit(-2);
		}

		// Initialize the pointers to the componet arrays of the first source image
		filter_args.pRsrc = srcImage.data();
		filter_args.pGsrc = filter_args.pRsrc + filter_args.pixelCount;
		filter_args.pBsrc = filter_args.pGsrc + filter_args.pixelCount;

		// Initialize the pointers to the componet arrays of the second source image
		filter_args.pRsrc2 = srcImage2.data();
		filter_args.pGsrc2 = filter_args.pRsrc2 + filter_args.pixelCount;
		filter_args.pBsrc2 = filter_args.pGsrc2 + filter_args.pixelCount;
		
		// Pointers to the RGB arrays of the destination image
		filter_args.pRdst = pDstImage;
		filter_args.pGdst = filter_args.pRdst + filter_args.pixelCount;
		filter_args.pBdst = filter_args.pGdst + filter_args.pixelCount;

		try{
			clock_gettime(CLOCK_REALTIME, &tStart);	//Measures the time at which starts the filter

			//Repeat the function call until we get a time between 5 and 10
			filter(filter_args, blendLevel);
			filter(filter_args, blendLevel);
			filter(filter_args, blendLevel);
			filter(filter_args, blendLevel);

			filter(filter_args, blendLevel);
			filter(filter_args, blendLevel);
			filter(filter_args, blendLevel);
			filter(filter_args, blendLevel);

			filter(filter_args, blendLevel);
			filter(filter_args, blendLevel);
			filter(filter_args, blendLevel);
			filter(filter_args, blendLevel);

			filter(filter_args, blendLevel);
			filter(filter_args, blendLevel);
			filter(filter_args, blendLevel);
			filter(filter_args, blendLevel);

			clock_gettime(CLOCK_REALTIME, &tEnd);	//Measures the time at which finishes the filter

			//Calculates the time difference between the start and the finish time
			dElapsedTimeS=(tEnd.tv_sec -tStart.tv_sec); 
			dElapsedTimeS+=(tEnd.tv_nsec -tStart.tv_nsec)/1e+9;

			printf("The time was: %f\n", dElapsedTimeS); //Prints the time it took to execute the filter
		} catch (int error){ //Throws an error if there was an error with clock_gettime method
			perror("Error calculating the time.");
			exit(-2);
		}

		//Creates the destination images
		CImg<data_t> dstImage(pDstImage, width, height, 1, nComp);

		// Store destination image in disk
		dstImage.save(DESTINATION_IMG);

		// Display destination image
		dstImage.display();
		
		// Free memory
		free(pDstImage);
	} catch (CImgException& e){ //Throws an error if there was an error by reading the source images
		perror("At least one of the source images is invalid. The program will be stopped now");
		exit(-2);
	}

	return 0;
}
