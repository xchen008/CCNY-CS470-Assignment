//Xue Ru Chen

#include "IP.h"
using namespace IP;
#include <cstdlib>  // for malloc

void gammaCorrection(ImagePtr, double, ImagePtr);
int *initializeErrorLUT(int, int);

 // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 // HW_errDiffusion:
 //
 // Apply error diffusion algorithm to image I1.
 //
 // This procedure produces a black-and-white dithered version of I1.
 // Each pixel is visited and if it + any error that has been diffused to it
 // is greater than the threshold, the output pixel is white, otherwise it is black.
 // The difference between this new value of the pixel from what it used to be
 // (somewhere in between black and white) is diffused to the surrounding pixel
 // intensities using different weighting systems.
 //
 // Use Floyd-Steinberg     weights if method=0.
 // Use Jarvis-Judice-Ninke weights if method=1.
 //
 // Use raster scan (left-to-right) if serpentine=0.
 // Use serpentine order (alternating left-to-right and right-to-left) if serpentine=1.
 // Serpentine scan prevents errors from always being diffused in the same direction.
 //
 // A circular buffer is used to pad the edges of the image.
 // Since a pixel + its error can exceed the 255 limit of uchar, shorts are used.
 //
 // Apply gamma correction to I1 prior to error diffusion.
 // Output is saved in I2.
 //
 void
 HW_errDiffusion(ImagePtr I1, int method, bool serpentine, double gamma, ImagePtr I2)
 {
   // Apply gamma correction to input image
   ImagePtr Inew;
   gammaCorrection(I1, gamma, Inew);

   IP_copyImageHeader(I1, I2);
   int w = I1->width ();
   int h = I1->height();

   int ch, type, x, y;
   int LUT[MXGRAY];
   ChannelPtr<uchar> p1, p2;

   
   // Error calculation variables
   int error, *Luterr, *TempPtr;
   int threshold = MXGRAY/2, bufferind = 0;
     
   // Buffer variables; some variables depend on the method.
   void *ptr;
   int *buffer, *In, *In1, *In2;
   int numOfRows = method+2;
   int rowPadding = numOfRows - 1;
   int rowWidthPlusPadding = w + (2 * rowPadding);
    
   
   for(int a = 0; a < MXGRAY; a++)
     {LUT[a] = (a < threshold) ? 0 : MaxGray;}

   // General variables
   int i, j, direction;

   Luterr = initializeErrorLUT(method, threshold);

   // Allocate buffer
   ptr = std::malloc(sizeof(int) * numOfRows * rowWidthPlusPadding);
   buffer = (int *)ptr;

   // For each channel
   for(ch = 0; IP_getChannel(Inew, ch, p1, type); ch++) {
     IP_getChannel(I2, ch, p2, type);
    /*numOfRows = method + 2;
    rowPadding = method + 1;
    bufferind = 0;
    rowWidthPlusPadding = w + (2 * rowPadding);*/
       
    

     // Insert row into buffer
    for(i = 0, j = rowPadding; i < w; i++, j++)
        {buffer[((bufferind % numOfRows) * rowWidthPlusPadding) + j] = p1[i];}
       
    bufferind++; p1 += w;

     if (method) {
       // Insert another row into buffer
       for(i = 0, j = rowPadding; i < w; i++, j++)
            {buffer[((bufferind % numOfRows) * rowWidthPlusPadding) + j] = p1[i];}
         
       bufferind++; p1 += w;
     }

     for(y = 0; y < h; y++) {
       // Insert row into buffer
        if(y+rowPadding < h){
           for(i = 0, j = rowPadding; i < w; i++, j++)
                {buffer[((bufferind % numOfRows) * rowWidthPlusPadding) + j] = p1[i];}
            
           p1 += w;
         }bufferind++; 
         

       /* Set pixel traversal direction;
       1 means left-to-right. -1 means right-to-left.
       */
       direction = (serpentine && (y % 2)) ? -1 : 1;

       // Access rows in buffer
       if (direction == -1) {
         In = buffer + ((bufferind % numOfRows) * rowWidthPlusPadding) + rowPadding + w - 1;
         In1 = buffer + (((bufferind + 1) % numOfRows) * rowWidthPlusPadding) + rowPadding + w - 1;
         if (method) {
           In2 = buffer + (((bufferind + 2) % numOfRows) * rowWidthPlusPadding) + rowPadding + w - 1;
         }
       }
       else {
         In = buffer + ((bufferind % numOfRows) * rowWidthPlusPadding) + rowPadding;
         In1 = buffer + (((bufferind + 1) % numOfRows) * rowWidthPlusPadding) + rowPadding;
         if (method) {
           In2 = buffer + (((bufferind + 2) % numOfRows) * rowWidthPlusPadding) + rowPadding;
         }
       }

       for(x = 0; x < w; x++) {
         // Apply threshold using Look up table
          // *p2 = LUT[*In];
        *p2 = (*In < threshold) ? 0 : MaxGray;



         // Compute error produced by thresholding
          error = *In - *p2;

         /* Spread error around to neightboring pixels;
       */
           if (!method) {
             TempPtr = Luterr + 4 * error;
             In[ 1 * direction ] +=  TempPtr[0]    ;
               
             In1[-1 * direction ] += TempPtr[1];
             In1[ 0 * direction ] += TempPtr[2];
             In1[ 1 * direction ] += TempPtr[3];
           }
           else {
             TempPtr = Luterr + 12 * error;
             In[ 1 * direction ] += TempPtr[0];
             In[ 2 * direction ] += TempPtr[1];
               
             In1[-2 * direction ] += TempPtr[2];
             In1[-1 * direction ] += TempPtr[3];
             In1[ 0 * direction ] += TempPtr[4];
             In1[ 1 * direction ] += TempPtr[5];
             In1[ 2 * direction ] += TempPtr[6];
               
             In2[-2 * direction ] += TempPtr[7];
             In2[-1 * direction ] += TempPtr[8];
             In2[ 0 * direction ] += TempPtr[9];
             In2[ 1 * direction ] += TempPtr[10];
             In2[ 2 * direction ] += TempPtr[11];
           }

         // Advance pointers
         if (direction == -1) {
            In--; In1--; p2--;
           
            if (method) In2--;
         }
         else {
           In++; In1++; p2++;
             
           if (method) In2++;
         }
       } // end x loop

       if      (serpentine && (direction == -1)) p2 += (w + 1);
       else if (serpentine && (direction ==  1)) p2 += (w - 1);
     } // end y loop
   } // end channel loop

     
     if(!method) Luterr -= 4*threshold;
     
     else Luterr -= 12*threshold;
     
     free(Luterr); free(buffer);

}




