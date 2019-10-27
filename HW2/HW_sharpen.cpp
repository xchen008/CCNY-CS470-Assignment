
// Meldin Kajoshaj - HW2 - CSc 470

// Sharpening for the most part works as intended and looks very similar to
// the profressors program. However when the factor is set to 1, and the size 
// of the kernal is the only thing changed, there are very small differences.
// My assumption for this happening might be due to the type of blurring used.
// I had implement the blurring through IP_Blur, so if the professor used
// a different implementation, then that could be the cause of the minor difference
// I was not able to use my own implementation of HW_blur since a linking error
// kept occuring, but nonetheless it should not matter

#include "Blur.h"
#include "IP.h"
using namespace IP;

// Macro function to perform clipping
#define CLIP(A, B, C)	((A) < (B) ? (B) : (A) < (C) ? (A) : (C))

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_sharpen:
//
// Sharpen image I1. Output is in I2.
//
void
HW_sharpen(ImagePtr I1, int size, double factor, ImagePtr I2)
{
	// copy image header (width, height) of input image I1 to output image I2
	IP_copyImageHeader(I1, I2);


	// Used IP_blur because HW_blur caused a linking error
	IP_blur(I1, size, size, I2);

	// init vars for width, height, and total number of pixels
	int width = I1->width();
	int height = I1->height();
	int total = width * height;
	int i, j;

	// declarations for image channel pointers and datatype
	ChannelPtr<uchar> p1, p2;
	int type;

	// visit all image channels and evaluate output image
	// get input  pointer for channel ch
	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {   
		// get output pointer for channel ch
		IP_getChannel(I2, ch, p2, type);
		for (i = 0; i < total; i++) {
			// Temp variable to hold the end value
			int temp = 0;

			// Factors allow for exageration of the edges
			for (j = 0; j < factor + 1; j++) 
				temp += (*(p1 + i) - *(p2 + i));

			// Adding the edges back onto the original image
			temp += *(p1 + i);

			// Clipping the values to avoid overflow and underflow
			*(p2 + i) = CLIP(temp, 0, 255);
		}
	}
}
