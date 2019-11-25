//
//  HW3_dft1D.cpp
//
//
//  Created by XueruChen on 11/24/19.
//

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define PI 3.1415927
#define MAG(i)        (sqrt(Fr[i]*Fr[i] + Fi[i]*Fi[i]))
#define PHASE(i)    (atan2(Fi[i],Fr[i]))

typedef struct {
    int len;      /* length of complex number list */
    float *real;  /* pointer to real number list */
    float *imag;  /* pointer to imaginary number list */
} complexS, *complexP;

int main(int argc, char *argv[]) {
    FILE *in, *out;
    int dir;

    if(argc == 4) {
        char *in_file = argv[1];
        dir = atoi(argv[2]);
        char *out_file = argv[3];
        in = fopen(in_file, "r");
        out = fopen(out_file, "w");
    }
    else {
        printf("Please enter three arguments:\n");
        return 0;
    }
    
    
    int u, x, h ,w ;
    fscanf(in, "%d %d",&w, &h);
    int N = h;
    double c, s, real, imag, cos(), sin();
    
    complexP i1 = malloc(sizeof(*i1));
    complexP i2 = malloc(sizeof(*i2));
    i1->len = N;
    i1->real = malloc(sizeof(float)*N);
    i1->imag = malloc(sizeof(float)*N);
    i2->len = N;
    i2->real = malloc(sizeof(float)*N);
    i2->imag = malloc(sizeof(float)*N);

    for (int i = 0; i < N; i++) {
        fscanf(in, "%f %f", &i1->real[i], &i1 -> imag[i]);
    }
    
    if(!dir){
        for(u=0; u<N; u++) {        /* compute spectrum over all freq u */
            real = imag = 0;    /* reset real, imag component of F(u) */
            for(x=0; x<N; x++) {    /* visit each input pixel */
                c =  cos(2.*PI*u*x/N);
                s = -sin(2.*PI*u*x/N);
                real += (i1 -> real[x]*c - i1 -> imag[x]*s);
                imag += (i1 -> real[x]*s + i1 -> imag[x]*c);
            }
            i2 -> real[u] = real / N;
            i2 -> imag[u] = imag / N;
        }
        //write to output
        int wrt = fprintf(out,"2 %d\n",N);
        for(int i = 0; i < N; i++) {
            wrt = fprintf(out, "%f %f\n", i2 -> real[i],  i2 -> imag[i]);
        }
        if(wrt < 0) printf("Unable to write to file. \n");
    }

    else{
        for(x=0; x<N; x++) {        /* compute each output pixel */
            real = imag = 0;
            for(u=0; u<N; u++) {
                c = cos(2.*PI*u*x/N);
                s = sin(2.*PI*u*x/N);
                real += (i1 -> real[u]*c - i1 -> imag[u]*s);
                imag += (i1 -> real[u]*s + i1 -> imag[u]*c);
            }
            i2 -> real[x] = real;
            i2 -> imag[x] = imag;
        }
        //write to outout
        int wrt = fprintf(out,"2 %d\n",N);
        for(int i = 0; i < N; i++) {
            wrt = fprintf(out, "%f %f\n", i2 -> real[i],  i2 -> imag[i]);
        }
        if(wrt < 0) printf("Unable to write to file. \n");
    }
    
    
    free(i1);
    free(i2);
    
    fclose(in);
    fclose(out);
    return 0;
}
