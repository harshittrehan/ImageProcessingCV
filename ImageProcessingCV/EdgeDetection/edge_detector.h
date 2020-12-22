//
//  edge_detector.h
//  ImageProcessingCV
//
//  Created by Harshit Trehan on 12/12/20.
//  Copyright © 2020 Harshit Trehan. All rights reserved.
//

#ifndef NETPBM
#define NETPBM
#include "../netpbm.h"
#endif

// Detects edges in image using Sobel edge detector. All pixels in magnitude matrix above <threshold> are binarized as white
// edges, those below are made black. <magnScale> is the value of gamma for gamma transformation of the image after magnitude
// is calculated. This is necessray to get real-world values of the <threshold> otherwise <threshold> values can be very small/large.
Image sobel(Image img, double magnScale, double threshold);

// Detects edges in image using Canny edge detector. All pixels in magnitude matrix above <highThreshold> are binarized as white
// edges, those below <lowThreshold> are made black and those between <lowThreshold> and <highThreshold> are marked to perform rest
// of Hysterisis Thresholding. <magnScale> is the value of gamma for gamma transformation of the image after magnitude
// is calculated. This is necessray to get real-world values of the <threshold> otherwise <threshold> values can be very small/large.
Image canny(Image img, double magnScale, double lowThreshold, double highThreshold);
