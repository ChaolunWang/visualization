/*  brain.cpp*/
/*  programmed by Chaolun Wang */
/*
	keyboard controls:	
	x/X:	Positive/negative X-axis shift of the model
	y/Y:	Positive/negative Y-axis shift of the model
	s/S:    decrease/increase the scale of model
	q, w, e: detailed control
	l,o: control the position of light
	
	UP/DOWN ARROWS:	(zoom) Z-axis shift of the model
	
	LEFT/RIGHT ARROWS:	Y-axis rotations
	PAGE UP/DOWN:	X-axis rotations
	ESC:	exit
*/
#include<stdio.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>
#include<cmath>
#include<iostream>
#include<fstream>
#include<vector>
//#include "jpeglib.h"
//#include "Jpegfile.h"


#define ESC 27
#define BUFFERSIZE 100


using namespace std;
/////////////////////////////////////////////////////////// need modify

static int li=-185, lj=-46, num=100;
const bool full=true;
const char fileName[]="000.ppm";


////////////////////////////////////////////////////////////////////
static GLfloat red[]={2,0,0}, diffuseLight[] = { 1, 0.00, 0.00, 1.0 }, white[]={5,5,5}; 
static int xrot=36,yrot=270,zrot=0;
static GLfloat xshift=-0.05,yshift=0.025,zoom=-1.5;
static int numpoints=0, numtriangle=0, numsegment=0, n=64;  //needs to be changed
static GLfloat scale=30.0, shine=100;


struct point{                          //datasturcture to store point
	GLfloat x;
	GLfloat y;
	GLfloat z;
	GLfloat nx;
	GLfloat ny;
	GLfloat nz;
};

struct segment{
	GLfloat x1;
	GLfloat y1;
	GLfloat z1;
	GLfloat x2;
	GLfloat y2;
	GLfloat z2;	
};

struct XYZ{                          //datasturcture to store point
	double x;
	double y;
	double z;
};

struct triangle{                        //datastructure to store triangle
	int a;
	int b;
	int c;
};
typedef struct {
   XYZ p[3];
} TRIANGLE;

typedef struct {
   XYZ p[8];
   double val[8];
} GRIDCELL;

XYZ VertexInterp(double isolevel,XYZ p1, XYZ p2, double valp1, double valp2);

