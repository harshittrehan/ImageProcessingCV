//
//  edge_detector.c
//  ImageProcessingCV
//
//  Created by Harshit Trehan on 12/12/20.
//  Copyright © 2020 Harshit Trehan. All rights reserved.
//

#ifndef NETPBM
#define NETPBM
#include "../netpbm.h"
#endif

#ifndef UTIL
#define UTIL
#include "../Utility/processing_utility.h"
#endif

#ifndef EDGE
#define EDGE
#include "edge_detector.h"
#endif

#include <math.h>
/*
 This function takes an image and finds the edges
 using Sobel filters and returns an image with
 the binarized edges.
 */
Image sobel(Image img, double magnScale, double threshold){
    
    //Converting smoothed image to image matrix
    Matrix imageMatrix = image2Matrix(img);
    
    //Horizontal gradient filter
    double dh[3][3] = {
        {1.0, 0.0, -1.0},
        {2.0, 0.0, -2.0},
        {1.0, 0.0, -1.0}
    };
    
    //Vertical gradient filter
    double dv[3][3] = {
        {1.0, 2.0, 1.0},
        {0.0, 0.0, 0.0},
        {-1.0, -2.0, -1.0}
    };
    
    //Creating matrices from filter values
    Matrix Sh = createMatrix(3, 3);
    Matrix Sv = createMatrix(3, 3);
    for(int x=0; x<3; x++){
        for(int y=0; y<3; y++){
            Sh.map[x][y] = dh[x][y];
            Sv.map[x][y] = dv[x][y];
        }
    }
    
    //Calculating the horizontal and vertical gradients for each pixel
    Matrix horizontalGradient = convolve(imageMatrix, Sh);
    Matrix verticalGradient = convolve(imageMatrix, Sv);
    
    //Deleting unneccessary matrices
    deleteMatrix(Sh);
    deleteMatrix(Sv);
    deleteMatrix(imageMatrix);

    //This matrix stores the magnitudes of the gradient
    //values for each (i,j) pixel in the image.
    Matrix magnitude = createMatrix(img.height, img.width);
    for(int x=0; x<magnitude.height; x++){
        for(int y=0; y<magnitude.width; y++){
            magnitude.map[x][y] = sqrt ( (horizontalGradient.map[x][y]*horizontalGradient.map[x][y]) +
                                           (verticalGradient.map[x][y]*verticalGradient.map[x][y])
                                         );
        }
    }
    
    //Deleting unneccessary matrices
    deleteMatrix(horizontalGradient);
    deleteMatrix(verticalGradient);
    
    // Scaling image matrix to get real world threshold values using Gamme Scaling.
    magnitude = scaleImage(magnitude, magnScale);
    
    //Final output image
    Image output_image = createImage(img.height, img.width);

    
    //Thresholding the image and binarizing the edges points.
    for(int x=0; x<img.height; x++){
        for(int y=0; y<img.width; y++){
            //Edge Point
            if(magnitude.map[x][y] >= threshold){
                output_image.map[x][y].i = 255;
            }
            //Not an Edge Point
            else {
                output_image.map[x][y].i = 0;
            }
        }
    }
    
    //Final image with Sobel edges.
    return output_image;
}

