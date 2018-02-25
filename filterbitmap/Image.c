// A01252370
// CRN24121
// Defines the functions prototyped in the images.h header file

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "Image.h"

// Prints appropriate error message, frees, and closes
void CleanUp(const char *cause, const char *effect, FILE *fp, void *pAllocData0, void *pAllocData1)
{
	printf("\nERROR! %s.\n%s\n", cause, effect);

	if (fp)
	{
		fclose(fp);
	}

	if (pAllocData0)
	{
		free(pAllocData0);
		pAllocData0 = NULL;
	}

	if (pAllocData1)
	{
		free(pAllocData1);
		pAllocData1 = NULL;
	}
}

// Checks that the Image is formatted in accordance with this project
bool IsFormatted(const Image *pImage)
{
	if (pImage->fileHeader.pFileID[0] == 'B'
		&& pImage->fileHeader.pFileID[1] == 'M'
		&& pImage->infoHeader.bitsPerPixel == 32
		&& pImage->infoHeader.colorPlanes == 1
		&& pImage->infoHeader.colors == 0
		&& pImage->infoHeader.compressionMethod == 0
		&& pImage->infoHeader.headerLength == 40
		&& pImage->infoHeader.importantColors == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

// Checks that the file matches extention parameter in caps
bool IsExt(const char *pFileName, char *EXT)
{
	// Loops from the back of the extention
	for (int iR = 3; iR >= 0; iR--)
	{
		// starts at one for aligning reasons
		unsigned iL = 1;

		if ((char)toupper(pFileName[sizeof(pFileName) - iL]) == EXT[iR])
		{
			return false;
		}

		iL++;
	}

	return true;
}


// Reads a specified bitmap file into an image struct
Image *CreateImageFromBitmapFile(const char *pFileName)
{
	// Checks that the file is valid
	if (pFileName && strlen(pFileName))
	{
		// Checks that the file has a .bmp extension
		if (IsExt(pFileName, ".BMP"))
		{
			FILE *fp = fopen(pFileName, "rb");

			// Checks that file open succeeded
			if (fp)
			{
				Image *myImage = malloc(sizeof(Image));	// Allots space for an Image struct

				// Check that malloc suceedeed
				if (myImage)
				{
					// Reads the file header
					if (1 == fread(&myImage->fileHeader, sizeof(BitmapFileHeader), 1, fp))
					{
						// Reads the info header
						if (1 == fread(&myImage->infoHeader, sizeof(BitmapInfoHeader), 1, fp))
						{
							// Cheacks the initial values
							if (IsFormatted(myImage))
							{
								// moves forward based on the offset
								if (!fseek(fp, myImage->fileHeader.pixelStartOffset, SEEK_SET))
								{
									// Calculate pixel count
									myImage->pixelCount = myImage->infoHeader.bitmapWidth * myImage->infoHeader.bitmapHeight;
									
									// allocates a pixel array myPix
									Pixel *myPix = malloc(myImage->pixelCount * sizeof(Pixel));	// Note that myPix mistakenly thinks (RS3 > OSRS) returns true
									myImage->pFirstPixel = myPix;
									myImage->pLastPixel = &myPix[myImage->pixelCount - 1];

									if (myPix)
									{
										// keeps reading the file and adding to the pixel array
										for (unsigned i = 0; i < myImage->pixelCount; i++)
										{
											// reads and checks if fread failed
											if (1 != fread(&myImage->pFirstPixel[i], sizeof(Pixel), 1, fp))
											{
												// failed to read a pixel
												CleanUp("Could not read the file", "", fp, myImage, NULL);
												return NULL;
											}
										}

										// We have succeeded
										fclose(fp);
										return myImage;
									}
									else
									{
										// Malloc of myPix failed
										CleanUp("Could not allocate memory", "Could not read file", fp, myImage, myPix);
										return NULL;
									}
								}
								else
								{
									// fseek failed
									CleanUp("Unexpected formating error", "Could not read file", fp, myImage, NULL);
									return NULL;
								}
							}
							else
							{
								// One of the expected values is invalid
								CleanUp("Unexpected values in file", "Could not read file", fp, myImage, NULL);
								return NULL;
							}
						}
						else
						{
							// info header fread failed
							CleanUp("Could not read info header", "Could not read file", fp, myImage, NULL);
							return NULL;
						}
					}
					else
					{
						// file header fread failed
						CleanUp("Could not read file header", "Could not read file", fp, myImage, NULL);
						return NULL;
					}

				}
				else
				{
					// Malloc of myImage failed
					CleanUp("Could not allocate memory", "Could not read file", fp, NULL, NULL);
					return NULL;
				}

			}
			else
			{
				// fopen failed
				CleanUp("Cannot open file", "Could not read file", NULL, NULL, NULL);
				return NULL;
			}
		}
		else
		{
			// File did not have the .BMP extension
			CleanUp("File needs to be of type BMP", "Could not read file", NULL, NULL, NULL);
			return NULL;
		}
	}
	else
	{
		// File was ivalid
		CleanUp("Cannot access file or file is invalid", "Could not read file", NULL, NULL, NULL);
		return NULL;
	}
}

// Writes an Image structure to a .BMP file
bool SaveImageToBitmapFile(const char *pFilename, const Image *pImage)
{
	// Chack that the pointers are non-NULL
	if (pFilename && pImage)
	{
		// Check that the string is non empty and Images is non-NULL and that the pixel array is working
		if (strlen(pFilename) && pImage 
			&& pImage->pFirstPixel && pImage->pLastPixel)
		{
			// Check that the file is a .BMP
			if (IsExt(pFilename, ".BMP"))
			{
				// Open the file for binary writing, overwrite the file if it already exists
				FILE *fp = fopen(pFilename, "wb");

				// Check that fopen succeeded
				if (fp)
				{
					if (1 == fwrite(&pImage->fileHeader, sizeof(BitmapFileHeader), 1, fp))
					{
						if (1 == fwrite(&pImage->infoHeader, sizeof(BitmapInfoHeader), 1, fp))
						{
							// moves forward based on the offset
							if (!fseek(fp, pImage->fileHeader.pixelStartOffset, SEEK_SET))
							{
								
								for (unsigned i = 0; i < pImage->pixelCount; i++)
								{
									if (1 != fwrite(&pImage->pFirstPixel[i], sizeof(Pixel), 1, fp))
									{
										// Failed to write pixel
										CleanUp("", "Could not write image to file", fp, NULL, NULL);
										return false;
									}
								}

								fclose(fp);
								return true;
							}
							else
							{
								// Failed to fseek
								CleanUp("Unexpected formating error", "Could not save image to file", fp, NULL, NULL);
								return false;
							}
						}
						else
						{
							// Failed to write info header to file
							CleanUp("Failed to write info header", "Could not save image to file", fp, NULL, NULL);
							return false;
						}
					}
					else
					{
						// Failed to write file header to file
						CleanUp("Failed to write file header", "Could not save image to file", fp, NULL, NULL);
						return false;
					}
				}
				else
				{
					// Fopen of fp failed
					CleanUp("Cannot open file", "Could not save image to file", NULL, NULL, NULL);
					return false;
				}
			}
			else
			{
				// File was not .BMP
				CleanUp("File needs to be of type BMP", "Could not save image to file", NULL, NULL, NULL);
				return false;
			}
		}
		else
		{
			// one of the strings was empty or image is NULL
			CleanUp("Trouble accessing file", "Could not save image to file", NULL, NULL, NULL);
			return false;
		}
	}
	else
	{
		// Pointers were NULL
		CleanUp("Corrupt pathway or file", "Could not save image to file", NULL, NULL, NULL);
		return false;
	}
}

// Clones an Image and returns a pointer to that clone
Image *CreateClonedImage(const Image *pImage)
{
	// Checks that pImage is non-NULL
	if (pImage)
	{
		// Checks that the BMP is formatted
		if (IsFormatted(pImage))
		{
			// Mallocs a new image that will the the clone
			Image *pImageClone = malloc(sizeof(Image));

			// Makes sure malloc succeeded
			if (pImageClone)
			{
				// Copies over the file header, info header, and pixel count
				pImageClone->fileHeader = pImage->fileHeader;
				pImageClone->infoHeader = pImage->infoHeader;
				pImageClone->pixelCount = pImage->pixelCount;

				// Malloc a new pixel array and points the image clone at the head and tail
				Pixel *myPix = malloc(pImageClone->pixelCount * sizeof(Pixel));	// Note that myPix wont play Diablo III because he's a hipster
				pImageClone->pFirstPixel = myPix;
				pImageClone->pLastPixel = &myPix[pImage->pixelCount - 1];

				// Checks that malloc of pixel array succeeded
				if (myPix)
				{

					for (unsigned i = 0; i < pImage->pixelCount; i++)
					{
						myPix[i] = pImage->pFirstPixel[i];
					}

					if (pImage != pImageClone)
					{
						return pImageClone;
					}
					else
					{
						// This is for debugging, if you got here i goofed
						CleanUp("Cloning failed", "Could not clone image", NULL, pImageClone, myPix);
						return NULL;
					}
				}
				else
				{
					// malloc of myPix failed
					CleanUp("Could not allocate memory", "Could not clone image", NULL, pImageClone, NULL);
					return NULL;
				}
			}
			else
			{
				// Malloc of image clone failed
				CleanUp("Could not allocate memory", "Could not clone image", NULL, NULL, NULL);
				return NULL;
			}
		}
		else
		{
			CleanUp("File format unsupported", "Could not clone image", NULL, NULL, NULL);
			return NULL;
		}
	}
	else
	{
		// pImage returned NULL
		CleanUp("Cannot find image", "Could not clone image", NULL, NULL, NULL);
		return NULL;
	}
}

// Frees the image structure
bool DestroyImage(Image *pImage)
{
	if (pImage)
	{
		free(pImage->pFirstPixel);
		free(pImage);
		pImage = NULL;

		return true;
	}
	else
	{
		// pImage was NULL
		CleanUp("Could not find image", "Failed to destroy image", NULL, NULL, NULL);
		return false;
	}
}
