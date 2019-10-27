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
	
	// Getting original width of image
	int width = I1->width();
	int height = I1->height();
	int total = width * height;

	// Retrieving pointer to images
	ChannelPtr<uchar> p1, p2;
	int type;
	int ch = 0;
	IP_getChannel(I1, ch, p1, type);
	IP_getChannel(I2, ch, p2, type);

	// Filters can only be odd
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
	// Size is the width plus all the padding on bot sides
	int *allocMemory = new int[width + fullPaddingW]; 
	int i, j;

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

	delete allocMemory;


	// THIS PART OF THE CODE BROKE THE WIDTH NO CLUE
	// allocating memory to use as buffer
	//int *allocMemory2 = new int[newHeight]; // new int[width + fullPaddingW];

	//for (j = 0; j < width; j++) {
	//	// padding the left side of image to be the same as the first pixel of original image
	//	for (i = 0; i < paddingH; i++)
	//		*(allocMemory2 + i) = *(p1 + j * height);

	//	// copying the original image into buffer
	//	// offsetting image by the padding
	//	for (i = paddingH; i < height + (paddingH); i++)
	//		*(allocMemory2 + i) = *(p1 + (j*height) + i - paddingH);

	//	// padding the right side of image to be the same as the last pixel of original image
	//	for (i = height + (paddingH); i < fullPaddingH; i++)
	//		*(allocMemory2 + i) = *(p1 + (j*height) + height - 1);

	//	// start the blurring process
	//	for (i = paddingH; i < height + (paddingH); i++) {
	//		int count = 0;
	//		for (int k = i - paddingH; k < i + paddingH; k++)
	//			count += *(allocMemory2 + k);
	//		*(allocMemory2 + i) = count / filterH;
	//	}

	//	// gcopying data from buffer to output image
	//	for (i = 0; i < height; i++)
	//		*(p2 + j * height + i) = *(allocMemory2 + paddingH + i);  //*p1++; 
	//}

	//delete allocMemory2;

}
