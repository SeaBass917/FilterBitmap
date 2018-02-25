// A01252370
// CRN24121
// Takes user input from command line args and filters images

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ImageFilters.h"

#define NEGATIVE_FILTER_IMPLEMENTED
#define SEPIA_FILTER_IMPLEMENTED
#define GRAYSCALE_FILTER_IMPLEMENTED
#define BINARY_FILTER_IMPLEMENTED
#define SOLARIZED_FILTER_IMPLEMENTED
#define BOXBLUR_FILTER_IMPLEMENTED
#define GAUSSIANBLUR_FILTER_IMPLEMENTED

//#define DEBUG

/*
TestPart1

COPY THIS SOURCE CODE into your main.c file before the main function.

This function verifies that the three C structures defined in Part 1 are the correct
size, to exactly match the requirements of a BMP image file. If any of the structures
are found to have incorrect sizes, the program terminates with an error message. If
all sizes are correct, the function prints a "passed" message.
*/

void HelperBinaryErrorFinder(char *buffer1, char *buffer2, unsigned byteCount, unsigned unruleyIndex)
{
	if (byteCount == 1)
	{
		// Reached an unruley clone index
		printf("Unruley value located at index %u\n", unruleyIndex);
	}

	//first half has an error
	if (memcmp(buffer1, buffer2, byteCount / 2) != 0)
	{
		HelperBinaryErrorFinder(buffer1, buffer2, byteCount / 2, unruleyIndex);
	}
	//second half has an error
	if (memcmp(&buffer1[byteCount / 2], &buffer2[byteCount / 2], byteCount / 2) != 0)
	{
		unruleyIndex += (byteCount / 2);
		HelperBinaryErrorFinder(&buffer1[byteCount / 2], &buffer2[byteCount / 2], byteCount / 2, unruleyIndex);
	}
}

void BinaryErrorFinder(char *buffer1, char *buffer2, unsigned byteCount)
{
	unsigned unruleyIndex = 0;
	HelperBinaryErrorFinder(buffer1, buffer2, byteCount, unruleyIndex);
}



static void TestPart1(void)
{
	const unsigned expectedFileHeaderLength = 14;	// exact byte count of bitmap file header
	const unsigned expectedInfoHeaderLength = 40;	// exact byte count of bitmap info header
	const unsigned expectedPixelLength = 4;			// exact byte count of pixel structure
	bool testCaseFailed = false;

	printf("\n\nStarting TestPart1...\n\n");

	if (sizeof(BitmapFileHeader) != expectedFileHeaderLength)
	{
		printf("\tERROR: BitmapFileHeader is not the correct size of %u bytes.\n",
			expectedFileHeaderLength);
		testCaseFailed = true;
	}

	if (sizeof(BitmapInfoHeader) != expectedInfoHeaderLength)
	{
		printf("\tERROR: BitmapInfoHeader is not the correct size of %u bytes.\n",
			expectedInfoHeaderLength);
		testCaseFailed = true;
	}

	if (sizeof(Pixel) != expectedPixelLength)
	{
		printf("\tERROR: Pixel is not the correct size of %u bytes.\n",
			expectedPixelLength);
		testCaseFailed = true;
	}

	if (testCaseFailed)
	{
		printf("\tFAILED: One or more test cases failed.\n");
		exit(EXIT_FAILURE);
	}

	printf("\tSUCCESS: Test cases PASSED.\n\n");
}

