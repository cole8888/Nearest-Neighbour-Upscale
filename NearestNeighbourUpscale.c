/*
	Cole L - 5th January 2022 - https://github.com/cole8888/Nearest-Neighbour-Upscale

	Upscale a provided image using nearest neighbour upscaling.

	"long long int" is necessary in some places because large images can overwhelm "int" and "long int" due to there being 3 or 4 channels per pixel.
*/

#include "NearestNeighbourUpscale.h"

// Performs nearest neighbour upscaling of the original image where each pixel in the original image is expanded into an expanded pixel of size scale^2 in the upscaled image.
// Used for 32bit RGBA or 24bit RGB images that have been read in as RGBA images.
void upscaleNN_RGBA(u_char *originalImg, u_char *upscaledImg, int dimX, int dimY, int scale){
	long long int scaledDimX = (long long int)(dimX * scale);	// X Dimension of the upscaled image.
	long long int scaledDimY = (long long int)(dimY * scale);	// Y Dimension of the upscaled image.

	// STEP 1. Place each pixel from the original image into the top-left corner of it's respective expanded pixel in the scaled up image.
	#pragma omp parallel for
	for(long long int pixels = 0; pixels < dimX * dimY; pixels++){
		long long int rowsOffset = (pixels / dimX) * (long long int)(pixels != 0) * scaledDimX * (long long int)(scale - 1) * (long long int)CHANNELS_PER_PIXEL_RGBA;	// Account for the rows that come before where we want to write.
		long long int colsOffset = pixels * (long long int)scale * (long long int)CHANNELS_PER_PIXEL_RGBA;	// Account for the number of expanded pixels that come before where we want to write in this row.
		
		upscaledImg[colsOffset + rowsOffset] = originalImg[pixels * (long long int)CHANNELS_PER_PIXEL_RGBA];										// R
		upscaledImg[colsOffset + rowsOffset + (long long int)1] = originalImg[pixels * (long long int)CHANNELS_PER_PIXEL_RGBA + (long long int)1];	// G
		upscaledImg[colsOffset + rowsOffset + (long long int)2] = originalImg[pixels * (long long int)CHANNELS_PER_PIXEL_RGBA + (long long int)2];	// B
		upscaledImg[colsOffset + rowsOffset + (long long int)3] = originalImg[pixels * (long long int)CHANNELS_PER_PIXEL_RGBA + (long long int)3];	// A
	}

	// STEP 2. Go through all the rows containing the topmost row of each expanded pixel and fill in the blank spots with the data from the top-left pixel which we placed in step 1.
	#pragma omp parallel for
	for(long long int rows = 0; rows < dimY; rows++){
		long long int rowsOffset = rows * (long long int)scale * scaledDimX * (long long int)CHANNELS_PER_PIXEL_RGBA;	// Account for the rows that come before where we want to write.
		// For all the expanded pixels in the current row.
		for(long long int cols = 0; cols < dimX; cols++){
			long long int offset = rowsOffset + (cols * (long long int)scale * (long long int)CHANNELS_PER_PIXEL_RGBA);	// Account for the rowsOffset and the number of expanded pixels we have already written to in this row.
			
			// Copy the leftmost pixel to the other blank pixels in this row of the expanded pixel. (Starts at 1 because that is the one that was populated in step 1 and we are copying from it.)
			for(int pixelCol = 1; pixelCol < scale; pixelCol++){
				upscaledImg[offset + (long long int)(pixelCol * CHANNELS_PER_PIXEL_RGBA)] = upscaledImg[offset];										// R
				upscaledImg[offset + (long long int)(pixelCol * CHANNELS_PER_PIXEL_RGBA) + (long long int)1] = upscaledImg[offset + (long long int)1];	// G
				upscaledImg[offset + (long long int)(pixelCol * CHANNELS_PER_PIXEL_RGBA) + (long long int)2] = upscaledImg[offset + (long long int)2];	// B
				upscaledImg[offset + (long long int)(pixelCol * CHANNELS_PER_PIXEL_RGBA) + (long long int)3] = upscaledImg[offset + (long long int)3];	// A
			}
		}
	}

	// STEP 3. Complete the remaining rows by cloning the completed first row of the expanded pixels we placed in step 2 into all of the blank rows remaining of the expanded pixels.
	#pragma omp parallel for
	for(long long int rows = 1; rows < scaledDimY; rows++){
		// Make sure this row is not one of the top rows of an expanded pixel.
		if((rows % (long long int)scale) != 0){
			long long int rowsOffset = rows * scaledDimX * (long long int)CHANNELS_PER_PIXEL_RGBA;	// Account for the rows that come before where we want to write.
			long long int copyFromOffset = (rows - (rows % (long long int)scale)) * scaledDimX * (long long int)CHANNELS_PER_PIXEL_RGBA;	// Figure out which row we should copy data from. (Should be the first row in this expanded pixel.)
			
			// For all pixels in this row of the expanded pixel, copy the value from the first row of this expanded pixel which was filled out in STEP 2.
			for(long long int pixelCol = 0; pixelCol < scaledDimX; pixelCol++){
				long long int pixelOffset = pixelCol * (long long int)CHANNELS_PER_PIXEL_RGBA;	// Account for how many pixels we have written to in this row already.
				
				upscaledImg[pixelOffset + rowsOffset] = upscaledImg[copyFromOffset + pixelOffset];											// R
				upscaledImg[pixelOffset + rowsOffset + (long long int)1] = upscaledImg[copyFromOffset + pixelOffset + (long long int)1];	// G
				upscaledImg[pixelOffset + rowsOffset + (long long int)2] = upscaledImg[copyFromOffset + pixelOffset + (long long int)2];	// B
				upscaledImg[pixelOffset + rowsOffset + (long long int)3] = upscaledImg[copyFromOffset + pixelOffset + (long long int)3];	// A
			}
		}
	}
}

