# Nearest-Neighbor-Upscale
C program to quickly perform nearest neighbour upscaling of an image.
Example driver code works on 24bit or 32bit PNG images. Upscaler functions should work with any image format if you can extract the values of each pixel in the image into a u_char array.

To use the example driver code:
- `git clone https://github.com/cole8888/Nearest-Neighbour-Upscale`
- `cd Nearest-Neighbour-Upscale`
- `make`
- `./NearestNeighbourUpscale <INPUT_IMAGE.PNG> <SCALE>`
  - Where `<INPUT_IMAGE.PNG>` is the image you want to upscale and `<SCALE>` the the scale you want to upscale it by.

# How it works:
For this example we will take this 25x25px image and upscale it to 50x50px.

(Please note I've upscaled the images in this demo by 10x so that they can easily be viewed without zooming in.)

![upscaled_img79](https://user-images.githubusercontent.com/32819560/148020448-0e3c5614-9ea5-4781-9513-26995b7e70e5.png)

- Step 1: Copy the pixels from the original image into the topleft corner of each expanded pixel
![0624](https://user-images.githubusercontent.com/32819560/148003124-f89114e3-4e99-43fb-ac90-34a429cb4c4d.png)

- Step 2: Fill in the top row of each expanded pixel with the data from the topleft-most pixel in that expanded pixel.
![1249](https://user-images.githubusercontent.com/32819560/148003211-6ee86265-4f49-4d19-b965-95dbbadea093.png)

- Step 3: Fill in the remaining rows of each expanded pixel with the top row of each expanded pixel.
![2499](https://user-images.githubusercontent.com/32819560/148003252-d34fb355-287f-4132-acbf-bd6db3b30cc0.png)

(See the VIDEO_DEMOS directory if you'd like a visual representation.)

The example driver code uses lodepng which can be found here https://github.com/lvandeve/lodepng
