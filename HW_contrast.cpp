#include "IP.h"
using namespace IP;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_contrast:
//
// Apply contrast enhancement to I1. Output is in I2.
// Stretch intensity difference from reference value (128) by multiplying
// difference by "contrast" and adding it back to 128. Shift result by
// adding "brightness" value.
//
void
HW_contrast(ImagePtr I1, double brightness, double contrast, ImagePtr I2)
{
    // copy image header (width, height) of input image I1 to output image I2
    IP_copyImageHeader(I1,I2);
    
    // init vars for width, height, and total number of pixels
    int w = I1 -> width();
    int h = I1 -> height();
    int total = w * h;
    
    // init lookup table
    int i, lut[MXGRAY];
    
    // declarations for image channel pointers and datatype
    ChannelPtr<uchar> p1, p2;
    int type;
    
    /*finding max gray and min gray
    int min = 255, max = 0;
    for(int ch=0; IP_getChannel(I1, ch, p1, type); ch++) {
        for(i=0; i<total; i++)
        {
            if(*p1 < min) min = *p1;
            else if(*p1 > max) max = *p1++;
        }
    }*/
    
    //generate lut table
   
    for(i = 0; i < MXGRAY; i++)
        lut[i] = MIN(MAX(128 + brightness + (i-128)*contrast,0),255);
    
    
    // visit all image channels and evaluate output image
    for(int ch=0; IP_getChannel(I1, ch, p1, type); ch++) {    // get input  pointer for channel ch
        IP_getChannel(I2, ch, p2, type);
            // get output pointer for channel ch
        for(i=0; i<total; i++) *p2++ = lut[*p1++];    // use lut[] to eval output
    }
    
}
