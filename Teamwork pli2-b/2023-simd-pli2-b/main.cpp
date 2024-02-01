/*
 * Main.cpp
 *
 *  Created on: Fall 2019
 */

#include <stdio.h>
#include <immintrin.h> // Required to use intrinsic functions
#include <CImg.h>
#include <time.h>

using namespace cimg_library;

// Data type for image components
typedef float data_t;

const char* SOURCE_IMG      = "bailarina.bmp";

const char* SOURCE_IMG2      = "flores_3.bmp";

const char* DESTINATION_IMG = "solution.bmp";

const int MAX_PIXEL_COLOR=255;

const int MIN_PIXEL_COLOR=0;

const int ITEMS_PER_PACKET = (sizeof(__m256)/sizeof(data_t)); //It's 8 floats per packet

int PIXEL_COUNT= 0; //Total number of pixels
int nPackets =0; //Total number of packets

/**
 * Applies the filter for the photos for the remaining pixels (the part that is not multiple of ITEMS_PER_PACKET)
 * @param src is the pointer for the pixels of the first source image
 * @param src2 is the pointer for the pixels of the second source image
 * @param dst is the pointer for the pixels of the destination image
*/
void filterDataInExcess(data_t* src, data_t* src2, data_t* dst, float c) {
    int dataInExcess = (PIXEL_COUNT)%(sizeof(__m256)/sizeof(data_t)); //Number of pixels in excess (not multiple of ITEMS_PER_PACKET)

    // Calculate the addition for each pixel in excess
    for (int i =0; i< dataInExcess; i++){
        *(dst + nPackets * ITEMS_PER_PACKET + i) = *(src + nPackets * ITEMS_PER_PACKET + i)*c + *(src2 + nPackets * ITEMS_PER_PACKET + i)*(1-c);
    }
}

/**
 * Checks that a pixel has a correct value for the color (between the MIN_PIXEL_COLOR and the MAX_PIXEL_COLOR).
 * If it's out of bounds, then it sets it to the MAX_PIXEL_COLOR or MIN_PIXEL_COLOR
 * @param dst is the pointer to the pixel 
*/
void checkColorsInBounds(data_t* dst){
    for (int i =0; i< PIXEL_COUNT; i++){
        if(dst[i] >=MAX_PIXEL_COLOR)
            dst[i]=MAX_PIXEL_COLOR;
        if(dst[i] <=MIN_PIXEL_COLOR)
            dst[i]=MIN_PIXEL_COLOR;
    }
}

/**
 * Applies the filter for blending the 2 source images into the destination image
 * @param src is the pointer for the pixels of the first source image
 * @param src2 is the pointer for the pixels of the second source image
 * @param dst is the pointer for the pixels of the destination image
 * @param c is the blend level. It must be in range [0,1]
 * @throws an exception if the blend level is invalid
*/
void filter(data_t* src, data_t* src2, data_t* dst, float c){
    if (c<0 || c>1){ //check blend level is correct
		perror("The c parameter is invalid.");
		exit(-2);
	}

    __m256 vSrc, vSrc2;

    //Adding the elements by packets into dst by picking 8 floats successively from src and src2
    for (int i=0; i<nPackets;i++){
        vSrc = _mm256_loadu_ps(src + ITEMS_PER_PACKET*i); 
        vSrc2 = _mm256_loadu_ps(src2 + ITEMS_PER_PACKET*i);
        *(__m256 *)(dst + ITEMS_PER_PACKET*i) = _mm256_add_ps(vSrc*c, vSrc2*(1-c));
    }
    filterDataInExcess(src, src2, dst, c);
    checkColorsInBounds(dst);

}

int main() {
    try{
        CImg<data_t> srcImage(SOURCE_IMG);
        CImg<data_t> srcImage2(SOURCE_IMG2);

        //Initialization of the time variables
        struct timespec tStart, tEnd;
        double dElapsedTimeS;


        float blendLevel = 0.5;				//Initialization of the blend level for the filter

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
        
        PIXEL_COUNT = width*height*nComp; //Array size (total number of pixels)
        
        // Allocate src and src2 with the total number of pixels and alligend to 256 bits
        data_t *src = (data_t *)_mm_malloc(sizeof(data_t) * PIXEL_COUNT, sizeof(__m256));
        data_t *src2 = (data_t *)_mm_malloc(sizeof(data_t) * PIXEL_COUNT, sizeof(__m256));

        // Calculation of number of packets
        nPackets = (PIXEL_COUNT * sizeof(data_t)/sizeof(__m256));
    
        // Allocate dst with the total number of pixels and alligend to 256 bits
        data_t *dst = (data_t *)_mm_malloc(sizeof(data_t) * PIXEL_COUNT, sizeof(__m256));

        if (dst == NULL || src == NULL || src2 == NULL) {
            perror("Allocating images");
            exit(-2);
        }

        // Initialize data arrays
        data_t* data1 = srcImage.data();
        data_t* data2 = srcImage2.data();
        
        // Set the initial values of the dst with -1
        for (int i=0; i<= nPackets;i++){
            *((__m256 *)(dst + ITEMS_PER_PACKET*i)) = _mm256_set1_ps(-1);
        }

        //Initialize src and src2 with the data for each image 
        for (int i = 0; i < PIXEL_COUNT; i++) {
            src[i] = data1[i];
            src2[i] = data2[i];   
        }
    
        try{
            clock_gettime(CLOCK_REALTIME, &tStart);
            
            filter(src, src2, dst, blendLevel);
            filter(src, src2, dst, blendLevel);
            filter(src, src2, dst, blendLevel);
            filter(src, src2, dst, blendLevel);

            filter(src, src2, dst, blendLevel);
            filter(src, src2, dst, blendLevel);
            filter(src, src2, dst, blendLevel);
            filter(src, src2, dst, blendLevel);

            filter(src, src2, dst, blendLevel);
            filter(src, src2, dst, blendLevel);
            filter(src, src2, dst, blendLevel);
            filter(src, src2, dst, blendLevel);

            filter(src, src2, dst, blendLevel);
            filter(src, src2, dst, blendLevel);
            filter(src, src2, dst, blendLevel);
            filter(src, src2, dst, blendLevel);
            //filter(src, src2, dst, blendLevel);

            clock_gettime(CLOCK_REALTIME, &tEnd);	

            //Calculates the time difference between the start and the finish time
            dElapsedTimeS=(tEnd.tv_sec -tStart.tv_sec); 
            dElapsedTimeS+=(tEnd.tv_nsec -tStart.tv_nsec)/1e+9;

            printf("The time was: %f\n", dElapsedTimeS);
        }
        catch (int error){ //Throws an error if there was an error with clock_gettime method
            perror("Error calculating the time.");
            exit(-2);
        }

        //Create the destination image, save it and displays it
        CImg<float> dstImage(dst, width, height, 1, nComp);
        dstImage.save(DESTINATION_IMG);
        dstImage.display();

        //Free the variables allocated with _mm_malloc
        _mm_free(dst);
        _mm_free(src);
        _mm_free(src2);

    } catch (CImgException& e){ //Throws an error if there was an error by reading the source images
		perror("At least one of the source images is invalid. The program will be stopped now");
		exit(-2);
	}
	return 0;
}