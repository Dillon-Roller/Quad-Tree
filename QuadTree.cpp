/*
    Implementation of quadtree functions
*/


#include "QuadTree.h"

//Constructor for a quad tree
QuadTree::QuadTree() 
{
    root = nullptr;
}
//Destructor for quad tree
QuadTree::~QuadTree()
{
    cleanTree(this->root);
}
/*
	This Function takes in a quad from a quad tree and calculates its
	mean, max, and min grayscale color by looking at each pixel within
	the quad.
*/
void QuadTree::processQuad(node*& quad, byte** arr, point start, int length) 
{
    quad->mean = 0;
    quad->maxColor = arr[start.i][start.j];
    quad->minColor = arr[start.i][start.j];
    
    for (int i = start.i; i < start.i + length; i++) 
    {
        for (int j = start.j; j < start.j + length; j++) 
        {
            quad->mean += arr[i][j];
            if (arr[i][j] > quad->maxColor) quad->maxColor = arr[i][j];
            if (arr[i][j] < quad->minColor) quad->minColor = arr[i][j];
        }   
    }   
    quad->mean /= length*length;
}


/*
	This is a recursive function that takes in an image as a 2D array and 
	converts it into 4 quads starting with the upper left. This process repeats
	for all four quads until it finds that the quad is 1 pixel in length. 
*/
void QuadTree::createTree(byte** arr, point start, int length, node*& root) 
{
    root = new(nothrow) node(length, start);
    processQuad(root, arr, start, length);
    if (length == 1) //base case
    {
    	return;
    }
    int quadLength = length / 2;
    //UL
    createTree(arr, start, quadLength, root->UL);
    //UR
    createTree(arr, point(start.i, start.j + quadLength), quadLength, root->UR);
    //LL
    createTree(arr, point(start.i + quadLength, start.j), quadLength, root->LL);
    //LR
    createTree(arr, point(start.i + quadLength, start.j + quadLength), quadLength, root->LR);
}

/*
	This is a helper function to call the create tree function that splits 
	the image into quads.
*/
void QuadTree::compress(byte** arr, int length)
{
	createTree(arr, point(0, 0), length, this->root);
}

/*
	This is a helper function to call the Compress Array function that takes a 
	range and quadtree, creating a 2D array as it goes.
*/
void QuadTree::create2dArray(byte** arr, byte** arr2, int range) 
{
    leafCount = nodeCount = 0; //reset leaf and node count;
    
    //recursively compress and store values into arrays
    compressArray(arr, arr2, this->root, range); 
    
    //fix missing white pixels for right and bottom side of root quad
    for (int i = 0; i < root->length; i++)
    {
        arr2[i][root->length - 1] = 255;
        arr2[root->length - 1][i] = 255;
    }
}

/*
    Given the range, gets compression values from quadtree and stores them into
    the two arrays, one for just displaying the compression, and the other
    for storing 
*/
void QuadTree::compressArray(byte** arr, byte** arr2, node* root, int range) 
{
    nodeCount++;
    if(root->maxColor - root->mean <= range && root->mean - root->minColor <= range)
    {
        leafCount++;
        for(int i = root->p.i; i < root->p.i + root->length; i++) 
        {
            for(int j = root->p.j; j < root->p.j + root->length; j++) 
            {
                arr[i][j] = root->mean;
                arr2[i][j] = root->mean;
            }
        }
        
        //fills in white pixels for lines in lines array
        //white lines left side of quad
        for (int i = root->p.i; i < root->p.i + root->length; i++)
        {
            arr2[i][root->p.j] = 255;
        }
        //while lines for top side of quad
        for (int j = root->p.j; j < root->p.j + root->length; j++)
        {
            arr2[root->p.i][j] = 255;
        }
        return;
    }
    //recursively process this quad
    compressArray(arr, arr2, root->UL, range);
    compressArray(arr, arr2, root->UR, range);
    compressArray(arr, arr2, root->LL, range);
    compressArray(arr, arr2, root->LR, range);
}

//Returns node count
int QuadTree::retrieveNodes() 
{ 
	return nodeCount; 
}
//Returns leaf count
int QuadTree::retrieveLeafNodes()
{
    return leafCount;
}
//Constructor for a point
QuadTree::point::point(int I, int J) 
{
    i = I;
    j = J;
}
//Constructor for a node
QuadTree::node::node(int quadLength, point P) 
{ 
	UL = UR = LL = LR = nullptr;
	length = quadLength;
	p = P;
}
//cleans up allocated memory of quadtree
void QuadTree::cleanTree(node* root)
{
    if (root == nullptr)
        return;
    cleanTree(root->UL);
    cleanTree(root->UR);
    cleanTree(root->LL);
    cleanTree(root->LR);
    
    delete root;
}
