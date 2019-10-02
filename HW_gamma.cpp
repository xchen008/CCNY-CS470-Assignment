#include "IP.h"
using namespace IP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_gammaCorrect:
//
// Gamma correct image I1. Output is in I2.
//
void
HW_gammaCorrect(ImagePtr I1, double gamma, ImagePtr I2)
{
	IP_copyImageHeader(I1, I2);
	int total = I1->width() * I1->height();
	double lut[MXGRAY];
	int i;

	for (i = 0; i < MXGRAY; i++) {
		lut[i] = (pow((i/255.0), (1/gamma)) * 255.0);
	}

	ChannelPtr<uchar> p1, p2;
	int type;

	// visit all image channels and evaluate output image
	for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {	// get input  pointer for channel ch
		IP_getChannel(I2, ch, p2, type);		// get output pointer for channel ch
		for (i = 0; i < total; i++) *p2++ = lut[*p1++];	// use lut[] to eval output
	}
}
