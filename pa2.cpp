/*
                ***** pa2 *****

Author: John M. Weiss, Ph.D.
Class:  CSC 315 Data Structures & Algorithms
Date:   Spring 2018

** Edited by: Dillon Roller & Michael Ahlers

This program reads in an NxN png file and displays a compressed grayscale 
version of it using OpenGL. We present an interface which allows the user to 
change range (basically the quality) of the image. The image is compressed
by recursively subdividing the image into 4 quads, and checking if all the 
pixel in each quad is within a certain range. If it is not, the quadrant is
further subdivided into 4 more quads. White lines can be displayed over the 
compressed image to get a better detail of how the image is compressed. Lines
can be toggled and range can be changed using the keyboard.

Compression works as follows: 

The grayscale values of the original grayscaled image are converted into a 
quadtree, where each individual pixel is a leaf node in that tree. Each node 
represent a quad in the image, and stores the mean (and a few others to help
with the program) of the pixels in that quadrant inside the node. We can then 
decode that quadtree into a compressed 2-D grayscale array by recursing into
the tree until the range is met. This allows us to make just one quadtree
for the entire run of the program, and given a range, construct the 2-D array
and display it to the screen each time it is changed. 
*/
#include <cstdio>
#include <ctime>
#include <GL/freeglut.h>
#include "image.h"
#include <iostream>
#include "QuadTree.h"

using namespace std;
// main function
/*
Starts program and calls needed functions to run OpenGL and glut
*/
int main( int argc, char *argv[] )
{
    // process command-line arguments
    char* filename;         // input filename
    int thresh = 50;       //quality factor
    switch ( argc )
    {
        case 3:
            thresh = atoi( argv[2] );
            if ( thresh < 1 || thresh > 255 ) thresh = 128;
        case 2:
            filename = argv[1];
            break;
        default:
            printf( "Usage: %s image.png\n", argv[0] );
            return -1;
    }
    
    // read input PNG file into 1-D array of 24-bit RGB color pixels
    printf( "reading %s: ", filename );
    unsigned width = 0, height = 0;
    byte** imageRGB = readPNG( filename, width, height );
    printf( "%d x %d\n", width, height );

    // generate 8-bit grayscale intensity image from 24-bit color image
    printf( "generating 8-bit grayscale image\n" );
    
    //allocated needed variables
    byte** imageGray = grayscale( width, height, imageRGB );
	QuadTree tree;

    // perform various OpenGL initializations
    glutInit( &argc, argv );
    
    initOpenGL( filename, width, height, imageGray, &tree, thresh);

    // go into OpenGL/GLUT main loop, never to return
    glutMainLoop();

    // yeah I know, but it keeps compilers from bitching
    return 0;
}
