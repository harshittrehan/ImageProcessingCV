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
    
    char *inputFilename = "coins.pgm";
    inputFilename = concat(directory, inputFilename);
    printf("%s%s", inputFilename, "\n");
    
}
