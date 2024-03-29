#include <algorithm>
#include <vector>
#include "IP.h"
using namespace IP;
using std::vector;
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_median:
//
// Apply median filter of size sz x sz to I1.
// Clamp sz to 9.
// Output is in I2.
//
void
HW_median(ImagePtr I1, int sz, ImagePtr I2)
{
    IP_copyImageHeader(I1, I2);
    int w = I1->width ();
    int h = I1->height();
    int ch, type, i, j, a ,b;
    
    ChannelPtr<uchar> p1, p2;
    
    int padding = (sz-1)/2;
    
    //creating a padding array so we can have padding around the image
    int buffer[h + 2 * padding][w + 2 * padding];
    //use 0 and 255 to alternative because we will have a min and a max and therefore should balance out the effect of padding
    /*for(i = 0; i < w + 2 * padding; i++){
        for(j = 0; j < h + 2 * padding; j++){
            buffer[i][j] = (i+j)%2 ? 0 : 255;
        }
    }*/
    
    
    for(ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {
    IP_getChannel(I2, ch, p2, type);
        //store image in the middle
        /*for(i = padding; i < w + padding; i++){
            for(j = padding; j < h + padding; j++){
                buffer[i][j] = *p1++;
            }
        }*/
        // Filling up the top padding area
        //The rows up until the actual image
        for (j = 0; j < padding; j++) {
            // Filling up the top-left corner
            for (i = 0; i < padding; i++)
                buffer[j][i]= *(p1);
            
            // Filling up the top-middle
            for (i = padding; i < w + padding; i++)
                buffer[j][i] = *(p1 + i - padding);

            // Filling up the top-right corner
            for (i = w + padding; i < w + 2 * padding; i++)
                buffer[j][i] = *(p1 + w - 1);
        }

        // Filling up the middle part of the image
        //        The rows that are the actual height of the original image
        for (j = padding; j < h + padding; j++) {
            // left padding adjacent to the original image
            for (i = 0; i < padding; i++)
                buffer[j][i] = *(p1 + (j - padding) * w);

            // Adding the actual image
            for (i = padding; i < w + padding; i++)
                buffer[j][i] = *(p1 + (j - padding) * w + (i - padding));

            // right padding adjacent to the original image
            for (i = w + padding; i < w + 2*padding; i++)
                buffer[j][i] = *(p1 + (j-padding) * w - 1);
        }

        // Filling up the bottom padding
        for (j = h+padding; j < h + 2*padding; j++) {
            // Filling up the bottom-left corner
            for (i = 0; i < padding; i++)
                buffer[j][i] = *(p1 + (h - 1) * w);

            // Filling up the bottom-middle
            for (i = padding; i < w + padding; i++)
                buffer[j][i] = *(p1 + (h-1) * w + i - padding);

            // Filling up the bottom-right corner
            for (i = w + padding; i < w + 2*padding; i++)
                buffer[j][i] = *(p1 + h * w);
        }
       //create an vector for all the numbers around so we can sort it and return the median
        vector<int> vec;
        for(i = padding; i < w + padding; i++){
            for(j = padding; j < h + padding; j++){
                //now find all the nums around the pixel to then find median
                for(a = -1 * padding; a < padding + 1; a++){
                    for(b = -1 * padding; b < padding + 1; b++){
                        vec.push_back(buffer[i+a][j+b]);
                    }
                }
                //update p2

                nth_element(vec.begin(), vec.begin() + (vec.size()/2), vec.end());
                *p2++ = vec[(vec.size()/2)];//findmedian(vec);
                vec.clear();
            }
        }
    }//end channal
    
    
    
    
}//end function

