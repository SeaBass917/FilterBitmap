// A01252370
// CRN24121
// Defines the structures used to define a bitmap file

#ifndef BITMAPFILEFORMAT_H_USED
#define BITMAPFILEFORMAT_H_USED

#include <stdint.h>

// Defines the file header of a bitmap file
#pragma pack(push, 1)	// pragma removes the padding that would otherwise break the header
typedef struct BitmapFileHeader
{
	char pFileID[2];	// [must be array of 2 chars B and M]
	uint32_t byteCount;	// Number of bytes in the file
	int16_t reserved0;	// ignore
	int16_t reserved1;	// ignore
	uint32_t pixelStartOffset;	// offset in bytes from beggining of file to start of pixel data
} BitmapFileHeader;
#pragma pack(pop)	// Undoes the pragma call before the struct definition

// Defines the info header of a bitmap file
typedef struct BitmapInfoHeader
{
	uint32_t headerLength; // [40] Length of this header in bytes 
	int32_t bitmapWidth;	// The width of the bitmap in pixels
	int32_t bitmapHeight;	// The height of the bitmap in pixels
	uint16_t colorPlanes;	// [1]
	uint16_t bitsPerPixel;	// [32]
	uint32_t compressionMethod; // [0] means no compression
	uint32_t imageSize;	// Size of image data area in bytes
	int32_t horizontalResolution;	// Number of pixels per meter along the X-axis
	int32_t verticalResolution;		// Number of pixels per meter along the Y-axis
	uint32_t colors;	// [0] means all 2^n colors used
	uint32_t importantColors;	// [0] Is just a political statement. All colors are important
} BitmapInfoHeader;

// Defines an RGB pixel
typedef struct Pixel
{
	uint8_t blueChannel;	// B
	uint8_t greenChannel;	// G
	uint8_t redChannel;		// R
	uint8_t alphaChannel;	// ignored in this project
} Pixel;

#endif