/*
   Given a grid cell and an isolevel, calculate the triangular
   facets required to represent the isosurface through the cell.
   Return the number of triangular facets, the array "triangles"
   will be loaded up with the vertices at most 5 triangular facets.
	0 will be returned if the grid cell is either totally above
   of totally below the isolevel.
*/
int Polygonise(GRIDCELL grid,double isolevel,TRIANGLE *triangles)
{
   int i,ntriang;
   int cubeindex;
   XYZ vertlist[12];

int edgeTable[256]={
0x0  , 0x109, 0x203, 0x30a, 0x406, 0x50f, 0x605, 0x70c,
0x80c, 0x905, 0xa0f, 0xb06, 0xc0a, 0xd03, 0xe09, 0xf00,
0x190, 0x99 , 0x393, 0x29a, 0x596, 0x49f, 0x795, 0x69c,
0x99c, 0x895, 0xb9f, 0xa96, 0xd9a, 0xc93, 0xf99, 0xe90,
0x230, 0x339, 0x33 , 0x13a, 0x636, 0x73f, 0x435, 0x53c,
0xa3c, 0xb35, 0x83f, 0x936, 0xe3a, 0xf33, 0xc39, 0xd30,
0x3a0, 0x2a9, 0x1a3, 0xaa , 0x7a6, 0x6af, 0x5a5, 0x4ac,
0xbac, 0xaa5, 0x9af, 0x8a6, 0xfaa, 0xea3, 0xda9, 0xca0,
0x460, 0x569, 0x663, 0x76a, 0x66 , 0x16f, 0x265, 0x36c,
0xc6c, 0xd65, 0xe6f, 0xf66, 0x86a, 0x963, 0xa69, 0xb60,
0x5f0, 0x4f9, 0x7f3, 0x6fa, 0x1f6, 0xff , 0x3f5, 0x2fc,
0xdfc, 0xcf5, 0xfff, 0xef6, 0x9fa, 0x8f3, 0xbf9, 0xaf0,
0x650, 0x759, 0x453, 0x55a, 0x256, 0x35f, 0x55 , 0x15c,
0xe5c, 0xf55, 0xc5f, 0xd56, 0xa5a, 0xb53, 0x859, 0x950,
0x7c0, 0x6c9, 0x5c3, 0x4ca, 0x3c6, 0x2cf, 0x1c5, 0xcc ,
0xfcc, 0xec5, 0xdcf, 0xcc6, 0xbca, 0xac3, 0x9c9, 0x8c0,
0x8c0, 0x9c9, 0xac3, 0xbca, 0xcc6, 0xdcf, 0xec5, 0xfcc,
0xcc , 0x1c5, 0x2cf, 0x3c6, 0x4ca, 0x5c3, 0x6c9, 0x7c0,
0x950, 0x859, 0xb53, 0xa5a, 0xd56, 0xc5f, 0xf55, 0xe5c,
0x15c, 0x55 , 0x35f, 0x256, 0x55a, 0x453, 0x759, 0x650,
0xaf0, 0xbf9, 0x8f3, 0x9fa, 0xef6, 0xfff, 0xcf5, 0xdfc,
0x2fc, 0x3f5, 0xff , 0x1f6, 0x6fa, 0x7f3, 0x4f9, 0x5f0,
0xb60, 0xa69, 0x963, 0x86a, 0xf66, 0xe6f, 0xd65, 0xc6c,
0x36c, 0x265, 0x16f, 0x66 , 0x76a, 0x663, 0x569, 0x460,
0xca0, 0xda9, 0xea3, 0xfaa, 0x8a6, 0x9af, 0xaa5, 0xbac,
0x4ac, 0x5a5, 0x6af, 0x7a6, 0xaa , 0x1a3, 0x2a9, 0x3a0,
0xd30, 0xc39, 0xf33, 0xe3a, 0x936, 0x83f, 0xb35, 0xa3c,
0x53c, 0x435, 0x73f, 0x636, 0x13a, 0x33 , 0x339, 0x230,
0xe90, 0xf99, 0xc93, 0xd9a, 0xa96, 0xb9f, 0x895, 0x99c,
0x69c, 0x795, 0x49f, 0x596, 0x29a, 0x393, 0x99 , 0x190,
0xf00, 0xe09, 0xd03, 0xc0a, 0xb06, 0xa0f, 0x905, 0x80c,
0x70c, 0x605, 0x50f, 0x406, 0x30a, 0x203, 0x109, 0x0   };
int triTable[256][16] =
{{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 9, 8, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 0, 2, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 8, 3, 2, 10, 8, 10, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 8, 11, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 2, 1, 9, 11, 9, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 1, 11, 10, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 10, 1, 0, 8, 10, 8, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{3, 9, 0, 3, 11, 9, 11, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 7, 3, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 1, 9, 4, 7, 1, 7, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 4, 7, 3, 0, 4, 1, 2, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 2, 10, 9, 0, 2, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 9, 2, 9, 7, 2, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{8, 4, 7, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 4, 7, 11, 2, 4, 2, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 8, 4, 7, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{4, 7, 11, 9, 4, 11, 9, 11, 2, 9, 2, 1, -1, -1, -1, -1},
{3, 10, 1, 3, 11, 10, 7, 8, 4, -1, -1, -1, -1, -1, -1, -1},
{1, 11, 10, 1, 4, 11, 1, 0, 4, 7, 11, 4, -1, -1, -1, -1},
{4, 7, 8, 9, 0, 11, 9, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{4, 7, 11, 4, 11, 9, 9, 11, 10, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 1, 5, 0, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 5, 4, 8, 3, 5, 3, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 10, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 2, 10, 5, 4, 2, 4, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{2, 10, 5, 3, 2, 5, 3, 5, 4, 3, 4, 8, -1, -1, -1, -1},
{9, 5, 4, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 11, 2, 0, 8, 11, 4, 9, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 5, 4, 0, 1, 5, 2, 3, 11, -1, -1, -1, -1, -1, -1, -1},
{2, 1, 5, 2, 5, 8, 2, 8, 11, 4, 8, 5, -1, -1, -1, -1},
{10, 3, 11, 10, 1, 3, 9, 5, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 0, 8, 1, 8, 10, 1, 8, 11, 10, -1, -1, -1, -1},
{5, 4, 0, 5, 0, 11, 5, 11, 10, 11, 0, 3, -1, -1, -1, -1},
{5, 4, 8, 5, 8, 10, 10, 8, 11, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 5, 7, 9, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 3, 0, 9, 5, 3, 5, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 8, 0, 1, 7, 1, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 7, 8, 9, 5, 7, 10, 1, 2, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 9, 5, 0, 5, 3, 0, 5, 7, 3, -1, -1, -1, -1},
{8, 0, 2, 8, 2, 5, 8, 5, 7, 10, 5, 2, -1, -1, -1, -1},
{2, 10, 5, 2, 5, 3, 3, 5, 7, -1, -1, -1, -1, -1, -1, -1},
{7, 9, 5, 7, 8, 9, 3, 11, 2, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 7, 9, 7, 2, 9, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{2, 3, 11, 0, 1, 8, 1, 7, 8, 1, 5, 7, -1, -1, -1, -1},
{11, 2, 1, 11, 1, 7, 7, 1, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 8, 8, 5, 7, 10, 1, 3, 10, 3, 11, -1, -1, -1, -1},
{5, 7, 0, 5, 0, 9, 7, 11, 0, 1, 0, 10, 11, 10, 0, -1},
{11, 10, 0, 11, 0, 3, 10, 5, 0, 8, 0, 7, 5, 7, 0, -1},
{11, 10, 5, 7, 11, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 8, 3, 1, 9, 8, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 2, 6, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 5, 1, 2, 6, 3, 0, 8, -1, -1, -1, -1, -1, -1, -1},
{9, 6, 5, 9, 0, 6, 0, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 9, 8, 5, 8, 2, 5, 2, 6, 3, 2, 8, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 0, 8, 11, 2, 0, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 1, 9, 2, 9, 11, 2, 9, 8, 11, -1, -1, -1, -1},
{6, 3, 11, 6, 5, 3, 5, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 11, 0, 11, 5, 0, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{3, 11, 6, 0, 3, 6, 0, 6, 5, 0, 5, 9, -1, -1, -1, -1},
{6, 5, 9, 6, 9, 11, 11, 9, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 3, 0, 4, 7, 3, 6, 5, 10, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 5, 10, 6, 8, 4, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 6, 5, 1, 9, 7, 1, 7, 3, 7, 9, 4, -1, -1, -1, -1},
{6, 1, 2, 6, 5, 1, 4, 7, 8, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 5, 5, 2, 6, 3, 0, 4, 3, 4, 7, -1, -1, -1, -1},
{8, 4, 7, 9, 0, 5, 0, 6, 5, 0, 2, 6, -1, -1, -1, -1},
{7, 3, 9, 7, 9, 4, 3, 2, 9, 5, 9, 6, 2, 6, 9, -1},
{3, 11, 2, 7, 8, 4, 10, 6, 5, -1, -1, -1, -1, -1, -1, -1},
{5, 10, 6, 4, 7, 2, 4, 2, 0, 2, 7, 11, -1, -1, -1, -1},
{0, 1, 9, 4, 7, 8, 2, 3, 11, 5, 10, 6, -1, -1, -1, -1},
{9, 2, 1, 9, 11, 2, 9, 4, 11, 7, 11, 4, 5, 10, 6, -1},
{8, 4, 7, 3, 11, 5, 3, 5, 1, 5, 11, 6, -1, -1, -1, -1},
{5, 1, 11, 5, 11, 6, 1, 0, 11, 7, 11, 4, 0, 4, 11, -1},
{0, 5, 9, 0, 6, 5, 0, 3, 6, 11, 6, 3, 8, 4, 7, -1},
{6, 5, 9, 6, 9, 11, 4, 7, 9, 7, 11, 9, -1, -1, -1, -1},
{10, 4, 9, 6, 4, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 10, 6, 4, 9, 10, 0, 8, 3, -1, -1, -1, -1, -1, -1, -1},
{10, 0, 1, 10, 6, 0, 6, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 1, 8, 1, 6, 8, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{1, 4, 9, 1, 2, 4, 2, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 1, 2, 9, 2, 4, 9, 2, 6, 4, -1, -1, -1, -1},
{0, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 3, 2, 8, 2, 4, 4, 2, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 4, 9, 10, 6, 4, 11, 2, 3, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 2, 2, 8, 11, 4, 9, 10, 4, 10, 6, -1, -1, -1, -1},
{3, 11, 2, 0, 1, 6, 0, 6, 4, 6, 1, 10, -1, -1, -1, -1},
{6, 4, 1, 6, 1, 10, 4, 8, 1, 2, 1, 11, 8, 11, 1, -1},
{9, 6, 4, 9, 3, 6, 9, 1, 3, 11, 6, 3, -1, -1, -1, -1},
{8, 11, 1, 8, 1, 0, 11, 6, 1, 9, 1, 4, 6, 4, 1, -1},
{3, 11, 6, 3, 6, 0, 0, 6, 4, -1, -1, -1, -1, -1, -1, -1},
{6, 4, 8, 11, 6, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 10, 6, 7, 8, 10, 8, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 7, 3, 0, 10, 7, 0, 9, 10, 6, 7, 10, -1, -1, -1, -1},
{10, 6, 7, 1, 10, 7, 1, 7, 8, 1, 8, 0, -1, -1, -1, -1},
{10, 6, 7, 10, 7, 1, 1, 7, 3, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 6, 1, 6, 8, 1, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 6, 9, 2, 9, 1, 6, 7, 9, 0, 9, 3, 7, 3, 9, -1},
{7, 8, 0, 7, 0, 6, 6, 0, 2, -1, -1, -1, -1, -1, -1, -1},
{7, 3, 2, 6, 7, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 11, 10, 6, 8, 10, 8, 9, 8, 6, 7, -1, -1, -1, -1},
{2, 0, 7, 2, 7, 11, 0, 9, 7, 6, 7, 10, 9, 10, 7, -1},
{1, 8, 0, 1, 7, 8, 1, 10, 7, 6, 7, 10, 2, 3, 11, -1},
{11, 2, 1, 11, 1, 7, 10, 6, 1, 6, 7, 1, -1, -1, -1, -1},
{8, 9, 6, 8, 6, 7, 9, 1, 6, 11, 6, 3, 1, 3, 6, -1},
{0, 9, 1, 11, 6, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 8, 0, 7, 0, 6, 3, 11, 0, 11, 6, 0, -1, -1, -1, -1},
{7, 11, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 8, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 9, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 9, 8, 3, 1, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{10, 1, 2, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 8, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{2, 9, 0, 2, 10, 9, 6, 11, 7, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 2, 10, 3, 10, 8, 3, 10, 9, 8, -1, -1, -1, -1},
{7, 2, 3, 6, 2, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{7, 0, 8, 7, 6, 0, 6, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{2, 7, 6, 2, 3, 7, 0, 1, 9, -1, -1, -1, -1, -1, -1, -1},
{1, 6, 2, 1, 8, 6, 1, 9, 8, 8, 7, 6, -1, -1, -1, -1},
{10, 7, 6, 10, 1, 7, 1, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 6, 1, 7, 10, 1, 8, 7, 1, 0, 8, -1, -1, -1, -1},
{0, 3, 7, 0, 7, 10, 0, 10, 9, 6, 10, 7, -1, -1, -1, -1},
{7, 6, 10, 7, 10, 8, 8, 10, 9, -1, -1, -1, -1, -1, -1, -1},
{6, 8, 4, 11, 8, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 3, 0, 6, 0, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 6, 11, 8, 4, 6, 9, 0, 1, -1, -1, -1, -1, -1, -1, -1},
{9, 4, 6, 9, 6, 3, 9, 3, 1, 11, 3, 6, -1, -1, -1, -1},
{6, 8, 4, 6, 11, 8, 2, 10, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 3, 0, 11, 0, 6, 11, 0, 4, 6, -1, -1, -1, -1},
{4, 11, 8, 4, 6, 11, 0, 2, 9, 2, 10, 9, -1, -1, -1, -1},
{10, 9, 3, 10, 3, 2, 9, 4, 3, 11, 3, 6, 4, 6, 3, -1},
{8, 2, 3, 8, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 2, 4, 6, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 9, 0, 2, 3, 4, 2, 4, 6, 4, 3, 8, -1, -1, -1, -1},
{1, 9, 4, 1, 4, 2, 2, 4, 6, -1, -1, -1, -1, -1, -1, -1},
{8, 1, 3, 8, 6, 1, 8, 4, 6, 6, 10, 1, -1, -1, -1, -1},
{10, 1, 0, 10, 0, 6, 6, 0, 4, -1, -1, -1, -1, -1, -1, -1},
{4, 6, 3, 4, 3, 8, 6, 10, 3, 0, 3, 9, 10, 9, 3, -1},
{10, 9, 4, 6, 10, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 5, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 5, 11, 7, 6, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 1, 5, 4, 0, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 6, 8, 3, 4, 3, 5, 4, 3, 1, 5, -1, -1, -1, -1},
{9, 5, 4, 10, 1, 2, 7, 6, 11, -1, -1, -1, -1, -1, -1, -1},
{6, 11, 7, 1, 2, 10, 0, 8, 3, 4, 9, 5, -1, -1, -1, -1},
{7, 6, 11, 5, 4, 10, 4, 2, 10, 4, 0, 2, -1, -1, -1, -1},
{3, 4, 8, 3, 5, 4, 3, 2, 5, 10, 5, 2, 11, 7, 6, -1},
{7, 2, 3, 7, 6, 2, 5, 4, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 5, 4, 0, 8, 6, 0, 6, 2, 6, 8, 7, -1, -1, -1, -1},
{3, 6, 2, 3, 7, 6, 1, 5, 0, 5, 4, 0, -1, -1, -1, -1},
{6, 2, 8, 6, 8, 7, 2, 1, 8, 4, 8, 5, 1, 5, 8, -1},
{9, 5, 4, 10, 1, 6, 1, 7, 6, 1, 3, 7, -1, -1, -1, -1},
{1, 6, 10, 1, 7, 6, 1, 0, 7, 8, 7, 0, 9, 5, 4, -1},
{4, 0, 10, 4, 10, 5, 0, 3, 10, 6, 10, 7, 3, 7, 10, -1},
{7, 6, 10, 7, 10, 8, 5, 4, 10, 4, 8, 10, -1, -1, -1, -1},
{6, 9, 5, 6, 11, 9, 11, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{3, 6, 11, 0, 6, 3, 0, 5, 6, 0, 9, 5, -1, -1, -1, -1},
{0, 11, 8, 0, 5, 11, 0, 1, 5, 5, 6, 11, -1, -1, -1, -1},
{6, 11, 3, 6, 3, 5, 5, 3, 1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 10, 9, 5, 11, 9, 11, 8, 11, 5, 6, -1, -1, -1, -1},
{0, 11, 3, 0, 6, 11, 0, 9, 6, 5, 6, 9, 1, 2, 10, -1},
{11, 8, 5, 11, 5, 6, 8, 0, 5, 10, 5, 2, 0, 2, 5, -1},
{6, 11, 3, 6, 3, 5, 2, 10, 3, 10, 5, 3, -1, -1, -1, -1},
{5, 8, 9, 5, 2, 8, 5, 6, 2, 3, 8, 2, -1, -1, -1, -1},
{9, 5, 6, 9, 6, 0, 0, 6, 2, -1, -1, -1, -1, -1, -1, -1},
{1, 5, 8, 1, 8, 0, 5, 6, 8, 3, 8, 2, 6, 2, 8, -1},
{1, 5, 6, 2, 1, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 6, 1, 6, 10, 3, 8, 6, 5, 6, 9, 8, 9, 6, -1},
{10, 1, 0, 10, 0, 6, 9, 5, 0, 5, 6, 0, -1, -1, -1, -1},
{0, 3, 8, 5, 6, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{10, 5, 6, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 7, 5, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{11, 5, 10, 11, 7, 5, 8, 3, 0, -1, -1, -1, -1, -1, -1, -1},
{5, 11, 7, 5, 10, 11, 1, 9, 0, -1, -1, -1, -1, -1, -1, -1},
{10, 7, 5, 10, 11, 7, 9, 8, 1, 8, 3, 1, -1, -1, -1, -1},
{11, 1, 2, 11, 7, 1, 7, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 1, 2, 7, 1, 7, 5, 7, 2, 11, -1, -1, -1, -1},
{9, 7, 5, 9, 2, 7, 9, 0, 2, 2, 11, 7, -1, -1, -1, -1},
{7, 5, 2, 7, 2, 11, 5, 9, 2, 3, 2, 8, 9, 8, 2, -1},
{2, 5, 10, 2, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{8, 2, 0, 8, 5, 2, 8, 7, 5, 10, 2, 5, -1, -1, -1, -1},
{9, 0, 1, 5, 10, 3, 5, 3, 7, 3, 10, 2, -1, -1, -1, -1},
{9, 8, 2, 9, 2, 1, 8, 7, 2, 10, 2, 5, 7, 5, 2, -1},
{1, 3, 5, 3, 7, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 7, 0, 7, 1, 1, 7, 5, -1, -1, -1, -1, -1, -1, -1},
{9, 0, 3, 9, 3, 5, 5, 3, 7, -1, -1, -1, -1, -1, -1, -1},
{9, 8, 7, 5, 9, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{5, 8, 4, 5, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{5, 0, 4, 5, 11, 0, 5, 10, 11, 11, 3, 0, -1, -1, -1, -1},
{0, 1, 9, 8, 4, 10, 8, 10, 11, 10, 4, 5, -1, -1, -1, -1},
{10, 11, 4, 10, 4, 5, 11, 3, 4, 9, 4, 1, 3, 1, 4, -1},
{2, 5, 1, 2, 8, 5, 2, 11, 8, 4, 5, 8, -1, -1, -1, -1},
{0, 4, 11, 0, 11, 3, 4, 5, 11, 2, 11, 1, 5, 1, 11, -1},
{0, 2, 5, 0, 5, 9, 2, 11, 5, 4, 5, 8, 11, 8, 5, -1},
{9, 4, 5, 2, 11, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 5, 10, 3, 5, 2, 3, 4, 5, 3, 8, 4, -1, -1, -1, -1},
{5, 10, 2, 5, 2, 4, 4, 2, 0, -1, -1, -1, -1, -1, -1, -1},
{3, 10, 2, 3, 5, 10, 3, 8, 5, 4, 5, 8, 0, 1, 9, -1},
{5, 10, 2, 5, 2, 4, 1, 9, 2, 9, 4, 2, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 3, 5, 1, -1, -1, -1, -1, -1, -1, -1},
{0, 4, 5, 1, 0, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{8, 4, 5, 8, 5, 3, 9, 0, 5, 0, 3, 5, -1, -1, -1, -1},
{9, 4, 5, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 11, 7, 4, 9, 11, 9, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{0, 8, 3, 4, 9, 7, 9, 11, 7, 9, 10, 11, -1, -1, -1, -1},
{1, 10, 11, 1, 11, 4, 1, 4, 0, 7, 4, 11, -1, -1, -1, -1},
{3, 1, 4, 3, 4, 8, 1, 10, 4, 7, 4, 11, 10, 11, 4, -1},
{4, 11, 7, 9, 11, 4, 9, 2, 11, 9, 1, 2, -1, -1, -1, -1},
{9, 7, 4, 9, 11, 7, 9, 1, 11, 2, 11, 1, 0, 8, 3, -1},
{11, 7, 4, 11, 4, 2, 2, 4, 0, -1, -1, -1, -1, -1, -1, -1},
{11, 7, 4, 11, 4, 2, 8, 3, 4, 3, 2, 4, -1, -1, -1, -1},
{2, 9, 10, 2, 7, 9, 2, 3, 7, 7, 4, 9, -1, -1, -1, -1},
{9, 10, 7, 9, 7, 4, 10, 2, 7, 8, 7, 0, 2, 0, 7, -1},
{3, 7, 10, 3, 10, 2, 7, 4, 10, 1, 10, 0, 4, 0, 10, -1},
{1, 10, 2, 8, 7, 4, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 7, 1, 3, -1, -1, -1, -1, -1, -1, -1},
{4, 9, 1, 4, 1, 7, 0, 8, 1, 8, 7, 1, -1, -1, -1, -1},
{4, 0, 3, 7, 4, 3, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{4, 8, 7, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 8, 10, 11, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 11, 9, 10, -1, -1, -1, -1, -1, -1, -1},
{0, 1, 10, 0, 10, 8, 8, 10, 11, -1, -1, -1, -1, -1, -1, -1},
{3, 1, 10, 11, 3, 10, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 2, 11, 1, 11, 9, 9, 11, 8, -1, -1, -1, -1, -1, -1, -1},
{3, 0, 9, 3, 9, 11, 1, 2, 9, 2, 11, 9, -1, -1, -1, -1},
{0, 2, 11, 8, 0, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{3, 2, 11, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 10, 8, 9, -1, -1, -1, -1, -1, -1, -1},
{9, 10, 2, 0, 9, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{2, 3, 8, 2, 8, 10, 0, 1, 8, 1, 10, 8, -1, -1, -1, -1},
{1, 10, 2, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{1, 3, 8, 9, 1, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 9, 1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{0, 3, 8, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1},
{-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}};

   /*
      Determine the index into the edge table which
      tells us which vertices are inside of the surface
   */
   cubeindex = 0;
   if (grid.val[0] < isolevel) cubeindex |= 1;
   if (grid.val[1] < isolevel) cubeindex |= 2;
   if (grid.val[2] < isolevel) cubeindex |= 4;
   if (grid.val[3] < isolevel) cubeindex |= 8;
   if (grid.val[4] < isolevel) cubeindex |= 16;
   if (grid.val[5] < isolevel) cubeindex |= 32;
   if (grid.val[6] < isolevel) cubeindex |= 64;
   if (grid.val[7] < isolevel) cubeindex |= 128;

   /* Cube is entirely in/out of the surface */
   if (edgeTable[cubeindex] == 0)
      return(0);

   /* Find the vertices where the surface intersects the cube */
   if (edgeTable[cubeindex] & 1)
      vertlist[0] =
         VertexInterp(isolevel,grid.p[0],grid.p[1],grid.val[0],grid.val[1]);
   if (edgeTable[cubeindex] & 2)
      vertlist[1] =
         VertexInterp(isolevel,grid.p[1],grid.p[2],grid.val[1],grid.val[2]);
   if (edgeTable[cubeindex] & 4)
      vertlist[2] =
         VertexInterp(isolevel,grid.p[2],grid.p[3],grid.val[2],grid.val[3]);
   if (edgeTable[cubeindex] & 8)
      vertlist[3] =
         VertexInterp(isolevel,grid.p[3],grid.p[0],grid.val[3],grid.val[0]);
   if (edgeTable[cubeindex] & 16)
      vertlist[4] =
         VertexInterp(isolevel,grid.p[4],grid.p[5],grid.val[4],grid.val[5]);
   if (edgeTable[cubeindex] & 32)
      vertlist[5] =
         VertexInterp(isolevel,grid.p[5],grid.p[6],grid.val[5],grid.val[6]);
   if (edgeTable[cubeindex] & 64)
      vertlist[6] =
         VertexInterp(isolevel,grid.p[6],grid.p[7],grid.val[6],grid.val[7]);
   if (edgeTable[cubeindex] & 128)
      vertlist[7] =
         VertexInterp(isolevel,grid.p[7],grid.p[4],grid.val[7],grid.val[4]);
   if (edgeTable[cubeindex] & 256)
      vertlist[8] =
         VertexInterp(isolevel,grid.p[0],grid.p[4],grid.val[0],grid.val[4]);
   if (edgeTable[cubeindex] & 512)
      vertlist[9] =
         VertexInterp(isolevel,grid.p[1],grid.p[5],grid.val[1],grid.val[5]);
   if (edgeTable[cubeindex] & 1024)
      vertlist[10] =
         VertexInterp(isolevel,grid.p[2],grid.p[6],grid.val[2],grid.val[6]);
   if (edgeTable[cubeindex] & 2048)
      vertlist[11] =
         VertexInterp(isolevel,grid.p[3],grid.p[7],grid.val[3],grid.val[7]);

   /* Create the triangle */
   ntriang = 0;
   for (i=0;triTable[cubeindex][i]!=-1;i+=3) {
      triangles[ntriang].p[0] = vertlist[triTable[cubeindex][i  ]];
      triangles[ntriang].p[1] = vertlist[triTable[cubeindex][i+1]];
      triangles[ntriang].p[2] = vertlist[triTable[cubeindex][i+2]];
      ntriang++;
   }

   return(ntriang);
}

/*
   Linearly interpolate the position where an isosurface cuts
   an edge between two vertices, each with their own scalar value
*/
XYZ VertexInterp(double isolevel,XYZ p1, XYZ p2, double valp1, double valp2)
{
   double mu;
   XYZ p;

   if (abs(isolevel-valp1) < 0.00001)
      return(p1);
   if (abs(isolevel-valp2) < 0.00001)
      return(p2);
   if (abs(valp1-valp2) < 0.00001)
      return(p1);
   mu = (isolevel - valp1) / (valp2 - valp1);
   p.x = p1.x + mu * (p2.x - p1.x);
   p.y = p1.y + mu * (p2.y - p1.y);
   p.z = p1.z + mu * (p2.z - p1.z);

   return(p);
}

GRIDCELL getcell(int x, int y, int z, int n, double *data)
{
	GRIDCELL result;
	result.p[0].x=x; result.p[0].y=y; result.p[0].z=z+1;
	result.p[1].x=x+1; result.p[1].y=y; result.p[1].z=z+1;
	result.p[2].x=x+1; result.p[2].y=y+1; result.p[2].z=z+1;
	result.p[3].x=x; result.p[3].y=y+1; result.p[3].z=z+1;
	result.p[4].x=x; result.p[4].y=y; result.p[4].z=z;
	result.p[5].x=x+1; result.p[5].y=y; result.p[5].z=z;
	result.p[6].x=x+1; result.p[6].y=y+1; result.p[6].z=z;
	result.p[7].x=x; result.p[7].y=y+1; result.p[7].z=z;
	result.val[0]=data[x+y*n+(z+1)*n*n];
	result.val[1]=data[x+1+y*n+(z+1)*n*n];
	result.val[2]=data[x+1+(y+1)*n+(z+1)*n*n];
	result.val[3]=data[x+(y+1)*n+(z+1)*n*n];
	result.val[4]=data[x+y*n+(z)*n*n];
	result.val[5]=data[x+1+y*n+(z)*n*n];
	result.val[6]=data[x+1+(y+1)*n+(z)*n*n];
	result.val[7]=data[x+(y+1)*n+(z)*n*n];
	return result;
}

static point *pointlist;                 
static triangle *trianglelist;
static TRIANGLE list[6];
static double *gradient;
static segment *segmentlist;

int findpoint(GLfloat x,GLfloat y,GLfloat z)
{
	int result=-1;
	for(int i=0; i<numpoints; ++i)
	{
		if(abs(pointlist[i].x-x)<0.00001 && abs(pointlist[i].y-y)<0.00001 && abs(pointlist[i].z-z)<0.00001)
		{
			result=i;
			break;
		}
	}
	return result;
}


void updatelist(GLfloat x1, GLfloat y1, GLfloat z1, GLfloat x2, GLfloat y2, GLfloat z2, GLfloat x3, GLfloat y3, GLfloat z3)
{
	int i=findpoint(x1, y1, z1);
	if(i==-1)
	{
		pointlist[numpoints].x=x1;
		pointlist[numpoints].y=y1;
		pointlist[numpoints].z=z1;
		pointlist[numpoints].nx=pointlist[numpoints].ny=pointlist[numpoints].nz=0;
		trianglelist[numtriangle].a=numpoints;
		++numpoints;
	}
	else
	{
		trianglelist[numtriangle].a=i;
	}
	
	i=findpoint(x2, y2, z2);
	if(i==-1)
	{
		pointlist[numpoints].x=x2;
		pointlist[numpoints].y=y2;
		pointlist[numpoints].z=z2;
		pointlist[numpoints].nx=pointlist[numpoints].ny=pointlist[numpoints].nz=0;
		trianglelist[numtriangle].b=numpoints;
		++numpoints;
	}
	else
	{
		trianglelist[numtriangle].b=i;
	}
	
	i=findpoint(x3, y3, z3);
	if(i==-1)
	{
		pointlist[numpoints].x=x3;
		pointlist[numpoints].y=y3;
		pointlist[numpoints].z=z3;
		pointlist[numpoints].nx=pointlist[numpoints].ny=pointlist[numpoints].nz=0;
		trianglelist[numtriangle].c=numpoints;
		++numpoints;
	}
	else
	{
		trianglelist[numtriangle].c=i;
	}
	++numtriangle;
}



void picture()
{
	int start_x=0, start_y=0, w=700, h=700;
        FILE *f=fopen(fileName,"wb");
        if (!f) return;
        std::vector<unsigned char> out(3*w*h);
        glPixelStorei(GL_PACK_ALIGNMENT,1); /* byte aligned output */
        glReadPixels(start_x,start_y,w,h, GL_RGB,GL_UNSIGNED_BYTE,&out[0]);
        fprintf(f,"P6\n%d %d\n255\n",w,h);
        for (int y=0;y<h;y++) { /* flip image bottom-to-top on output */
                fwrite(&out[3*(h-1-y)*w],1,3*w,f);
        }
        fclose(f);
}

/*  GL initialization */
void init()
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glEnable(GL_LIGHTING);	
	GLfloat mat_specular[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat mat_shininess[] = { shine };
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	
	//set material property
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, mat_shininess);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, red);
	//glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cyan);	
	

  //glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE ) ;
  //glEnable ( GL_COLOR_MATERIAL ) ;
//glColor4f(0.0f, 0.0f, 1.0f, 0.01);
	

	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
   

}

/* The display callback */
void draw_stuff()
{
	GLfloat x1, x2, x3, y1, y2, y3, z1, z2, z3, a, b, c,l;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	

		
	//set light   
	GLfloat light_position[] = { -1*cos(lj/360.0*2*M_PI)*cos(li/360.0*2*M_PI), -1*sin(lj/360.0*2*M_PI), -1*sin(li/360.0*2*M_PI), 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, red);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);	
    
	
	
	//set direction of camera
	glLoadIdentity();                  
	glTranslatef(xshift, yshift, zoom);  
 	glRotatef((GLfloat)xrot,1.0,0.0,0.0);
	glRotatef((GLfloat)yrot,0.0,1.0,0.0);
	glRotatef((GLfloat)zrot,0.0,0.0,1.0);
	
	//plot triangles and normals
	glBegin(GL_TRIANGLES);  
    for(int i=0; i<numtriangle; ++i)
	{

		x1= pointlist[trianglelist[i].a].x/scale;
		x2=pointlist[trianglelist[i].b].x/scale;
		x3=pointlist[trianglelist[i].c].x/scale;
		y1=pointlist[trianglelist[i].a].y/scale;
		y2=pointlist[trianglelist[i].b].y/scale;
		y3=pointlist[trianglelist[i].c].y/scale;
		z1=pointlist[trianglelist[i].a].z/scale;
		z2=pointlist[trianglelist[i].b].z/scale;
		z3=pointlist[trianglelist[i].c].z/scale;

		a=pointlist[trianglelist[i].a].nx;
		b=pointlist[trianglelist[i].a].ny;
		c=pointlist[trianglelist[i].a].nz;
		l=sqrt(a*a+b*b+c*c);
		glNormal3f(a/l, b/l, c/l);		
		glVertex3f( x1, y1, z1);
		a=pointlist[trianglelist[i].b].nx;
		b=pointlist[trianglelist[i].b].ny;
		c=pointlist[trianglelist[i].b].nz;
		l=sqrt(a*a+b*b+c*c);
		glNormal3f(a/l, b/l, c/l);		
		glVertex3f( x2, y2, z2);
		a=pointlist[trianglelist[i].c].nx;
		b=pointlist[trianglelist[i].c].ny;
		c=pointlist[trianglelist[i].c].nz;
		l=sqrt(a*a+b*b+c*c);
		glNormal3f(a/l, b/l, c/l);		
		glVertex3f( x3, y3, z3);
	}
	glEnd();  
	 
	glLineWidth(1.0);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, white);
	glBegin(GL_LINES);
	for(int i=0; i<numsegment; ++i)
	{
		
		glVertex3f( segmentlist[i].x1/scale, segmentlist[i].y1/scale, segmentlist[i].z1/scale);
		glVertex3f( segmentlist[i].x2/scale, segmentlist[i].y2/scale, segmentlist[i].z2/scale);
	}
	glEnd();
	
	glutSwapBuffers();  
}

/* the window-reshape callback */
void reshape(int w, int h)
{
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0,(GLfloat)w/(GLfloat)h,1.0,20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0,0.0,-5.0);
}

/* The basic keyboard controls, callback for glutKeyboardFunc() */
void kb(unsigned char key, int x, int y)
{
	switch(key)
	{

		case 'x':
			xshift+=0.25;
			glutPostRedisplay();
			break;
		case 'X':
			xshift-=0.25;
			glutPostRedisplay();
			break;
		case 'y':
			yshift+=0.25;
			glutPostRedisplay();
			break;
		case 'Y':
			yshift-=0.25;
			glutPostRedisplay();
			break;
		case 'r':
		case 'R':
			xrot=yrot=zrot=0;
			xshift=-1.0;
			yshift=0.0;
			zoom=-6.0;
			glutPostRedisplay();
			break;
		case 'z':
			zrot=(zrot-1)%360;
			glutPostRedisplay();
			break;
		case 'Z':
			zrot=(zrot+1)%360;
			glutPostRedisplay();
			break;
		case 's':
			scale+=5.0;
			glutPostRedisplay();
			break;
		case 'S':
			scale-=5.0;
			glutPostRedisplay();
			break;
		case ESC:
			exit(0);
			break;
		case 'q':
			xshift+=0.025;
			glutPostRedisplay();
			break;
		case 'Q':
			xshift-=0.025;
			glutPostRedisplay();
			break;
		case 'w':
			yshift+=0.025;
			glutPostRedisplay();
			break;
		case 'W':
			yshift-=0.025;
			glutPostRedisplay();
			break;			
		case 'e':
			zoom+=0.05;
			glutPostRedisplay();
			break;
		case 'E':
			zoom-=0.05;
			glutPostRedisplay();
			break;
		case 'l':
			li=(li-1)%360;
			glutPostRedisplay();
			break;
		case 'L':
			li=(li+1)%360;
			glutPostRedisplay();
			break;		
			
		case 'o':
			lj=(lj-1)%360;
			glutPostRedisplay();
			break;
		case 'O':
			lj=(lj+1)%360;
			glutPostRedisplay();
			break;	
		case 'P':
			picture();
			//WindowDump();
			break;	
		case 'M':
			cout<<'\n'<<"scale li lj zoom xshift yshift xrot yrot zrot"<<scale<<' '<<li<<' '<<lj<<' '<<zoom<<' '<<xshift<<' '<<yshift<<' '<<xrot<<' '<<yrot<<' '<<zrot<<'\n';
			break;					
		default:
			break;
	}
}

/* The "special" keyboard controls, callback for glutSpecialFunc() */
void skb(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			zoom+=0.5;
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			zoom-=0.5;
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			yrot=(yrot-1)%360;
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			yrot=(yrot+1)%360;
			glutPostRedisplay();
			break;
		case GLUT_KEY_PAGE_UP:
			xrot=(xrot-1)%360;
			glutPostRedisplay();
			break;
		case GLUT_KEY_PAGE_DOWN:
			xrot=(xrot+1)%360;
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

int main(int argc, char **argv)
{
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(700,700);
	glutInitWindowPosition(100,50);
	GLfloat x1, x2, x3, y1, y2, y3, z1, z2, z3, a, b, c,l;
	//get data

	
	char filename[20]="proteinout.b";
	FILE *fpt;
	fpt=fopen(filename, "r");
	if(!fpt)
		return false;
	gradient=new double[n*n*n];
	for(int i=0; i<=num; ++i)
		fread(gradient, sizeof(double), n*n*n, fpt);
	
	fclose(fpt);
	
	//processing
	pointlist= new point[100000];
	trianglelist=new triangle[100000];
	segmentlist=new segment[10000];
	
	for(int i=0; i<n-1; ++i)
	{
		for(int j=0; j<n-1; ++j)
		{
			for(int k=0; k<n-1; ++k)
			{
				int numt=Polygonise(getcell(i, j, k, n, gradient), 0, list);
				for(int q=0; q<numt; ++q)
				{
					y1=list[q].p[0].x-32;
					y2=list[q].p[1].x-32;
					y3=list[q].p[2].x-32;
					z1=list[q].p[0].y-32;
					z2=list[q].p[1].y-32;
					z3=list[q].p[2].y-32;
					x1=list[q].p[0].z-32;
					x2=list[q].p[1].z-32;
					x3=list[q].p[2].z-32;
					if((!(x1>=0 && y1>=0 && x2>=0 && y2>=0 && x3>=0 && y3>=0)) || full)
						updatelist(x1, y1, z1, x2, y2, z2, x3, y3, z3);
					else
					{
						if(abs(x1)<0.00001 && abs(x2)<0.00001)
						{
							segmentlist[numsegment].x1=x1;
							segmentlist[numsegment].x2=x2;
							segmentlist[numsegment].y1=y1;
							segmentlist[numsegment].y2=y2;
							segmentlist[numsegment].z1=z1;
							segmentlist[numsegment].z2=z2;
							++numsegment;
						}
						if(abs(y1)<0.00001 && abs(y2)<0.00001)
						{
							segmentlist[numsegment].x1=x1;
							segmentlist[numsegment].x2=x2;
							segmentlist[numsegment].y1=y1;
							segmentlist[numsegment].y2=y2;
							segmentlist[numsegment].z1=z1;
							segmentlist[numsegment].z2=z2;
							++numsegment;
						}
						if(abs(x1)<0.00001 && abs(x3)<0.00001)
						{
							segmentlist[numsegment].x1=x1;
							segmentlist[numsegment].x2=x3;
							segmentlist[numsegment].y1=y1;
							segmentlist[numsegment].y2=y3;
							segmentlist[numsegment].z1=z1;
							segmentlist[numsegment].z2=z3;
							++numsegment;
						}
						if(abs(y1)<0.00001 && abs(y3)<0.00001)
						{
							segmentlist[numsegment].x1=x1;
							segmentlist[numsegment].x2=x3;
							segmentlist[numsegment].y1=y1;
							segmentlist[numsegment].y2=y3;
							segmentlist[numsegment].z1=z1;
							segmentlist[numsegment].z2=z3;
							++numsegment;
						}
						if(abs(x2)<0.00001 && abs(x3)<0.00001)
						{
							segmentlist[numsegment].x1=x2;
							segmentlist[numsegment].x2=x3;
							segmentlist[numsegment].y1=y2;
							segmentlist[numsegment].y2=y3;
							segmentlist[numsegment].z1=z2;
							segmentlist[numsegment].z2=z3;
							++numsegment;
						}
						if(abs(y2)<0.00001 && abs(y3)<0.00001)
						{
							segmentlist[numsegment].x1=x2;
							segmentlist[numsegment].x2=x3;
							segmentlist[numsegment].y1=y2;
							segmentlist[numsegment].y2=y3;
							segmentlist[numsegment].z1=z2;
							segmentlist[numsegment].z2=z3;
							++numsegment;
						}														
					}
				}
			}
		}
	}	
	for(int i=0; i<numpoints; ++i)
	{
		cout<<pointlist[i].x<<' '<<pointlist[i].y<<' '<<pointlist[i].z<<'\n';
	}
	for(int i=0; i<numtriangle; ++i)
	{
		cout<<trianglelist[i].a<<' '<<trianglelist[i].b<<' '<<trianglelist[i].c<<'\n';
	}	
	//calculate normal
	for(int i=0; i<numtriangle; ++i)
	{
		x1= pointlist[trianglelist[i].a].x;
		x2=pointlist[trianglelist[i].b].x;
		x3=pointlist[trianglelist[i].c].x;
		y1=pointlist[trianglelist[i].a].y;
		y2=pointlist[trianglelist[i].b].y;
		y3=pointlist[trianglelist[i].c].y;
		z1=pointlist[trianglelist[i].a].z;
		z2=pointlist[trianglelist[i].b].z;
		z3=pointlist[trianglelist[i].c].z;
		a=(y2-y1)*(z3-z1)-(y3-y1)*(z2-z1);
		b=(z2-z1)*(x3-x1)-(x2-x1)*(z3-z1);
		c=(x2-x1)*(y3-y1)-(x3-x1)*(y2-y1);
		l=sqrt(a*a+b*b+c*c);
		pointlist[trianglelist[i].a].nx+=a/l;
		pointlist[trianglelist[i].b].nx+=a/l;
		pointlist[trianglelist[i].c].nx+=a/l;

		
		pointlist[trianglelist[i].a].ny+=b/l;
		pointlist[trianglelist[i].b].ny+=b/l;
		pointlist[trianglelist[i].c].ny+=b/l;

		
		
		pointlist[trianglelist[i].a].nz+=c/l;
		pointlist[trianglelist[i].b].nz+=c/l;		
		pointlist[trianglelist[i].c].nz+=c/l;
			
							
	}	
	
	glutInit(&argc,argv);

	glutCreateWindow("bubble");								//name of window	
	
			
	init();
	
	glutDisplayFunc(draw_stuff);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(kb);
	glutSpecialFunc(skb);
	glutMainLoop(); 	
	delete [] pointlist;
	delete [] trianglelist;
	delete [] gradient;
	return 0;
}
