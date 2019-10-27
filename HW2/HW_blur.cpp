// Meldin Kajoshaj - HW2 - CSc 470

#include "IP.h"
using namespace IP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_blur:
//
// Blur image I1 with a box filter (unweighted averaging).
// The filter has width filterW and height filterH.
// We force the kernel dimensions to be odd.
// Output is in I2.
//
void
HW_blur(ImagePtr I1, int filterW, int filterH, ImagePtr I2)
{
	IP_copyImageHeader(I1, I2);

	// Retrieving pointer to images
	ChannelPtr<uchar> p1, p2;
	int type;
	int ch = 0;
	IP_getChannel(I1, ch, p1, type);
	IP_getChannel(I2, ch, p2, type);
	
	// Getting original width of image
	int width = I1->width();
	int height = I1->height();
	int total = width * height;

	// Forcing Filter to be odd
	int filterW2, filterH2;

	if (filterW % 2 == 0)	filterW2 = filterW + 1;
	else	filterW2 = filterW;

	if (filterH % 2 == 0)	filterH2 = filterH + 1;
	else	filterH2 = filterH;

	// Getting new size of padded image
	int newWidth = I1->width() + filterW2;
	int newHeight = I1->height() + filterH2;
	int newTotal = newWidth * newHeight;

	// Padding size for each side of the array
	int paddingW = filterW2 / 2;
	int paddingH = filterH2 / 2;

	// Padding added to the array as a whole
	int fullPaddingW = (paddingW) * 2;
	int fullPaddingH = (paddingH) * 2;

	// Allocating memory to use as buffer. 
	// Size depends on which part of the dimensions of the image is larger
	int memorySize;

	if (width + fullPaddingW > height + fullPaddingH)
		memorySize = width + fullPaddingW;
	else
		memorySize = height + fullPaddingH;

	int *allocMemory = new int[memorySize]; 
	int i, j;

	/* FIRST pass through the image */

	// Determining which row to work on
	for (j = 0; j < height; j++) {
		// Padding the left side of image to be the same as the first pixel of original image
		for (i = 0; i < paddingW; i++)
			*(allocMemory + i) = *(p1 + j*width);
	
		// Copying the original image into buffer
		// Offsetting image by the padding
		for (i = paddingW; i < width + (paddingW); i++) 
			*(allocMemory + i) = *(p1 + (j*width) + i - paddingW);

		// Padding the right side of image to be the same as the last pixel of original image
		for (i = width + (paddingW); i < width + fullPaddingW; i++)
			*(allocMemory + i) = *(p1 + (j*width) + width - 1);

		// Start the blurring process
		for (i = paddingW; i < width + (paddingW); i++) {
			int count = 0;
			for (int k = i - paddingW; k < i + paddingW + 1; k++) { count += *(allocMemory + k); }
			*(allocMemory + i) = count / filterW2;
		}

		// Copying data from buffer to output image
		for (i = 0; i < width; i++)
			*(p2 + j * width + i) = *(allocMemory + paddingW + i);
	}

	/* SECOND pass through the image */

	// Determining which column to work on
	for (j = 0; j < width; j++) {
		// Padding the top side of image to be the same as the first pixel of original image
		for (i = 0; i < paddingH; i++)
			*(allocMemory + i) = *(p2 + j);

		// Copying the original image into buffer
		// Offsetting image by the padding
		for (i = paddingH; i < height + (paddingH); i++)
			*(allocMemory + i) = *(p2 + ((i - paddingH)*width) + j);

		// Padding the bottom side of image to be the same as the last pixel of original image
		for (i = height + (paddingH); i < height + fullPaddingH; i++)
			*(allocMemory + i) = *(p2 + ((height-1)*width) + j); 

		// Start the blurring process
		for (i = paddingH; i < height + (paddingH); i++) {
			int count = 0;
			for (int k = i - paddingH; k < i + paddingH + 1; k++) 
				count += *(allocMemory + k); 
			*(allocMemory + i) = count / filterH2;
		}

		// Copying data from buffer to output image
		for (i = 0; i < height; i++)
			*(p2 + i*width + j) = *(allocMemory + paddingH + i);
	}

	delete allocMemory;

}
