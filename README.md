Ensure you have the OpenGL Glut library installed. This is needed to render the GUI. On Linux, do
```
sudo apt-get install freeglut3-dev
```
## Build
To build and compile all source files, do `make`. 
## Run 
Build outputs an executable called `quadtree`. You can run this like `./quadtree [image]` from within the directory.
## Program
This program takes in an image and converts it into its QuadTree representation. This program reads in an NxN png file and displays a compressed grayscale 
version of it using OpenGL. We present an interface which allows the user to change range (basically the quality) of the image. The image is compressed by recursively subdividing the image into 4 quads, and checking if all the pixel in each quad is within a certain range. If it is not, the quadrant is further subdivided into 4 more quads. White lines can be displayed over the  compressed image to get a better detail of how the image is compressed. Lines
can be toggled and range can be changed using the keyboard.

Compression works as follows: 

The grayscale values of the original grayscaled image are converted into a quadtree, where each individual pixel is a leaf node in that tree. Each node 
represent a quad in the image, and stores the mean (and a few others to help with the program) of the pixels in that quadrant inside the node. We can then decode that quadtree into a compressed 2-D grayscale array by recursing into the tree until the range is met. This allows us to make just one quadtree for the entire run of the program, and given a range, construct the 2-D array and display it to the screen each time it is changed. 

## Usage
Show white lines: Spacebar

Increase range (compress): '+' key

Decrease range (decompress): '-' key

## Preview

![preview](https://github.com/Dillon-Roller/Quad-Tree/blob/main/example.png)
