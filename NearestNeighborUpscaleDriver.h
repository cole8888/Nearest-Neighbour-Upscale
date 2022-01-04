/*
	Cole L - 4th December 2021 - https://github.com/cole8888/Nearest-Neighbor-Upscale
*/

#ifndef NEARESTNEIGHBORUPSCALEDRIVER_H
#define NEARESTNEIGHBORUPSCALEDRIVER_H

#define _GNU_SOURCE

#include "LODEPNG/lodepng.h"
#include "NearestNeighborUpscale.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>

#define DEFAULT_FILENAME "upscaled_img"
#define BACKUP_FILENAME "upscaled_img_backupName.png"
#define FILENAME_BUFFER_SIZE 255
#define PATH_SEPERATOR "/"

// If you know your image(s) will not have an alpha channel (or you do not care if it is missing in the upscaled image), you can change this to 3 and you will get a small speed boost.
#define CHANNELS_PER_PIXEL 4

// Calculate the amount of time in seconds between the provided start and finish timespec structs.
double getElapsedTime(struct timespec start, struct timespec finish);

// Functions to determine whether or not a string starts with or ends with a particular string.
bool endsWith(char* str, char* toCheck);
bool startsWith(char* str, char* toCheck);

// Construct a filepath from two strings. sizeFile is the number of valid characters in the file string.
char *path_join(char* dir, char* file, int sizeFile);

// Determine how many digits are in a base 10 integer.
int getIntDigits(int num);

// Save the image, do not overwrite any previous images.
void saveImg(u_char *img, int dimX, int dimY);

// Load the image from disk. (Handles RGBA (32bit) and RGB (24bit) without issue, so I use by default).
bool loadImgRGBA(u_char** img, u_int* width, u_int* height, char* filename);

// Load the image from disk. RGBA images will be stripped of their alpha channel.
bool loadImgRGB(u_char** img, u_int* width, u_int* height, char* filename);

// Parse the arguments and coordinate the calling of other functions.
int main(int argc, char *argv[]);

#endif