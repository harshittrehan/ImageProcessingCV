//
//  hough_transformer.h
//  ImageProcessingCV
//
//  Created by Harshit Trehan on 12/12/20.
//  Copyright © 2020 Harshit Trehan. All rights reserved.
//

#ifndef NETPBM
#define NETPBM
#include "../netpbm.h"
#endif

//3D parameter space for Hough Circle transformation
typedef struct {
    int height, width, radius;
    double ***map;
} OutputParams_3D;

OutputParams_3D create3DMatrix(int height, int width, int radius);

void delete_3DMatrix(OutputParams_3D matrix);

OutputParams_3D houghTransformCircle(Matrix mxSpatial, int radius, Matrix outputImg);

int isLocalMaximum(OutputParams_3D mx, int m, int n, int r);

void insertMaxEntry(Matrix  mx, int vPos, int hPos, int radius, double strength, int index);

void deleteMaxEntry(Matrix mx, int i);

Matrix findHoughMaxima(OutputParams_3D mx, int number, double minSeparation)
