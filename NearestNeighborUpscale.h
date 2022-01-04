/*
	Cole L - 4th December 2021 - https://github.com/cole8888/Nearest-Neighbor-Upscale
*/

#ifndef NEARESTNEIGHBORUPSCALE_H
#define NEARESTNEIGHBORUPSCALE_H

#include <stdio.h>
#include <stdlib.h>

// Don't change these, lets the program know how many colour channels there are for RGB and RGBA.
#define CHANNELS_PER_PIXEL_RGB 3
#define CHANNELS_PER_PIXEL_RGBA 4

// Performs nearest neighbor upscaling of the original image where each pixel in the original image is expanded into an expanded pixel of size scale^2 in the upscaled image.
// Used for 32bit RGBA or 24bit RGB images that have been read in as RGBA.
void upscaleNN_RGBA(u_char *originalImg, u_char *scaledImg, int dimX, int dimY, int scale);

// Performs nearest neighbor upscaling of the original image where each pixel in the original image is expanded into an expanded pixel of size scale^2 in the upscaled image.
// Used for 24bit RBG images.
void upscaleNN_RGB(u_char *originalImg, u_char *scaledImg, int dimX, int dimY, int scale);

#endif