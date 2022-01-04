/*
	Cole L - 4th December 2021 - https://github.com/cole8888/Nearest-Neighbour-Upscale

	Driver program to use the NearestNeighbourUpscale code from a commandline program.
	Parses the command line arguments, reads in the image from disk, passes it to the appropriate upscaler function and saves the upscaled image to disk.
*/

#include "NearestNeighbourUpscaleDriver.h"

// Calculate the amount of time in seconds between the provided start and finish timespec structs.
double getElapsedTime(struct timespec start, struct timespec finish){
	return (finish.tv_sec - start.tv_sec) + (finish.tv_nsec - start.tv_nsec) / 1000000000.0;
}

// Functions to determine whether or not a string starts with or ends with a particular string.
bool endsWith(char *str, char *toCheck){
	int n = strlen(str);
	int cl = strlen(toCheck);
	if(n < cl){
		return false;
	}
	char *subbuff = (char *)malloc(cl * sizeof(char));
	memcpy(subbuff, &str[n - cl], cl);
	return (strcmp(subbuff, toCheck) == 0);
}
bool startsWith(char *str, char* toCheck){
	int n = strlen(str);
	int cl = strlen(toCheck);
	if(n < cl){
		return false;
	}
	char *subbuff = (char *)malloc(cl * sizeof(char));
	memcpy(subbuff, &str[0 - cl], cl);
	return (strcmp(subbuff, toCheck) == 0);
}

// Construct a filepath from two strings. sizeFile is the number of valid characters in the file string.
char *path_join(char* dir, char* file, int sizeFile){
	int size = strlen(dir) + sizeFile + 2;
	char *buf = (char*)malloc(size*sizeof(char));
	if(NULL == buf){
		return NULL;
	}
	
	strcpy(buf, dir);

	// If the path does not end with the path seperator then we need to add it.
	if(!endsWith(dir, PATH_SEPERATOR)){
		strcat(buf, PATH_SEPERATOR);
	}
	// If the file starts with the path seperator, ignore it.
	if(startsWith(file, PATH_SEPERATOR)){
		char *filecopy = strdup(file);
		if(NULL == filecopy){
			free(buf);
			return NULL;
		}
		strcat(buf, filecopy + 1);
		free(filecopy);
	}
	else{
		strcat(buf, file);
	}
	return buf;
}

// Determine how many digits are in a base 10 integer.
int getIntDigits(int num){
	int digits = 0;
	while(num != 0){
		num /= 10;
		++digits;
	}
	return digits;
}

// Save the image, do not overwrite any previous images.
void saveImg(u_char *img, int dimX, int dimY){
	char *cwd = get_current_dir_name();	// Current working directory.
	char ext[] = ".png";	// File extention. Must be .PNG
	char buff[FILENAME_BUFFER_SIZE];	// Buffer to store the filename while we figure out what the file should be called.
	char *file;	// Once we settle on a filename, this will hold it.

	// See if an image with this name exists. If so, append a number and keep incrementing that number until we find one where that name is not in use.
	sprintf(buff, "%s%s", DEFAULT_FILENAME, ext);
	if(access(path_join(cwd, buff, strlen(DEFAULT_FILENAME) + strlen(ext)), F_OK) == 0){
		// File with this filename already exists. Let's try another one.
		int num = 2;	// Start by trying to append the number 2.
		int digits = 1;	// 2 has one digit.
		sprintf(buff, "%s%d%s", DEFAULT_FILENAME, num, ext);
		
		// Increment the appended number until we find a name that is not in use.
		while(access(path_join(cwd, buff, digits + strlen(DEFAULT_FILENAME) + strlen(ext)), F_OK) == 0){
			num++;
			digits = getIntDigits(num);
			sprintf(buff, "%s%d%s", DEFAULT_FILENAME, num, ext);
		}
		
		// Save the filename with the number at the end.
		sprintf(buff, "%s%d%s", DEFAULT_FILENAME, num, ext);
		file = path_join(cwd, buff, digits + strlen(DEFAULT_FILENAME) + strlen(ext));
	}
	else{
		// File with this name does not exist, lets use this name for the new file.
		sprintf(buff, "%s%s", DEFAULT_FILENAME, ext);
		file = path_join(cwd, buff, strlen(DEFAULT_FILENAME) + strlen(ext));
	}

	// If the filename is NULL for some reason, let the user know, but still try to save saving the image. (Since this might have taken a long time to compute and there is no need to exit for this error.)
	if(NULL == file){
		fprintf(stderr, "\nError when making filename, it is NULL. Will attempt saving anyway using backup filename\n");
		file = BACKUP_FILENAME;
	}

	// Start the timer and then save the image.
	struct timespec start, finish;
	clock_gettime(CLOCK_MONOTONIC, &start);
	unsigned error;
	
	if(CHANNELS_PER_PIXEL == 4){
		error = lodepng_encode32_file(file, img, dimX, dimY);
	}
	else if(CHANNELS_PER_PIXEL == 3){
		error = lodepng_encode24_file(file, img, dimX, dimY);
	}
	else{
		fprintf(stderr, "CHANNELS_PER_PIXEL in the header file is not a valid value. Must be 3 or 4.\n");
		exit(EXIT_FAILURE);
	}
	
	clock_gettime(CLOCK_MONOTONIC, &finish);

	// See if there was an issue when saving the image.
	if(error){
		fprintf(stderr, "\nUnable to save the image, lodepng returned an error.\nError %u: %s\n", error, lodepng_error_text(error));
	}
	else{
		printf("Saved to %s (%f secs)\n", file, getElapsedTime(start, finish));
	}
}

