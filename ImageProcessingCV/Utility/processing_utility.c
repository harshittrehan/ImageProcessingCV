//
//  processing_utility.c
//  ImageProcessingCV
//
//  Created by Harshit Trehan on 12/13/20.
//  Copyright © 2020 Harshit Trehan. All rights reserved.
//
#ifndef NETPBM
#define NETPBM
#include "../netpbm.h"
#endif

#ifndef UTIL
#define UTIL
#include "processing_utility.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <float.h>

float randomProbability(){
    return (float)rand() / (float)RAND_MAX;
}

char* concat(const char *s1, const char *s2) {
    const size_t len1 = strlen(s1);
    const size_t len2 = strlen(s2);
    char *result = (char *) malloc(sizeof(char)*(len1 + len2 + 1));
    
    memcpy(result, s1, len1);
    memcpy(result + len1, s2, len2 + 1);
    
    return result;
}

Image createBlackAndWhite(Image original, int threshold, int background, Image blackWhite) {
    int x;
    int y;
    
    if(background==0) {
        for(x=0; x<original.height; x++){
            for(y=0; y<original.width; y++){
                (original.map[x][y].i >= threshold) ? (blackWhite.map[x][y].i = 0) : (blackWhite.map[x][y].i = 255);
            }
        }
    }
    else {
        for(x=0; x<original.height; x++){
            for(y=0; y<original.width; y++){
                (original.map[x][y].i <= threshold) ? (blackWhite.map[x][y].i = 0) : (blackWhite.map[x][y].i = 255);
            }
        }
    }
    
    return blackWhite;
}

void deepCopy(Image original, Image copy){
    int x, y;
    for(x=0; x<original.height; x++){
        for(y=0; y<original.width; y++){
            copy.map[x][y].i = original.map[x][y].i;
        }
    }
}

Matrix getGaussian(double sigma, int kernelSize) {
    
    if(kernelSize%2==0){
        printf("%s", "Even kernels not supported. Incrementing kernel size by 1.");
        kernelSize++;
    }
    Matrix gKernel = createMatrix(kernelSize, kernelSize);
    int center = kernelSize/2;
    double s = 2.0 * sigma * sigma;

    double sum = 0.0;
    
    for (int x = -center; x <= center; x++) {
        for(int y = -center; y <= center; y++) {
            double r = sqrt(x*x + y*y);
            gKernel.map[x + center][y + center] = (exp(-(r*r)/s))/(PI * s);
            sum += gKernel.map[x + center][y + center];
        }
    }
    
    for(int i = 0; i < kernelSize; ++i)
        for(int j = 0; j < kernelSize; ++j)
            gKernel.map[i][j] /= sum;

    return gKernel;
}

Matrix convolve(Matrix m1, Matrix m2) {
    
    Matrix convolved = createMatrix(m1.height, m1.width);
    
    //Center coordinates for the filter
    int xCenter = m2.width / 2;
    int yCenter = m2.height / 2;
    
    //Iterating over the image matrix
    for(int x=0; x < m1.height; x++) {
        for(int y=0; y < m1.width; y++) {
            if(x==0 || x==m1.height-1 || y==0 || y==m1.width-1){
                convolved.map[x][y] = 0.0;
                continue;
            }
            //Iterating over the filter
            for(int m=0; m < m2.height; ++m) {
                for(int n=0; n < m2.width; ++n){
                    
                    //Indices of the filter
                    int filterM = m2.height - 1 - m;
                    int filterN = m2.width - 1 - n;
                    
                    //Indices of the image matrix
                    int imageX = x + (xCenter - filterM);
                    int imageY = y + (yCenter - filterN);
                    
                    if( imageX >= 0 && imageX < m1.height && imageY >= 0 && imageY < m1.width )
                        convolved.map[x][y] += m1.map[imageX][imageY] * m2.map[filterM][filterN];
                }
            }
        }
    }
    return convolved;
}

/*
 Function to scale given image matrix with given gamma value.
 Assuming here that in output, Max = 255 and Min = 0.
*/
Matrix scaleImage(Matrix m, double gamma){
    double max = -DBL_MAX;
    
    for(int x=0; x<m.height; x++){
        for(int y=0; y<m.width; y++){
            double val = (int)m.map[x][y];
            if (val > max){
                max = val;
            }
        }
    }
    
    for(int x=0; x<m.height; x++){
        for(int y=0; y<m.width; y++){
            double val = m.map[x][y];
            m.map[x][y] = 255.0*pow((val)/(max), gamma);
        }
    }
    
    return m;
}
