//
//  noise_remover.c
//  ImageProcessingCV
//
//  Created by Harshit Trehan on 12/12/20.
//  Copyright © 2020 Harshit Trehan. All rights reserved.
//

#ifndef NOISE
#define NOISE
#include "noise_remover.h"
#endif

#ifndef NETPBM
#define NETPBM
#include "../netpbm.h"
#endif

#ifndef UTIL
#define UTIL
#include "../Utility/processing_utility.h"
#endif

void expand(Image finalImage, Image originalImage){
    int x, y;
    
    //These vectors are used to check the 4-Neighbors.
    int xVector[] = {0, 0, -1, 1};
    int yVector[] = {1, -1, 0, 0};
    
    int imageHeight = originalImage.height;
    int imageWidth = originalImage.width;
    
    for(x=0; x<imageHeight; x++){
        for(y=0; y<imageWidth; y++){
            //If value is WHITE (255), then we need to check neighbors.
            if(originalImage.map[x][y].i==255){
                int i=0;
                for(i=0; i<4; i++){
                    int newX = x + xVector[i];
                    int newY = y + yVector[i];
                    
                    //If neighbor co-ordinates are out of range, skip.
                    if(newX<0 || newY<0 || newX >= imageHeight || newY >= imageWidth){
                        continue;
                    }
                    
                    //If co-ordinates are in range and neighbor is BLACK (0), then we flip.
                    else if(originalImage.map[newX][newY].i == 0){
                        finalImage.map[x][y].i = 0;
                        break;
                    }
                }
            }
        }
    }
}

void shrink(Image finalImage, Image originalImage){
    int x, y;
    
    //These vectors are used to check the 4-Neighbors.
    int xVector[] = {0, 0, -1, 1};
    int yVector[] = {1, -1, 0, 0};
    
    int imageHeight = originalImage.height;
    int imageWidth = originalImage.width;
    
    for(x=0; x<imageHeight; x++){
        for(y=0; y<imageWidth; y++){
            //If value is BLACK (0), then we need to check neighbors.
            if(originalImage.map[x][y].i==0){
                int i=0;
                for(i=0; i<4; i++){
                    int newX = x + xVector[i];
                    int newY = y + yVector[i];
                    
                    //If neighbor co-ordinates are out of range, skip.
                    if(newX<0 || newY<0 || newX >= imageHeight || newY >= imageWidth){
                        continue;
                    }
                    
                    //If co-ordinates are in range and neighbor is WHITE (255), then we flip.
                    else if(originalImage.map[newX][newY].i == 255){
                        finalImage.map[x][y].i = 255;
                        break;
                    }
                }
            }
        }
    }
}

void noiseImage(Image img, float probability){
    int x;
    int y;
    
    for(x=0; x<img.height; x++){
        for(y=0; y<img.width; y++){
            
            //If the probability generated is more than the given probability, then we flip.
            float probabilityToFlip = randomProbability();
            
            if(probabilityToFlip > probability){
                (img.map[x][y].i == 255) ? (img.map[x][y].i = 0) : (img.map[x][y].i = 255);
            }
        }
    }
}
