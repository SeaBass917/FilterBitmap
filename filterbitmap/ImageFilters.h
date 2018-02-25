// A01252370
// CRN24121
// prototypes 7 image filter functions 

#ifndef IMAGEFILTERS_H_USED
#define IMAGEFILTERS_H_USED

#include "Image.h"

// Converts to negative (note that negating a negative image restores)
bool ConvertImageToNegative(Image *pImage);

// Converts to grayscale, returns Grayskull on success
bool ConvertImageToGrayscale(Image *pImage);

// Converts image to sepia
bool ConvertImageToSepia(Image *pImage);

// Threshhold double [0, 255]
bool ConvertImageToBinary(Image *pImage, double threshold);

// Threshhold double [0, 255] 
bool ConvertImageToSolarized(Image *pImage, double threshold);

// Creates blurred image
Image *CreateBoxBlurImage(const Image *pImage, unsigned radius);

// gauss's trademarked instagram filter
Image *CreateGaussianBlurImage(const Image *pImage, unsigned radius, double sigma);


#endif