// Load the image from disk. (Handles RGBA (32bit) and RGB (24bit) without issue, so use by default).
bool loadImgRGBA(u_char** img, u_int* width, u_int* height, char* filename){
	u_char *png = 0;
	size_t pngsize;
	LodePNGState state;

	lodepng_state_init(&state);

	unsigned error = lodepng_load_file(&png, &pngsize, filename);	// Load the file from disk into a temporary buffer in memory.
	if(!error){
		error = lodepng_decode(img, width, height, &state, png, pngsize);
		if(error){
			printf("Lodepng encountered an error.\nError: %u, %s\n", error, lodepng_error_text(error));
			return false;
		}
		free(png);
		lodepng_state_cleanup(&state);
		return true;
	}
	else{
		printf("Lodepng encountered an error.\nError: %u, %s\n", error, lodepng_error_text(error));
		return false;
	}
}

// Load the image from disk. RGBA images will be stripped of their alpha channel.
bool loadImgRGB(u_char** img, u_int* width, u_int* height, char* filename){
	u_char *png = 0;
	size_t pngsize;

	unsigned error = lodepng_load_file(&png, &pngsize, filename);	// Load the file from disk into a temporary buffer in memory.
	if(!error){
		error = lodepng_decode24(img, width, height, png, pngsize);
		if(error){
			printf("Lodepng encountered an error.\nError: %u, %s\n", error, lodepng_error_text(error));
			return false;
		}
		free(png);
		return true;
	}
	else{
		printf("Lodepng encountered an error.\nError: %u, %s\n", error, lodepng_error_text(error));
		return false;
	}
}

// Parse the arguments and coordinate the calling of other functions.
int main(int argc, char *argv[]){
	// Make sure there are exactly 2 arguments.
	if(argc != 3){
		fprintf(stderr, "No input filename, and/or no scale provided.\nUsage: ./upscale <INPUT_IMAGE> <SCALE>\n");
		return EXIT_FAILURE;
	}
	char *filename = argv[1];	// Read in the input image filename from the commandline arguments.
	int scale = 0;

	// Read in the scale from the command line args.
	char *temp;
	long value = strtol(argv[2], &temp, 10);
	if(temp != argv[2] && *temp == '\0'){
		scale = (int)value;
		if(scale < 1){
			fprintf(stderr, "Invalid data in scale argument. Must be a non-zero integer.\nUsage: ./upscale <INPUT_IMAGE> <SCALE>\n");
			return EXIT_FAILURE;
		}
	}
	else{
		fprintf(stderr, "Invalid data in scale argument. Must be a non-zero integer.\nUsage: ./upscale <INPUT_IMAGE> <SCALE>\n");
		return EXIT_FAILURE;
	}

	u_char *img = 0;	// Place to hold the original image we read from disk.
	u_int dimX = 0;	// Width of the original image.
	u_int dimY = 0;	// Height of the original image.

	// Load the image from disk.
	if(CHANNELS_PER_PIXEL == 4){
		// Image will be read as RGBA. If image is actually RGB it's okay, the alpha channel for all pixel will just be filled with 255.
		if(!loadImgRGBA(&img, &dimX, &dimY, filename)){
			return EXIT_FAILURE;
		}
	}
	else if(CHANNELS_PER_PIXEL == 3){
		// Image will be read as RGB. If image is actually RGBA then the alpha channel will be ignored.
		if(!loadImgRGB(&img, &dimX, &dimY, filename)){
			return EXIT_FAILURE;
		}
	}
	else{
		fprintf(stderr, "CHANNELS_PER_PIXEL in the header file is not a valid value. Must be 3 or 4.\n");
		return EXIT_FAILURE;
	}

	// Allocate memory for the upscaled image.
	u_char *upscaledImg = (u_char *)malloc((dimX*scale) * (dimY*scale) * CHANNELS_PER_PIXEL * sizeof(u_char));
	if(NULL == upscaledImg){
		fprintf(stderr, "Unable to allocate upscaledImg array... May have run out of RAM.\n");
		return EXIT_FAILURE;
	}

	printf("Start upscaling the image...\n");

	struct timespec start, finish;
	clock_gettime(CLOCK_MONOTONIC, &start);	// Start the timer.

	if(CHANNELS_PER_PIXEL == 4){
		upscaleNN_RGBA(img, upscaledImg, (int)dimX, (int)dimY, scale);	// Upscale the RGBA image.
	}
	else if(CHANNELS_PER_PIXEL == 3){
		upscaleNN_RGB(img, upscaledImg, (int)dimX, (int)dimY, scale);	// Upscale the RGB image.
	}
	
	clock_gettime(CLOCK_MONOTONIC, &finish);	// Stop the timer.
	printf("Finished upscaling the image.\t(%f secs)\n", getElapsedTime(start, finish));
	
	free(img);	// Free the original image.

	printf("\nStart saving the image...\n");
	saveImg(upscaledImg, dimX*scale, dimY*scale);	// Save the upscaled image to disk.
	
	free(upscaledImg);	// Free the upscaled image,
	return EXIT_SUCCESS;
}