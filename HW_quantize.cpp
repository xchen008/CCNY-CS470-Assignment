#include "IP.h"
using namespace IP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_quantize:
//
// Quantize I1 to specified number of levels. Apply dither if flag is set.
// Output is in I2.
//
void
HW_quantize(ImagePtr I1, int levels, bool dither, ImagePtr I2)
{

	IP_copyImageHeader(I1, I2);

	int total = I1->width() * I1->height();
	int i, lut[MXGRAY];

	double scale = (double) MXGRAY / levels;
	double bias = scale / 2; // (MXGRAY / 2) / levels;

	for (i = 0; i < MXGRAY; i++) {
		lut[i] = (scale * (int)(i / scale)) + bias;
	}

	// declarations for image channel pointers and datatype
	ChannelPtr<uchar> p1, p2;
	int type;

	if (!dither) {
		// visit all image channels and evaluate output image
		for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {	// get input  pointer for channel ch
			IP_getChannel(I2, ch, p2, type);						// get output pointer for channel ch
			for (i = 0; i < total; i++) *p2++ = lut[*p1++];			// use lut[] to eval output
		}
	}
	else {
		int temp1, temp2, temp3;
		for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {	// get input  pointer for channel ch
			IP_getChannel(I2, ch, p2, type);						// get output pointer for channel ch
			for (i = 0; i < total; i++) {
					temp1 = ((double)rand() / RAND_MAX) * bias;
					temp3 = rand() % 2;
					temp3 == 0 ? temp2 = *p1++ + temp1 : temp2 = *p1++ - temp1;
					temp2 <= 0 ? 
						*p2++=lut[0] : temp2 >= 255 ?
						*p2++ = lut[255] : *p2++ = lut[temp2];
			}
		}
	}
}