int *
initializeErrorLUT(int method, int threshold)
{
  void *ptr;
  int *TempPtr, *Luterr, i;

    if(!method){
    // Allocate error LUT
        ptr = std::malloc(sizeof(int) * MXGRAY * 4);
        Luterr = (int *)ptr;

        // Fill error LUT
        for(i = -threshold, TempPtr = Luterr; i < threshold; i++, TempPtr += 4) {
          TempPtr[0] = i * (7/16.);
          TempPtr[1] = i * (3/16.);
          TempPtr[2] = i * (5/16.);
          TempPtr[3] = i * (1/16.);
        }

        // Shift pointer to middle of LUT
        Luterr += 4*threshold;
  }

  // Jarvis-Judice-Ninke method
    else if(method == 1){
    // Allocate error LUT
        ptr = std::malloc(sizeof(int) * MXGRAY * 12);
        Luterr = (int *)ptr;

        // Fill error LUT
        for(i = -threshold, TempPtr = Luterr; i < threshold; i++, TempPtr += 12) {
          TempPtr[0] = i * (7/48.);
          TempPtr[1] = i * (5/48.);
          TempPtr[2] = i * (3/48.);
          TempPtr[3] = i * (5/48.);
          TempPtr[4] = i * (7/48.);
          TempPtr[5] = i * (5/48.);
          TempPtr[6] = i * (3/48.);
          TempPtr[7] = i * (1/48.);
          TempPtr[8] = i * (3/48.);
          TempPtr[9] = i * (5/48.);
          TempPtr[10] = i * (3/48.);
          TempPtr[11] = i * (1/48.);
        }

        // Shift pointer to middle of LUT
        Luterr += 12*threshold;
  }
  return Luterr;
}

 // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 // gammaCorrect:
 //
 // Apply gamma correction to image I1.
 // Save result in I2.
 //
 void
 gammaCorrection(ImagePtr I1, double gamma, ImagePtr I2)
 {
   IP_copyImageHeader(I1, I2);
      int total = I1->width() * I1->height();
      double lut[MXGRAY];
      int i;

      // Populating the look up table
      // Formula for gamma correction
      for (i = 0; i < MXGRAY; i++) {
          lut[i] = (pow((i/255.0), (1/gamma)) * 255.0);
      }

      ChannelPtr<uchar> p1, p2;
      int type;

      // visit all image channels and evaluate output image
      for (int ch = 0; IP_getChannel(I1, ch, p1, type); ch++) {    // get input  pointer for channel ch
          IP_getChannel(I2, ch, p2, type);        // get output pointer for channel ch
          for (i = 0; i < total; i++) *p2++ = lut[*p1++];    // use lut[] to eval output
      }
 }

