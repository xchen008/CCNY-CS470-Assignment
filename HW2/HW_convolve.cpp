// Meldin Kajoshaj - HW2 - CSc 470

#include "IP.h"
using namespace IP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_convolve:
//
// Convolve I1 with filter kernel in Ikernel.
// Output is in I2.
//
void
HW_convolve(ImagePtr I1, ImagePtr Ikernel, ImagePtr I2)
{
	IP_copyImageHeader(I1, I2);

	// Retrieving pointer to images
	ChannelPtr<uchar> p1, p2;
	ChannelPtr<float> pKernel;
	int type;
	int ch = 0;
	IP_getChannel(I1, ch, p1, type);
	IP_getChannel(I2, ch, p2, type);

	// Dimensions of original image
	int width = I1->width();
	int height = I1->height();
	int total = width * height;

	// Dimensions of kernel
	int kLength = Ikernel->width();
	int kTotal = kLength * kLength;

	// Padding size for each side of the array based on kernel size
	int halfPadding = kLength / 2;
	
	// Padding added to the array as a whole
	int fullPadding = halfPadding * 2;

	// Find new size of the image
	int newWidth = width + fullPadding;
	int newHeight = height + fullPadding;

	// Creating a buffer image large enough to hold the original image plus padding
	int memorySize = newWidth * newHeight;
	unsigned char *allocMemory = new unsigned char[memorySize];

	// temporary variables
	int i, j, k, g;
	double count;

	/* Filling up the buffer image */
	
	// Filling up the top padding area
		//The rows up until the actual image
	for (j = 0; j < halfPadding; j++) {
		// Filling up the top-left corner
		for (i = 0; i < halfPadding; i++) 
			*(allocMemory + j * width + i) = *(p1 + j * width);
		
		// Filling up the top-middle
		for (i = halfPadding; i < width + halfPadding; i++) 
			*(allocMemory + j * width + i) = *(p1 + j * width + i);

		// Filling up the top-right corner
		for (i = width + halfPadding; i < newWidth; i++) 
			*(allocMemory + j * width + i) = *(p1 + j * width + width - 1);
	}

	// Filling up the middle part of the image
	//		The rows that are the actual height of the original image
	for (j = halfPadding; j < height+halfPadding; j++) {
		// left padding adjacent to the original image
		for (i = 0; i < halfPadding; i++) 
			*(allocMemory + j * newWidth + i) = *(p1 + j * width);

		// Adding the actual image
		for (i = halfPadding; i < width + halfPadding; i++)
			*(allocMemory + j * newWidth + i) = *(p1 + (j-halfPadding) * width + (i-halfPadding));

		// right padding adjacent to the original image
		for (i = width + halfPadding; i < newWidth; i++)
			*(allocMemory + j * newWidth + i) = *(p1 + j * width + width - 1);
	}

	// Filling up the bottom padding
	for (j = height+halfPadding; j < newHeight; j++) {
		// Filling up the bottom-left corner
		for (i = 0; i < halfPadding; i++)
			*(allocMemory + j * width + i) = *(p1 + j * width);

		// Filling up the bottom-middle
		for (i = halfPadding; i < width + halfPadding; i++)
			*(allocMemory + j * width + i) = *(p1 + j * width + i);

		// Filling up the bottom-right corner
		for (i = width + halfPadding; i < newWidth; i++)
			*(allocMemory + j * width + i) = *(p1 + j * width + width - 1);
	}

	/* CONVOLUTION */

	// choosing the row
	for (j = halfPadding; j < height + halfPadding; j++) {
		// choosing the column
		for (i = halfPadding; i < width + halfPadding; i++) {
			// Keeping track of the sum of products
			count = 0;
			
			// Resetting the pointer to the beginning of the kernel
			IP_getChannel(Ikernel, 0, pKernel, type);

			// kernel height
			for (k = j - halfPadding; k < j + halfPadding + 1; k++) {
				// kernel width
				for (g = i - halfPadding; g < i + halfPadding + 1; g++) 
					count += *pKernel++ * *(allocMemory + k * newWidth + g); 
			}
			*(p2 + (j - halfPadding) * width + (i - halfPadding)) = CLIP(count, 0, 255);
		}
	}
}