/*
TestPart2

COPY THIS SOURCE CODE into your main.c file before the main function.

This function tests the functionality of CreateImageFromBitmapFile, SaveImageToBitmapFile,
CreateClonedImage, and DestroyImage, testing valid and invalid code paths.
*/
static void TestPart2(const char *pInputFileName, const char *pOutputPath)
{
	const char *pNonBmpFile = "temp.txt";
	//char outputBuffer[256];
	Image *pImage;
	Image *pClone;
	bool result;
	bool testCaseFailed = false;

	printf("\n\nStarting TestPart2...\n\n");

	printf("\tPassing NULL to CreateImageFromBitmapFile...\n\t");

	pImage = CreateImageFromBitmapFile(NULL);

	if (pImage)
	{
		printf("\n\tFAILED.\n");
		testCaseFailed = true;
	}
	else
	{
		printf("\n\tPASSED.\n\n");
	}

	printf("\tPassing empty string to CreateImageFromBitmapFile...\n\t");

	pImage = CreateImageFromBitmapFile("");

	if (pImage)
	{
		printf("\n\tFAILED.\n");
		testCaseFailed = true;
	}
	else
	{
		printf("\n\tPASSED.\n\n");
	}

	printf("\tPassing non-BMP file name to CreateImageFromBitmapFile...\n\t");

	pImage = CreateImageFromBitmapFile(pNonBmpFile);

	if (pImage)
	{
		printf("\n\tFAILED.\n");
		testCaseFailed = true;
	}
	else
	{
		printf("\n\tPASSED.\n\n");
	}

	printf("\tPassing good file name to CreateImageFromBitmapFile...\n\t");

	pImage = CreateImageFromBitmapFile(pInputFileName);

	if (!pImage ||
		!pImage->pFirstPixel ||
		!pImage->pixelCount)
	{
		printf("\n\tFAILED.\n");
		testCaseFailed = true;
	}
	else
	{
		printf("\n\tPASSED.\n\n");
		printf("\tProceeding to test of SaveImageToBitmapFile.\n\n");

		char outputBuffer[256];
		printf("\tPassing NULL to SaveImageToBitmapFile...\n\t");

		result = SaveImageToBitmapFile(NULL, pImage);

		if (result)
		{
			printf("\n\tFAILED.\n");
			testCaseFailed = true;
		}
		else
		{
			printf("\n\tPASSED.\n\n");
		}

		printf("\tPassing empty string to SaveImageToBitmapFile...\n\t");

		result = SaveImageToBitmapFile("", pImage);

		if (result)
		{
			printf("\n\tFAILED.\n");
			testCaseFailed = true;
		}
		else
		{
			printf("\n\tPASSED.\n\n");
		}

		printf("\tPassing non-BMP file to SaveImageToBitmapFile...\n\t");

		result = SaveImageToBitmapFile(pNonBmpFile, pImage);

		if (result)
		{
			printf("\n\tFAILED.\n");
			testCaseFailed = true;
		}
		else
		{
			printf("\n\tPASSED.\n\n");
		}

		sprintf(outputBuffer, "%s\\TestPart2CopyOfOriginal1.bmp", pOutputPath);

		printf("\tPassing NULL Image pointer to SaveImageToBitmapFile...\n\t");

		result = SaveImageToBitmapFile(outputBuffer, NULL);

		if (result)
		{
			printf("\n\tFAILED.\n");
			testCaseFailed = true;
		}
		else
		{
			printf("\n\tPASSED.\n\n");
		}

		printf("\tPassing good parameters to SaveImageToBitmapFile...\n\t");

		result = SaveImageToBitmapFile(outputBuffer, pImage);

		if (!result)
		{
			printf("\n\tFAILED.\n");
			testCaseFailed = true;
		}
		else
		{
			printf("\n\tPASSED.\n\n");
			printf("\tProceeding to test of CreateClonedImage.\n\n");

			printf("\tPassing NULL to CreateClonedImage...\n\t");

			pClone = CreateClonedImage(NULL);

			if (pClone)
			{
				printf("\n\tFAILED.\n");
				testCaseFailed = true;
			}
			else
			{
				printf("\n\tPASSED.\n\n");
			}

			printf("\tPassing good parameter to CreateClonedImage...\n\t");

			pClone = CreateClonedImage(pImage);

			if (!pClone ||
				pClone == pImage ||
				pClone->pixelCount != pImage->pixelCount ||
				pClone->pFirstPixel == pImage->pFirstPixel ||
				!pClone->pFirstPixel)
			{
				printf("\n\tFAILED.\n");
				testCaseFailed = true;
			}
			else
			{
				printf("\n\tPASSED.\n\n");

				sprintf(outputBuffer, "%s\\TestPart2CopyOfOriginal2.bmp", pOutputPath);

				printf("\tPassing image clone to SaveImageToBitmapFile...\n\t");

				result = SaveImageToBitmapFile(outputBuffer, pClone);

				if (!result)
				{
					printf("\n\tFAILED.\n");
					testCaseFailed = true;
				}
				else
				{
					printf("\n\tPASSED.\n\n");

					printf("\tComparing clone image to original image...\n\t");

					FILE *fpOriginal = fopen(pInputFileName, "rb");
					FILE *fpClone = fopen(outputBuffer, "rb");
					bool passed = true;

					if (!fpOriginal || !fpClone)
					{
						passed = false;
					}
					else
					{
						fseek(fpOriginal, 0L, SEEK_END);
						long sizeOriginal = ftell(fpOriginal);
						rewind(fpOriginal);

						fseek(fpClone, 0L, SEEK_END);
						long sizeClone = ftell(fpClone);
						rewind(fpClone);

						if (sizeOriginal != sizeClone)
						{
							passed = false;
						}
						else
						{
							char buffer1[1024];
							char buffer2[1024];

							while (!feof(fpOriginal))
							{
								unsigned bytesReadOriginal = fread(buffer1, 1, sizeof(buffer1), fpOriginal);
								unsigned bytesReadClone = fread(buffer2, 1, sizeof(buffer2), fpClone);

								if (bytesReadOriginal != bytesReadClone)
								{
									passed = false;
									break;
								}
								else
								{
									if (memcmp(buffer1, buffer2, bytesReadOriginal) != 0)
									{
#ifdef DEBUG
										BinaryErrorFinder(buffer1, buffer2, bytesReadOriginal);
#endif // finds where the memcpy is failing
										passed = false;
										break;
									}
								}
							}
						}

						fclose(fpOriginal);
						fclose(fpClone);
					}

					if (passed)
					{
						printf("\n\tPASSED.\n\n");
					}
					else
					{
						printf("\n\tFAILED.\n");
						testCaseFailed = true;
					}

					printf("\tProceeding to test of DestroyImage.\n\n");

					printf("\tPassing NULL to DestroyImage...\n\t");

					result = DestroyImage(NULL);

					if (result)
					{
						printf("\n\tFAILED.\n");
						testCaseFailed = true;
					}
					else
					{
						printf("\n\tPASSED.\n\n");
					}

					printf("\tPassing original image to DestroyImage...\n\t");

					result = DestroyImage(pImage);

					if (!result)
					{
						printf("\n\tFAILED.\n");
						testCaseFailed = true;
					}
					else
					{
						printf("\n\tPASSED.\n\n");
					}

					printf("\tPassing clone image to DestroyImage...\n\t");

					result = DestroyImage(pClone);

					if (!result)
					{
						printf("\n\tFAILED.\n");
						testCaseFailed = true;
					}
					else
					{
						printf("\n\tPASSED.\n\n");
					}
				}
			}
		}
	}

	if (testCaseFailed)
	{
		printf("\n\tFAILED: One or more test cases failed.\n");
		exit(EXIT_FAILURE);
	}

	printf("\n\tSUCCESS: Test cases PASSED.\n\n");
}