// This function takes an image and finds the edges
// using Canny filters and returns an image with the edges.
Image canny(Image img, double magnScale, double lowThreshold, double highThreshold) {
    
    //Converting smoothed image to image matrix
    Matrix imageMatrix = image2Matrix(img);
    
    //Horizontal gradient filter
    double dh[2][2] = {
        {0.5, -0.5},
        {0.5, -0.5}
    };
    
    //Vertical gradient filter
    double dv[2][2] = {
        {0.5, 0.5},
        {-0.5, -0.5}
    };
    
    //Creating matrices from filter values
    Matrix h = createMatrix(2, 2);
    Matrix v = createMatrix(2, 2);
    for(int x=0; x<2; x++){
        for(int y=0; y<2; y++){
            h.map[x][y] = dh[x][y];
            v.map[x][y] = dv[x][y];
        }
    }
    
    //Calculating the horizontal and vertical gradients for each pixel
    Matrix Q = convolve(imageMatrix, v);
    Matrix P = convolve(imageMatrix, h);

    //Deleting unneccessary matrices
    deleteMatrix(h);
    deleteMatrix(v);
    deleteMatrix(imageMatrix);
    
    // Magnitude matrix
    Matrix magnitude = createMatrix(img.height, img.width);
    for(int x=1; x<magnitude.height-1; x++){
        for(int y=1; y<magnitude.width-1; y++){
            magnitude.map[x][y] = sqrt ((P.map[x][y]*P.map[x][y]) + (Q.map[x][y]*Q.map[x][y]));
        }
    }
    
    // This Matrix stores the edge points after applying Non Maxima Suppression.
    Matrix nonMax = createMatrix(magnitude.height, magnitude.width);
    for(int x=0; x<magnitude.height; x++){
        for(int y=0; y<magnitude.width; y++){
            nonMax.map[x][y] = magnitude.map[x][y];
        }
    }
    
    //To convert angle from radians to degrees
    double val = 180.0/M_PI;
    
    // Non Maxima Suppression
    for(int x=1; x<magnitude.height-1; x++){
        for(int y=1; y<magnitude.width-1; y++){
            double arctan = atan2(Q.map[x][y], P.map[x][y]) * val;
            arctan = (arctan<0.0) ? arctan+180 : arctan;
            
            //Horizontal Edge
            if ( (0 <= arctan && arctan < 22.5) || (157.5 <= arctan && arctan <= 180) ) {
                if ((magnitude.map[x][y] < magnitude.map[x][y+1]) || (magnitude.map[x][y] < magnitude.map[x][y-1]))
                    nonMax.map[x][y] = 0;
            }
            //Diagonal Edge
            else if (22.5 <= arctan && arctan < 67.5) {
                if ((magnitude.map[x][y] < magnitude.map[x+1][y+1]) || (magnitude.map[x][y] < magnitude.map[x-1][y-1]))
                    nonMax.map[x][y] = 0;
            }
            //Vertical Edge
            else if (67.5 <= arctan && arctan < 112.5) {
                if ((magnitude.map[x][y] < magnitude.map[x+1][y]) || (magnitude.map[x][y] < magnitude.map[x-1][y]))
                    nonMax.map[x][y] = 0;
            }
            //Anti-Diagonal Edge
            else {
                if ((magnitude.map[x][y] < magnitude.map[x+1][y-1]) || (magnitude.map[x][y] < magnitude.map[x-1][y+1]))
                    nonMax.map[x][y] = 0;
            }
        }
    }
    
    //Deleting unneccessary matrices.
    deleteMatrix(P);
    deleteMatrix(Q);
    
    // Scaling image matrix to get real world threshold values using Gamme Scaling.
    nonMax = scaleImage(nonMax, magnScale);

    
    // Hysterisis Thresholding step 1.
    for(int x=0; x<nonMax.height; x++){
        for(int y=0; y<nonMax.width; y++){
            //Edge point
            if(nonMax.map[x][y] > highThreshold) {
                nonMax.map[x][y] = 255.0;
            }
            //Not an edge point
            else if(nonMax.map[x][y] < lowThreshold) {
                nonMax.map[x][y] = 0.0;
            }
            //Candidate edge point
            else {
                nonMax.map[x][y] = -1.0;
            }
        }
    }
    
    // X and Y vectors corresponding to 8-Neighbors required
    // for 2nd step of Hysterisis thresholding
    int xVector[8] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int yVector[8] = {-1, 0, 1, -1, 1, -1, 0, 1};

    //Final output image
    Image output_image = createImage(img.height, img.width);
    
    // Hysterisis Thresholding step 2.
    for(int x=0; x<img.height; x++) {
        for(int y=0; y<img.width; y++) {
            
            //Candidate Edge
            if(nonMax.map[x][y] == -1.0) {
                
                //Checking all the 8 neighbors
                for(int i=0; i<8; i++){
                    int neighborX = x + xVector[i];
                    int neighborY = y + yVector[i];
                    
                    if(neighborX>=0 && neighborX<nonMax.height && neighborY>=0 && neighborY<nonMax.width) {
                        //Neighbor is an edge
                        if(nonMax.map[neighborX][neighborY] == 255.0) {
                            output_image.map[x][y].i = 255;
                            break;
                        }
                        //Neighbor is not an edge
                        else {
                            output_image.map[x][y].i = 0;
                        }
                    }
                }
            }
            //Copy the value if not a candidate
            else {
                output_image.map[x][y].i = (int) nonMax.map[x][y];
            }
        }
    }
    
    //Final image with Canny edges.
    return output_image;
}
