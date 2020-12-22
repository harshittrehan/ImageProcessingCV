//
//  component_labeler.c
//  ImageProcessingCV
//
//  Created by Harshit Trehan on 12/12/20.
//  Copyright © 2020 Harshit Trehan. All rights reserved.
//

#ifndef NETPBM
#define NETPBM
#include "../netpbm.h"
#endif

#ifndef COMPONENT
#define COMPONENT
#include "component_labeler.h"
#endif

#include <stdlib.h>
#include <stdio.h>

void efficientComponentLabeling(Matrix componentMatrix, Image img, int x, int y, int componentNum) {
    if(img.map[x][y].i != 0 || componentMatrix.map[x][y] != 0.0){
        return;
    }
    
    componentMatrix.map[x][y] = (double) componentNum;
    
    int xVector[] = {0, 0, -1, 1};
    int yVector[] = {1, -1, 0, 0};
    
    int i=0;
    for(i=0; i<4; i++){
        int newX = x + xVector[i];
        int newY = y + yVector[i];
        
        //If neighbor co-ordinates are out of range, skip.
        if(newX<0 || newY<0 || newX >= img.height || newY >= img.width){
            continue;
        }
        else {
            efficientComponentLabeling(componentMatrix, img, newX, newY, componentNum);
        }
    }
}

void connectedComponents(Image img, int threshold){
    Matrix componentMatrix = createMatrix(img.height, img.width);
    int x;
    int y;
    
    int componentNum = 0;
    
    for(x=0; x<img.height; x++){
        for(y=0; y<img.width; y++){
            
            if(img.map[x][y].i == 0 && componentMatrix.map[x][y] == 0.0){
                efficientComponentLabeling(componentMatrix, img, x, y, ++componentNum);
            }
        }
    }
    
    int *componentPixelCount = (int *) calloc(componentNum+1, sizeof(int) );
    
    for(x=0; x<componentMatrix.height; x++){
        for(y=0; y<componentMatrix.width; y++){
            if(componentMatrix.map[x][y] != 0.0) {
                componentPixelCount[(int)componentMatrix.map[x][y]]++;
            }
        }
    }
    
    int count=0;
    
    for(int i=0; i<componentNum+1; i++){
        if(componentPixelCount[i] >= threshold){
            count++;
        }
    }
    
    printf("%s%d", "Total number of componenets in the image: ", componentNum);
    
    Pixel *pixelValues = (Pixel *) malloc(sizeof(Pixel)*componentNum+1);
    
    for(int i=0; i<componentNum+1; i++){
        pixelValues[i].r = rand() % 256;
        pixelValues[i].g = rand() % 256;
        pixelValues[i].b = rand() % 256;
    }
    
    for(x=0; x<componentMatrix.height; x++){
        for(y=0; y<componentMatrix.width; y++){
            if(componentMatrix.map[x][y] != 0.0 && componentPixelCount[(int)componentMatrix.map[x][y]] >= threshold){
                img.map[x][y].r = pixelValues[(int)componentMatrix.map[x][y]].r;
                img.map[x][y].g = pixelValues[(int)componentMatrix.map[x][y]].g;
                img.map[x][y].b = pixelValues[(int)componentMatrix.map[x][y]].b;
            }
            else if(componentMatrix.map[x][y] != 0.0){
                img.map[x][y].r = 0;
                img.map[x][y].g = 0;
                img.map[x][y].b = 0;
            }
        }
    }
    
    writeImage(img, "/Users/harshittrehan/Desktop/ImageProcessingCV/ImageProcessingCV/components_colored.ppm");
}
