/*
                ***** graphics.cpp *****

OpenGL/GLUT graphics module for image display.

Author: John M. Weiss, Ph.D.
Class:  CSC 315 Data Structures & Algorithms
Date:   Spring 2018

**Edited by: Dillon Roller & Michael Ahlers

*/

#include <cstdio>
#include <GL/glut.h>
#include <iostream>
#include "image.h"
#include "QuadTree.h"

using namespace std;

typedef unsigned char byte;

// OpenGL callback function prototypes etc.
void display( void );
void reshape( int w, int h );
void keyboard( unsigned char key, int x, int y );
void displayColor( int x, int y, int w, int h, byte** image );
void displayMonochrome( int x, int y, int w, int h, byte** image );
void DrawTextString( char *string, int x, int y, byte r, byte g, byte b );

// symbolic constants
const int ESC = 27;
const char UP = '+';
const char DWN = '-';


// global vars (unfortunately necessary due to OpenGL callback functions)
static unsigned height, width;
static byte** imageMono = NULL;
static byte** imageCompressed = NULL;
static byte** imageLines = NULL;
static QuadTree* tree1 = NULL;
bool lines = false;
int range;
double compression = 0;

/************************************************************/

// various commands to initialize OpenGL and GLUT
void initOpenGL( const char *filename, unsigned w, unsigned h, byte** imgGray, QuadTree* tree,
int thresh)
{
    height = h;
    width = w;
    imageMono = imgGray;
    imageCompressed = alloc2D(height, width);
    imageLines = alloc2D(height, width);
    tree1 = tree;
    range = thresh;
    
    /* INITIALIZING RIGHT IMAGE */
    tree1->compress(imageMono, width); //make tree
    tree1->create2dArray(imageCompressed, imageLines, range); //fill array with compressed values

    glutInitDisplayMode( GLUT_RGB | GLUT_SINGLE );	    // 24-bit graphics and single buffering

    glutInitWindowSize( 2 * width, height + 25);	        // initial window size
    glutInitWindowPosition( 0, 0 );			            // initial window position
    glutCreateWindow( filename );			            // window title

    glClearColor( 0.0, 0.0, 0.0, 0.0 );			        // use black for glClear command

    // callback routines
    glutDisplayFunc( display );				// how to redisplay window
    glutReshapeFunc( reshape );				// how to resize window
    glutKeyboardFunc( keyboard );			// how to handle key presses
}

/************************************************************/

// GLUT display callback
void display()
{
    glClear( GL_COLOR_BUFFER_BIT );
    
    // display color and monochrome images, side by side in window
    displayMonochrome( 0, 0, width, height, imageMono );
    
    if (lines) displayMonochrome( width, 0, width, height, imageLines );
    else displayMonochrome( width, 0, width, height, imageCompressed );
    
    // write text labels (in white)
    char str[257];
    sprintf( str, "Original Image" );
    DrawTextString( str, 200, height + 9, 255, 255, 255 );
    sprintf( str, "Compressed Image, range %d (spacebar toggles quads)", range);
    DrawTextString( str, 570, height + 9, 255, 255, 255 );
	
    glFlush(  );
}

// GLUT reshape callback (handles window resizing)
void reshape( int w, int h )
{
    glViewport( 0, 0, w, h );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity(  );
    gluOrtho2D( 0.0, w, 0.0, h );
}

// GLUT keyboard callback
void keyboard( unsigned char key, int x, int y )
{ 
    switch ( key )
    {
    	case ' ':
    		lines = !lines;
    		break;
    		
    	case UP:
    		if (++range > 255) range = 255;
    		cout << "Range: " << range << '\n';
    		break;
    		
    	case DWN:
    		if (--range < 0) range = 0;
    		cout << "Range: " << range << '\n';
    		break;
    		
        case ESC:
            free2D(imageMono);
            free2D(imageCompressed);
            free2D(imageLines);
            exit(1);
            break;
    }
    //create 2d array given range from quad tree
    tree1->create2dArray(imageCompressed, imageLines, range);
    
    //calculate compression and output info
	compression = 100.0 * 2 * tree1->retrieveLeafNodes() / (width * height);
	
	cout << "Nodes: " << tree1->retrieveNodes() << '\n' 
         << "Leaf Nodes: " << tree1->retrieveLeafNodes() << '\n'
         << "Estimated bytes used: " << 2 * tree1->retrieveLeafNodes() << '\n'
         << "Compression ratio: " << compression << "%\n\n";
         
    glutPostRedisplay();
}

/******************************************************************************/

// display 24-bit color image
void displayColor( int x, int y, int w, int h, byte** image )
{
    glRasterPos2i( x, y );
    glDrawPixels( w, h, GL_RGB, GL_UNSIGNED_BYTE, *image );
}

// display 8-bit monochrome image
void displayMonochrome( int x, int y, int w, int h, byte** image )
{
    glRasterPos2i( x, y );
    glDrawPixels( w, h, GL_LUMINANCE, GL_UNSIGNED_BYTE, *image );
}

// write a text string
void DrawTextString( char *string, int x, int y, byte r, byte g, byte b )
{
    glColor3ub( r, g, b );
    glRasterPos2i( x, y );
    while ( *string )
    {
        glutBitmapCharacter( GLUT_BITMAP_8_BY_13, *string );
        string++;
    }
}
