//
//  noise_remover.h
//  ImageProcessingCV
//
//  Created by Harshit Trehan on 12/12/20.
//  Copyright © 2020 Harshit Trehan. All rights reserved.
//

#ifndef NETPBM
#define NETPBM
#include "../netpbm.h"
#endif

// Function to perform expand operation for noise removal on given <originalImage> and return the the <finalImage>.
Image expand(Image originalImage, Image finalImage);

// Function to perform shrink operation for noise removal on given <originalImage> and return the the <finalImage>.
Image shrink(Image originalImage, Image finalImage);

// Function to generate salt and pepper noise in black and white image. A random number between 0 and 1
// is generated for each pixel and if its value is bigger than <probability> then the value of that
// pixel is flipped.
Image createNoisyImage(Image img, float probability);