/*
TestPart3

COPY THIS SOURCE CODE into your main.c file before the main function.

This function drives a series of filtering operations, using the original image in
argv[1], and generating output in the folder in argv[2].  From argv[3], we expect to
see filter names. The filters are applied in sequence, so that the output of one
filter becomes the input to the next filter. Thus, the resulting images show the
cumulative effects of the filters.

Conditional compilation is used enable/disable various filters using preprocessor
symbols.
*/
static void TestPart3(int argc, char *argv[])
{
	static const char *filterNames[] =
	{
		"negative", "sepia", "binary", "grayscale", "solarized", "boxblur", "gaussianblur", "posterize"
	};

	static const unsigned filterNameCount = sizeof(filterNames) / sizeof(filterNames[0]);

	typedef enum ImageFilters
	{
		NEGATIVE = 0,
		SEPIA, BINARY, GRAYSCALE, SOLARIZED,
		BOXBLUR, GAUSSIANBLUR, POSTERIZE
	} ImageFilters;

	char *pInputFile = argv[1];
	char *pOutputFolder = argv[2];
	Image *pImage = NULL;
	Image *pClonedImage = NULL;
	unsigned outputFileNumber = 0;

	printf("\n\nStarting TestPart3...\n\n");

	// See if the specific bitmap file exists and is valid.

	pImage = CreateImageFromBitmapFile(pInputFile);

	if (!pImage)
	{
		printf("Invalid file: %s\n", pInputFile);
		exit(EXIT_FAILURE);
	}

	// See if the output folder exists, by writing our required copy of the image.

	char outputPath[256];
	sprintf(outputPath, "%s\\%05u-%s.bmp", pOutputFolder, outputFileNumber++, "CopyOfOriginal");

	if (!SaveImageToBitmapFile(outputPath, pImage))
	{
		printf("Unable to write to %s folder.\n", pOutputFolder);
		exit(EXIT_FAILURE);
	}

	pClonedImage = CreateClonedImage(pImage);

	if (!pClonedImage)
	{
		printf("Unable to clone image.\n");
		exit(EXIT_FAILURE);
	}
	else
	{
		if (!DestroyImage(pClonedImage))
		{
			printf("Unable to destroy cloned image.\n");
		}
	}

	sprintf(outputPath, "%s\\%05u-%s.bmp", pOutputFolder, outputFileNumber++, "CloneOfOriginal");

	if (!SaveImageToBitmapFile(outputPath, pImage))
	{
		printf("Unable to write to %s folder.\n", pOutputFolder);
		exit(EXIT_FAILURE);
	}

	for (int filterRequest = 3; filterRequest < argc; filterRequest++)
	{
		unsigned filterIndex;

		for (filterIndex = 0; filterIndex < filterNameCount; filterIndex++)
		{
			if (strcmp(filterNames[filterIndex], argv[filterRequest]) == 0)
			{
				bool generatedImage = true;

				printf("Processing filter: %s...\n", argv[filterRequest]);

				switch (filterIndex)
				{
				case NEGATIVE:

#ifdef NEGATIVE_FILTER_IMPLEMENTED

					if (!ConvertImageToNegative(pImage))
					{
						printf("Error detected on call to ConvertImageToNegative.\n");
						generatedImage = false;
					}
#else
					printf("ConvertImageToNegative is not implemented.\n");
					generatedImage = false;
#endif

					break;

				case SEPIA:

#ifdef SEPIA_FILTER_IMPLEMENTED

					if (!ConvertImageToSepia(pImage))
					{
						printf("Error detected on call to ConvertImageToSepia.\n");
					}

#else
					printf("ConvertImageToSepia is not implemented.\n");
					generatedImage = false;
#endif

					break;

				case BINARY:

#ifdef BINARY_FILTER_IMPLEMENTED

					if (!ConvertImageToBinary(pImage, 130.5))
					{
						printf("Error detected on call to ConvertImageToBinary.\n");
					}

#else
					printf("ConvertImageToBinary is not implemented.\n");
					generatedImage = false;
#endif

					break;

				case GRAYSCALE:

#ifdef GRAYSCALE_FILTER_IMPLEMENTED

					if (!ConvertImageToGrayscale(pImage))
					{
						printf("Error detected on call to ConvertImageToGrayscale.\n");
					}

#else
					printf("ConvertImageToGrayscale is not implemented.\n");
					generatedImage = false;
#endif

					break;

				case SOLARIZED:

#ifdef SOLARIZED_FILTER_IMPLEMENTED

					if (!ConvertImageToSolarized(pImage, 120.1))
					{
						printf("Error detected on call to ConvertImageToSolarized.\n");
					}

#else
					printf("ConvertImageToSolarized is not implemented.\n");
					generatedImage = false;
#endif

					break;

				case BOXBLUR:

#ifdef BOXBLUR_FILTER_IMPLEMENTED

				{
					Image *pNewImage = CreateBoxBlurImage(pImage, 10);

					if (!pNewImage)
					{
						printf("Error detected on call to CreateBoxBlurredImage.\n");
					}
					else
					{
						DestroyImage(pImage);
						pImage = pNewImage;
						pNewImage = NULL;
					}
				}

#else
					printf("CreateBoxBlurredImage is not implemented.\n");
					generatedImage = false;
#endif

					break;

				case GAUSSIANBLUR:

#ifdef GAUSSIANBLUR_FILTER_IMPLEMENTED

				{
					Image *pNewImage = CreateGaussianBlurImage(pImage, 20, 2.7);

					if (!pNewImage)
					{
						printf("Error detected on call to CreateGaussianBlurImage.\n");
					}
					else
					{
						DestroyImage(pImage);
						pImage = pNewImage;
						pNewImage = NULL;
					}
				}

#else
					printf("CreateGaussianBlurImage is not implemented.\n");
					generatedImage = false;
#endif

					break;

				default:

					printf("We should never get here!\n");
					generatedImage = false;
				}

				if (generatedImage)
				{
					// Save the result after this filter has been applied.

					sprintf(outputPath, "%s\\%05u-Filtered-%s.bmp", pOutputFolder, outputFileNumber++, argv[filterRequest]);

					if (!SaveImageToBitmapFile(outputPath, pImage))
					{
						printf("Unable to write file: %s.\n", outputPath);
					}
				}

				break;		// Leave the loop that matches strings.
			}
		}

		if (filterIndex >= filterNameCount)
		{
			printf("Invalid filter name \"%s\".\n", argv[filterRequest]);
			exit(EXIT_FAILURE);
		}
	}

	if (!DestroyImage(pImage))
	{
		printf("Unable to destroy image.\n");
		exit(EXIT_FAILURE);
	}
}


int main(int argc, char *argv[])
{
	//TestPart1();
	//TestPart2(argv[1], argv[2]);

	TestPart3(argc, argv);

	return 0;
}