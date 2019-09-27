#include "IP.h"
using namespace IP;

void histoMatchApprox(ImagePtr, ImagePtr, ImagePtr);

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HW_histoMatch:
//
// Apply histogram matching to I1. Output is in I2.
//
void
HW_histoMatch(ImagePtr I1, ImagePtr targetHisto, bool approxAlg, ImagePtr I2)
{
	if(approxAlg) {
		histoMatchApprox(I1, targetHisto, I2);
		return;
	}

    int i, p, R;
    int left[MXGRAY], right[MXGRAY];
    int total, Hsum, Havg, h1[MXGRAY],save[MXGRAY];
    double scale;
    /* total number of pixels in image */
    IP_copyImageHeader(I1,I2);
    total = (long) I1->height() * I1->width();
    /* init I2 dimensions and buffer */
    
    int type;
    ChannelPtr<uchar> in, out;
    ChannelPtr<int> h2;
    
    
    for(int ch = 0; IP_getChannel(I1, ch, in, type); ch++) {
    IP_getChannel(I2, ch, out, type);
    IP_getChannel(targetHisto,ch, h2, type);
    
        for(i=0; i<MXGRAY; i++) h1[i] = 0; /* clear histogram */
        
        for(i=0; i<total; i++) h1[in[i]]++; /* eval input histogram */
    
    
        /* normalize h2 to conform with dimensions of I1 */
        for(i=Havg=0; i<MXGRAY; i++) Havg += h2[i];
        
        scale = (double) total / Havg;
        
        if(scale != 1) for(i=0; i<MXGRAY; i++) h2[i] *= scale;
        
        R = 0;
        Hsum = 0;
        /* evaluate remapping of all input gray levels;
        Each input gray value maps to an interval of valid output values.
        The endpoints of the intervals are left[] and right[] */
        for(i=0; i<MXGRAY; i++) {

            //i is the intensity level for h1(input histg) and R is intensity level for h2(target histg)
            left[i] = R; /* left end of interval */
            Hsum += h1[i]; /* cumulative value for interval */
            while(Hsum>h2[R]&&(R<MXGRAY-1)) { /* compute width of interval */
                Hsum -= h2[R];
                R++;
            }
            
            //saving the number of intensity level that needs to be reserve for pervious level
            //Hsum is the remainder
            save[i] = Hsum;
            right[i] = R;
    
        }
        
        /* adjust Hsum as interval widens */ /* update */
        /* init right end of interval */
        /* clear h1 and reuse it below */
        for(i=0; i<MXGRAY; i++) h1[i] = 0;
        /* visit all input pixels */
        for(i=0; i<total; i++) {
            
            //in[i] is the intensity level
         
            p = left[in[i]];
            //saving some spot for the previous intensity level
            /* mapping satisfies h2 */
            int a = in[i];
            //when the gray level is at their final/right end of the interval, we have to take out the "reserved seats" for it and intensity after it that is also saving the seat for it.
            while(p == right[a]) save[a++]--;
            
            //when we are black or already finish filling up the gray that needs to be reserve and moved to the next gray level, we dont need to worry about saving seats
            if((in[i] == 0 || left[in[i]] != right[in[i]-1]) && h1[p] < h2[p])
               out[i] = p;
            
            //make sure we dont over take seat for previous gray level
            else if((h1[p] + save[in[i]-1]) < h2[p])
                out[i] = p;
            //update left because the gray lvl is filled and time to move to the next gray lvl
            else out[i] = p = left[in[i]] = MIN(p+1, right[in[i]]);
            //increment histg
            h1[p]++;
        }
    }
    
}





void
histoMatchApprox(ImagePtr I1, ImagePtr targetHisto, ImagePtr I2)
{
    int i, p, R;
    int left[MXGRAY], right[MXGRAY];
    int total, Hsum, Havg, h1[MXGRAY];
    double scale;
    /* total number of pixels in image */
    IP_copyImageHeader(I1,I2);
    total = (long) I1->height() * I1->width();
    /* init I2 dimensions and buffer */
    
    int type;
    ChannelPtr<uchar> in, out;
    ChannelPtr<int> h2;
    
    
    for(int ch = 0; IP_getChannel(I1, ch, in, type); ch++) {
    IP_getChannel(I2, ch, out, type);
    /* target histogram */
    IP_getChannel(targetHisto,ch, h2, type);
    
        for(i=0; i<MXGRAY; i++) h1[i] = 0; /* clear histogram */
        for(i=0; i<total; i++) h1[in[i]]++; /* eval input histogram */
    
    
        /* normalize h2 to conform with dimensions of I1 */
        for(i=Havg=0; i<MXGRAY; i++) Havg += h2[i];
        scale = (double) total / Havg;
        if(scale != 1) for(i=0; i<MXGRAY; i++) h2[i] *= scale;
        
        R = 0;
        Hsum = 0;
        /* evaluate remapping of all input gray levels;
        Each input gray value maps to an interval of valid output values.
        The endpoints of the intervals are left[] and right[] */
        for(i=0; i<MXGRAY; i++) {

            left[i] = R; /* left end of interval */
            Hsum += h1[i]; /* cumulative value for interval */
            while(Hsum>h2[R] &&  R<MXGRAY-1) { /* compute width of interval */
                Hsum -= h2[R];
                R++;
            }
            right[i] = R;
        }
        /* adjust Hsum as interval widens */ /* update */
        /* init right end of interval */
        /* clear h1 and reuse it below */
        for(i=0; i<MXGRAY; i++) h1[i] = 0;
        /* visit all input pixels */
        for(i=0; i<total; i++) {
         
            p = left[in[i]];
            if(h1[p] < h2[p]) /* mapping satisfies h2 */
                    out[i] = p;
            else
                out[i] = p = left[in[i]] = MIN(p+1, right[in[i]]);
            
            h1[p]++;
        }
        }
}
