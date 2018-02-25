// A01252370
// CRN24121
// Uses the bitmap file format structs to define an bitmap image and prototypes 4 basic image functions

#ifndef IMAGE_H_USED
#define IMAGE_H_USED

#include <stdbool.h>
#include "BitmapFileFormat.h"

// Defines a bitmap image
typedef struct Image
{
	BitmapFileHeader fileHeader; // Storage for the file header
	BitmapInfoHeader infoHeader; // Storage for the info header
	unsigned pixelCount; // Total number of pixels in the image
	Pixel *pFirstPixel; // Points to start of the pixel array
	Pixel *pLastPixel; // Points to end of the pixel array
} Image;

// Reads a specified bitmap file into an image struct
Image *CreateImageFromBitmapFile(const char *pFileName);

// Writes an Image structure to a .BMP file
bool SaveImageToBitmapFile(const char *pFilename, const Image *pImage);

// Clones an Image and returns a pointer to that clone
Image *CreateClonedImage(const Image *pImage);

// Frees the image structure
bool DestroyImage(Image *pImage);

#endif