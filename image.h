/*
                ***** image.h *****

Header file for various image processing applications.

Author: John M. Weiss, Ph.D.
Class:  CSC 315 Data Structures & Algorithms
Date:   Spring 2018
*/
#include "QuadTree.h"

#ifndef IMAGE_H
#define IMAGE_H

// convenience data type
typedef unsigned char byte;

// function prototypes
byte** alloc2D( int nrows, int ncols );
void free2D( byte** image );
byte** readPNG( char* filename, unsigned& width, unsigned& height );
byte** grayscale( unsigned width, unsigned height, byte** imgRGB );
void initOpenGL( const char *filename, unsigned w, unsigned h, byte** imgGray, 
QuadTree* tree, int thresh);

#endif
