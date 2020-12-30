/* Header file containing declarations of various classes and structs to
handle Quadtree */

#ifndef QUADTREE_H_
#define QUADTREE_H_

#include <iostream>
typedef unsigned char byte;
using namespace std;

class QuadTree { //holds the structure of a quad tree
    private:
        struct point 
        {
            int i;
            int j;
            point(int I = 0, int J = 0);
        };
        
        struct node 
        { //holds info of a quad
            int maxColor, minColor;
            double mean;
            point p; //top left
            int length; //length of quad
            node *UL, *UR, *LL, *LR;
            
            node(int quadLength, point P); //constructor
        };
        node* root; //root of quad tree
        int leafCount;
        int nodeCount;
        void createTree(byte**, point, int, node*&);
        void processQuad(node*&, byte**, point, int);
	    void compressArray(byte**, byte**, node*, int);
        void cleanTree(node*);
        //void clean(node* root);
    public:
        QuadTree();
        ~QuadTree();
	    void create2dArray(byte**, byte**, int);
        int retrieveNodes();
        int retrieveLeafNodes();
        
        void compress(byte**, int);
        
        
};

#endif
