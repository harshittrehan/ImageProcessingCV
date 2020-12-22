//
//  hough_transformer.c
//  ImageProcessingCV
//
//  Created by Harshit Trehan on 12/12/20.
//  Copyright © 2020 Harshit Trehan. All rights reserved.
//

#ifndef NETPBM
#define NETPBM
#include "../netpbm.h"
#endif

#ifndef HOUGH
#define HOUGH
#include "hough_transformer.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <float.h>

/*
 Function to create 3D parameter structure for Hough
 transform and allocate memory.
 */
OutputParams_3D create3DMatrix(int height, int width, int radius) {
    
    OutputParams_3D matrix;
    
    matrix.height = height;
    matrix.width = width;
    matrix.radius = radius;
    matrix.map= (double ***) malloc(sizeof(double **)*height);
    
    for(int i = 0; i<height; i++){
        matrix.map[i]= (double **) malloc(sizeof(double *)*width);
        for(int j = 0; j<width; j++){
            matrix.map[i][j] = (double *)malloc(sizeof(double)*radius);
            for(int k=0; k<radius; k++){
                matrix.map[i][j][k] = 0.0;
            }
        }
    }
    
    return matrix;
}

/*
 Function to delet the Hough parameter matrix and
 free the memory.
 */
void delete_3DMatrix(OutputParams_3D matrix) {
    
    for(int i = 0; i<matrix.height; i++) {
        for(int j=0; j<matrix.width; j++) {
            free(matrix.map[i][j]);
        }
        free(matrix.map[i]);
    }
    free(matrix.map);
}

// Build a Hough parameter map for matrix mxSpatial for detecting straight lines.
// Rows in this map represent the normal alpha and columns represent the distance d from the origin.
// Increasing the size of the map in each dimension improves the resolution of the corresponding parameter.
OutputParams_3D houghTransformCircle(Matrix mxSpatial, int radius, Matrix outputImg) {
    //3D Output space for Hough space parameters
    OutputParams_3D mxParam = create3DMatrix(mxSpatial.height, mxSpatial.width, radius);
    
    // Generate lookup table for sin and cos values to speed up subsequent computation.
    Matrix sinCosMap = createMatrix(360, 2);
    for (int angle = 0; angle < 360; angle++) {
        double alpha = angle * PI/180;
        sinCosMap.map[angle][0] = (double) sin(alpha);
        sinCosMap.map[angle][1] = (double) cos(alpha);
    }
    
    // Iterating through the edge image
    for (int m = 0; m < mxSpatial.height; m++) {
        for (int n = 0; n < mxSpatial.width; n++) {
            //If edge encountered
            if(mxSpatial.map[m][n] == 255) {
                /*
                 Parametric equation for circle:
                 x = a + R*sin(theta)
                 y = b + R*cos(theta)
                 (a,b): center of circle
                 (x,y): point on perimetr of circle
                 R: radius
                 Theta: angle that sweeps from 0 to 360.
                 */
                for(int r = 10; r<radius/2; r++) {
                    for (int angle = 0; angle < 360; angle++) {
                        int a = (int)(m - (double)r*sinCosMap.map[angle][0]);
                        int b = (int)(n - (double)r*sinCosMap.map[angle][1]);
                        if(a>0 && b>0 && a<mxSpatial.height && b<mxSpatial.width){
                            //Accumulating votes at given coordinates (a,b,R)
                            mxParam.map[a][b][r] += mxSpatial.map[m][n];
                            //Accumulating strength to print the Hough image
                            outputImg.map[a][b] += mxSpatial.map[m][n];
                        }
                    }
                }
            }
        }
    }

    deleteMatrix(sinCosMap);
    return mxParam;
}

// Test whether entry (m, n) in matrix mx is a local maximum, i.e., is not exceeded by any of its
// maximally 8 neighbors. Return 1 if true, 0 otherwise.
int isLocalMaximum(OutputParams_3D mx, int m, int n, int r) {
    double strength = mx.map[m][n][r];
    int i, j, k;
    int iMin = (m == 0) ? 0 : (m - 1);
    int iMax = (m == mx.height -1) ? m : (m + 1);
    int jMin = (n == 0) ? 0 : (n - 1);
    int jMax = (n == mx.width -1) ? n : (n + 1);
    int kMin = (r == 0) ? 0 : (r-1);
    int kMax = (r == mx.radius-1) ? r : (r+1);
    
    for (i = iMin; i <= iMax; i++) {
        for (j = jMin; j <= jMax; j++) {
            for (k = kMin; k <= kMax; k++) {
                if (mx.map[i][j][k] > strength) {
                    return 0;
                }
            }
        }
    }
    return 1;
}

// Insert a new entry, consisting of vPos, hPos, and strength, into the list of maxima mx.
void insertMaxEntry(Matrix  mx, int vPos, int hPos, int radius, double strength, int index) {
    mx.map[0][index] = (double) vPos;
    mx.map[1][index] = (double) hPos;
    mx.map[2][index] = (double) radius;
    mx.map[3][index] = strength;
}

// Delete entry number i from the list of maxima mx.
void deleteMaxEntry(Matrix mx, int i) {
    
    if(i==mx.width-1){
        return;
    }
    for (int n = mx.width-1; n>i; n--)
        for (int m = 0; m < 4; m++)
            mx.map[m][n] = mx.map[m][n - 1];

}

// Find the <number> highest maxima in a Hough parameter map that are separated by a Euclidean distance
// of at least <minSeparation> in the map. The result is a three-row matrix with each column representing
// the row, the column, and the strength of one maximum, in descending order of strength.
Matrix findHoughMaxima(OutputParams_3D mx, int number, double minSeparation) {
    int index, do_not_insert;
    double minSepSquare = SQR(minSeparation);
    double strength;
    Matrix maximaMatrix = createMatrix(4, number);
    
    //Initializing the Hough maxima matrix
    for (int j = 0; j < number; j++) {
        maximaMatrix.map[3][j] = -1.0;
    }
    
    //Iterating through 3D output parameters
    for (int k = 0; k < mx.height; k++) {
        for (int m = 0; m < mx.width; m++) {
            for (int n = 0; n < mx.radius; n++) {
                strength = mx.map[k][m][n];
                index = 0;
                do_not_insert = 0;
                double dist = 0.0;
                //Checking if the current point is local maxima and greater than the currently last (smallest) maxima
                if (strength > 0.0 && strength > maximaMatrix.map[3][number - 1] && isLocalMaximum(mx, k, m, n)){
                    //Finding the index where the new maxima should be inserted
                    while(strength < maximaMatrix.map[3][index]) {
                        //Distance should be larger than the given minimum spearation
                        dist = SQR(maximaMatrix.map[0][index] - (double) k) + SQR(maximaMatrix.map[1][index] - (double) m) +
                               SQR(maximaMatrix.map[2][index] - (double) n);
                        
                        // If distance is less then do not insert
                        if(dist < minSepSquare) {
                            do_not_insert = 1;
                            break;
                        }
                        index++;
                    }
                    if(!do_not_insert && index < number){
                        //Deleting entry at index and inserting new entry over there
                        deleteMaxEntry(maximaMatrix, index);
                        insertMaxEntry(maximaMatrix, k, m, n, strength, index);
                    }
                }
            }
        }
    }
    return maximaMatrix;
}
