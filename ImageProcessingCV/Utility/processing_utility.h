//
//  processing_utility.h
//  ImageProcessingCV
//
//  Created by Harshit Trehan on 12/13/20.
//  Copyright © 2020 Harshit Trehan. All rights reserved.
//

#ifndef NETPBM
#define NETPBM
#include "../netpbm.h"
#endif

// Returns a random floating point number between 0 and 1
float randomProbability(void);

// This functions concatenates strings s1 and s2 and returns the pointer to the final copied string.
char* concat(const char *s1, const char *s2);

// Converts given Image to black and white. If the background is darker than the image pass background=0
// and all pixels above <threshold> are turned black and rest are turned white. If background is lighter,
// pass background = 1 and all pixels below <threshold> are turned black and rest are turned white.
Image createBlackAndWhite(Image original, int threshold, int background);

// Creates a deep copy of <original> Image into <copy> Image.
void deepCopy(Image original, Image copy);

// Returns a Gaussian filter of size <kernelSize> with standard deviation <sigma>
Matrix getGaussian(double sigma, int kernelSize);

// Convolves 2 matrics <m1> and <m2>. <m1> is the larger matrix which is usually an Image.
// <m2> is usually a filter
Matrix convolve(Matrix m1, Matrix m2);

// Performs gamma transformation (scaling) on given image Matrix <m> for value of <gamma>.
Matrix scaleImage(Matrix m, double gamma);
