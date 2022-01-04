# Nearest-Neighbor-Upscale
C program to quickly perform nearest neighbor upscaling of an image.
Example driver code works on 24bit or 32bit PNG images, but the upscaler functions should work with any image format if you can extract the values of each pixel in the image into a u_char array.

How it works:

- Step 1: Copy the pixels from the original image into the topleft corner of each expanded pixel
![0624](https://user-images.githubusercontent.com/32819560/148003124-f89114e3-4e99-43fb-ac90-34a429cb4c4d.png)

- Step 2: Fill in the top row of each expanded pixel with the data from the topleft-most pixel in that expanded pixel.
![1249](https://user-images.githubusercontent.com/32819560/148003211-6ee86265-4f49-4d19-b965-95dbbadea093.png)

- Step 3: Fill in the remaining rows of each expanded pixel with the top row of each expanded pixel.
![2499](https://user-images.githubusercontent.com/32819560/148003252-d34fb355-287f-4132-acbf-bd6db3b30cc0.png)