// Performs nearest neighbour upscaling of the original image where each pixel in the original image is expanded into an expanded pixel of size scale^2 in the upscaled image.
// Used for 24bit RBG images.
void upscaleNN_RGB(u_char *originalImg, u_char *upscaledImg, int dimX, int dimY, int scale){
	long long int scaledDimX = (long long int)(dimX * scale);	// X Dimension of the upscaled image.
	long long int scaledDimY = (long long int)(dimY * scale);	// Y Dimension of the upscaled image.

	// STEP 1. Place each pixel from the original image into the top-left corner of it's respective expanded pixel in the scaled up image.
	#pragma omp parallel for
	for(long long int pixels = 0; pixels < dimX * dimY; pixels++){
		long long int rowsOffset = (pixels / dimX) * (long long int)(pixels != 0) * scaledDimX * (long long int)(scale - 1) * (long long int)CHANNELS_PER_PIXEL_RGB;	// Account for the rows that come before where we want to write.
		long long int colsOffset = pixels * (long long int)scale * (long long int)CHANNELS_PER_PIXEL_RGB;	// Account for the number of expanded pixels that come before where we want to write in this row.
		
		upscaledImg[colsOffset + rowsOffset] = originalImg[pixels * (long long int)CHANNELS_PER_PIXEL_RGB];											// R
		upscaledImg[colsOffset + rowsOffset + (long long int)1] = originalImg[pixels * (long long int)CHANNELS_PER_PIXEL_RGB + (long long int)1];	// G
		upscaledImg[colsOffset + rowsOffset + (long long int)2] = originalImg[pixels * (long long int)CHANNELS_PER_PIXEL_RGB + (long long int)2];	// B
	}

	// STEP 2. Go through all the rows containing the topmost row of each expanded pixel and fill in the blank spots with the data from the top-left pixel which we placed in step 1.
	#pragma omp parallel for
	for(long long int rows = 0; rows < dimY; rows++){
		long long int rowsOffset = rows * (long long int)scale * scaledDimX * (long long int)CHANNELS_PER_PIXEL_RGB;	// Account for the rows that come before where we want to write.
		// For all the expanded pixels in the current row.
		for(long long int cols = 0; cols < dimX; cols++){
			long long int offset = rowsOffset + (cols * (long long int)scale * (long long int)CHANNELS_PER_PIXEL_RGB);	// Account for the rowsOffset and the number of expanded pixels we have already written to in this row.
			
			// Copy the leftmost pixel to the other blank pixels in this row of the expanded pixel. (Starts at 1 because that is the one that was populated in step 1 and we are copying from it.)
			for(int pixelCol = 1; pixelCol < scale; pixelCol++){
				upscaledImg[offset + (long long int)(pixelCol * CHANNELS_PER_PIXEL_RGB)] = upscaledImg[offset];											// R
				upscaledImg[offset + (long long int)(pixelCol * CHANNELS_PER_PIXEL_RGB) + (long long int)1] = upscaledImg[offset + (long long int)1];	// G
				upscaledImg[offset + (long long int)(pixelCol * CHANNELS_PER_PIXEL_RGB) + (long long int)2] = upscaledImg[offset + (long long int)2];	// B
			}
		}
	}

	// STEP 3. Complete the remaining rows by cloning the completed first row of the expanded pixels we placed in step 2 into all of the blank rows remaining of the expanded pixels.
	#pragma omp parallel for
	for(long long int rows = 1; rows < scaledDimY; rows++){
		// Make sure this row is not one of the top rows of an expanded pixel.
		if((rows % (long long int)scale) != 0){
			long long int rowsOffset = rows * scaledDimX * (long long int)CHANNELS_PER_PIXEL_RGB;	// Account for the rows that come before where we want to write.
			long long int copyFromOffset = (rows - (rows % (long long int)scale)) * scaledDimX * (long long int)CHANNELS_PER_PIXEL_RGB;	// Figure out which row we should copy data from. (Should be the first row in this expanded pixel.)
			// For all pixels in this row of the expanded pixel, copy the value from the first row of this expanded pixel which was filled out in STEP 2.
			for(long long int pixelCol = 0; pixelCol < scaledDimX; pixelCol++){
				long long int pixelOffset = pixelCol * (long long int)CHANNELS_PER_PIXEL_RGB;
				
				upscaledImg[pixelOffset + rowsOffset] = upscaledImg[copyFromOffset + pixelOffset];											// R
				upscaledImg[pixelOffset + rowsOffset + (long long int)1] = upscaledImg[copyFromOffset + pixelOffset + (long long int)1];	// G
				upscaledImg[pixelOffset + rowsOffset + (long long int)2] = upscaledImg[copyFromOffset + pixelOffset + (long long int)2];	// B
			}
		}
	}
}
