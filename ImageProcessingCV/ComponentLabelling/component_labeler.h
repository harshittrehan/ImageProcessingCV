//
//  component_labeler.h
//  ImageProcessingCV
//
//  Created by Harshit Trehan on 12/12/20.
//  Copyright © 2020 Harshit Trehan. All rights reserved.
//

#ifndef NETPBM
#define NETPBM
#endif
#include <stdio.h>

// To find connected components in a black and white image with atleast <threshold> number of
// pixels in a component. Also, saves an image with different components in different colors.
void connectedComponents(Image img, int threshold);

// Helper function which is recursively called by connectedComponents() and impleemnts an
// efficient component labelling algorithm. <componentMatrix> is the resulting labelled
// component Matrix. <img> is the original image. <x> and <y> are current pixel coordinates.
// <componentNum> is the current number of components found.
void efficientComponentLabeling(Matrix componentMatrix, Image img, int x, int y, int componentNum);
