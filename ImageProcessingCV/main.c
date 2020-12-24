//
//  main.c
//  ImageProcessingCV
//
//  Created by Harshit Trehan on 12/12/20.
//  Copyright © 2020 Harshit Trehan. All rights reserved.
//

#ifndef NETPBM
#define NETPBM
#include "netpbm.h"
#endif

#ifndef UTIL
#define UTIL
#include "Utility/processing_utility.h"
#endif

#ifndef HOUGH
#define HOUGH
#include "HoughTransformation/hough_transformer.h"
#endif

#ifndef COMPONENT
#define COMPONENT
#include "ComponentLabelling/component_labeler.h"
#endif

#ifndef NOISE
#define NOISE
#include "NoiseRemoval/noise_remover.h"
#endif

#ifndef EDGE
#define EDGE
#include "EdgeDetection/edge_detector.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

static char *directory = "/Users/harshittrehan/Desktop/ImageProcessingCV/ImageProcessingCV/";

int main(int argc, const char * argv[]) {
    time_t t;
    srand((unsigned)time(&t));
    
    /*
     DO NOT FORGET TO DELETE THE IMAGES/MATRICES YOU CREATE HERE. MEMORY IS ALLOCATED IN THE HEAP
     SO TO AVOID MEMORY LEAKS MAKE SURE TO DELETE.
     */
    
    // ************** EDGE DETECTION **************
    // Use grayscale image for edge detection. You can covert to grayscale
    // simply by reading PPM image and re-saving it with '.pgm' extension. Example:
    Image colored = readImage(concat(directory, "lena.ppm"));
    writeImage(colored, concat(directory, "lena.pgm"));
    deleteImage(colored);
    
    char *edgeFile = "engine.pgm";
    edgeFile = concat(directory, edgeFile);
    Image edgesImage = readImage(edgeFile);
    Image sobelEdges = sobel(edgesImage, 0.35, 130);
    writeImage(sobelEdges, concat(directory, "sobel_edges.pbm"));
    Image cannyEdges = canny(edgesImage, 0.35, 55, 120);
    writeImage(cannyEdges, concat(directory, "canny_edges.pbm"));
    deleteImage(edgesImage);
    deleteImage(sobelEdges);
    deleteImage(cannyEdges);
    
    // ************** COMPONENT LABELLING **************
    // Use black and white image for component labelling. If background is
    // darker than objects, pass 0 in <background> otherwise pass 1.
    char *componentFile = "coins.pgm";
    componentFile = concat(directory, componentFile);
    Image compsImage = readImage(componentFile);
    Image blackWhiteImage = createImage(compsImage.height, compsImage.width);
    blackWhiteImage = createBlackAndWhite(compsImage, 100, 0, blackWhiteImage);
    writeImage(blackWhiteImage, concat(directory, "black_and_white.pbm"));
    blackWhiteImage = connectedComponents(blackWhiteImage, 100);
    writeImage(blackWhiteImage, concat(directory, "labelled_components.ppm"));
    deleteImage(compsImage);
    deleteImage(blackWhiteImage);
    
    // ************** NOISE REMOVAL **************
    // Use black and white image for noise removal.
    char *noiseFile = "text.pbm";
    noiseFile = concat(directory, noiseFile);
    Image noiseImage = readImage(noiseFile);
    Image cleanImage = createImage(noiseImage.height, noiseImage.width);
    deepCopy(noiseImage, cleanImage);
    Image noiseImageDeepCopy = createImage(noiseImage.height, noiseImage.width);
    deepCopy(noiseImage, noiseImageDeepCopy);
    expand(noiseImageDeepCopy, cleanImage);
    deepCopy(cleanImage, noiseImageDeepCopy);
    shrink(noiseImageDeepCopy, cleanImage);
    deepCopy(cleanImage, noiseImageDeepCopy);
    shrink(noiseImageDeepCopy, cleanImage);
    deepCopy(cleanImage, noiseImageDeepCopy);
    shrink(noiseImageDeepCopy, cleanImage);
    deepCopy(cleanImage, noiseImageDeepCopy);
    expand(noiseImageDeepCopy, cleanImage);
    deepCopy(cleanImage, noiseImageDeepCopy);
    expand(noiseImageDeepCopy, cleanImage);
    writeImage(cleanImage, concat(directory, "clean_image.pbm"));
    
    noiseImage = createNoisyImage(noiseImage, 0.1);
    writeImage(noiseImage, concat(directory, "noise_image.pbm"));
    deleteImage(noiseImage);
    deleteImage(cleanImage);
    deleteImage(noiseImageDeepCopy);
    
    // ************** HOUGH TRANSFORM **************
    // Use an edge image for Hough transforms
    char *houghFile = "coins.pgm";
    // Visual inspection of image tells 10 circle so number = 10
    int number = 10, minDist = 35;
    houghFile = concat(directory, houghFile);
    Image houghImage = readImage(houghFile);
    Matrix houghMatrix = image2Matrix(houghImage);
    // Gaussian 5x5 kernel for smoothing
    Matrix gaussian5x5 = getGaussian(1.0, 5);
    houghMatrix = convolve(houghMatrix, gaussian5x5);
    Image smoothedHoughImage = matrix2Image(houghMatrix, 0, 1.0);
    Image houghEdges = canny(smoothedHoughImage, 0.5, 85, 150);
    houghMatrix = image2Matrix(houghEdges);
    Matrix houghParameterMatrix = createMatrix(houghImage.height, houghImage.width);
    int radius = (int)(MIN(houghEdges.height/2, houghEdges.width/2));
    OutputParams_3D houghArray = houghTransformCircle(houghMatrix, radius, houghParameterMatrix);
    Image houghParameterImage = matrix2Image(houghParameterMatrix, 0, 1.0);
    writeImage(houghParameterImage, concat(directory, "hough_parameter_space_image.pgm"));
    Matrix maxMatrix = findHoughMaxima(houghArray, number, minDist);
    for (int i = 0; i < number; i++){
        unsigned char r = rand() % 256;
        unsigned char g = rand() % 256;
        unsigned char b = rand() % 256;
       ellipse(houghImage, maxMatrix.map[0][i], maxMatrix.map[1][i],maxMatrix.map[2][i],maxMatrix.map[2][i], 1, 0, 0, r, g, b, 0);
    }
    writeImage(houghImage, concat(directory, "hough_circls.ppm"));
    deleteImage(houghImage);
    deleteImage(smoothedHoughImage);
    deleteImage(houghEdges);
    deleteImage(houghParameterImage);
    deleteMatrix(houghMatrix);
    deleteMatrix(gaussian5x5);
    deleteMatrix(houghParameterMatrix);
    deleteMatrix(maxMatrix);
    delete_3DMatrix(houghArray);
    
}
