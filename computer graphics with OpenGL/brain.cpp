/*  brain.cpp*/
/*  programmed by Chaolun Wang */
/*
	keyboard controls:	
	x/X:	Positive/negative X-axis shift of the model
	y/Y:	Positive/negative Y-axis shift of the model
	s/S:    decrease/increase the scale of model
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

#define ESC 27
#define BUFFERSIZE 100


using namespace std;

static GLfloat red[]={1,0,0}; 
static int xrot=0,yrot=0,zrot=0;
static GLfloat xshift=-1.0,yshift=0.0,zoom=-6.0;
static int numpoints=0, numtriangle=0;
static GLfloat scale=100.0;

struct point{                          //datasturcture to store point
	GLfloat x;
	GLfloat y;
	GLfloat z;
};

struct triangle{                        //datastructure to store triangle
	int a;
	int b;
	int c;
};

static point *pointlist;                 
static triangle *trianglelist;

/*  GL initialization */
void init()
{
	
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 90.0 };
   
	glClearColor (0.0, 0.0, 0.0, 0.0);
	//set material property
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);


	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
   

}

/* The display callback */
void draw_stuff()
{
	GLfloat x1, x2, x3, y1, y2, y3, z1, z2, z3, a, b, c,l;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	//set light   
	GLfloat light_position[] = { -1*xshift, -1*yshift, -1*zoom, 0.0 };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, red);
	
	
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
		glColor3fv(red);
		x1= pointlist[trianglelist[i].a].x/scale;
		x2=pointlist[trianglelist[i].b].x/scale;
		x3=pointlist[trianglelist[i].c].x/scale;
		y1=pointlist[trianglelist[i].a].y/scale;
		y2=pointlist[trianglelist[i].b].y/scale;
		y3=pointlist[trianglelist[i].c].y/scale;
		z1=pointlist[trianglelist[i].a].z/scale;
		z2=pointlist[trianglelist[i].b].z/scale;
		z3=pointlist[trianglelist[i].c].z/scale;
		a=(y2-y1)*(z3-z1)-(y3-y1)*(z2-z1);
		b=(z2-z1)*(x3-x1)-(x2-x1)*(z3-z1);
		c=(x2-x1)*(y3-y1)-(x3-x1)*(y2-y1);
		l=sqrt(a*a+b*b+c*c);
		glNormal3f(a/l, b/l, c/l);		
		glVertex3f( x1, y1, z1);		
		glVertex3f( x2, y2, z2);		
		glVertex3f( x3, y3, z3);
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
	
	//get data

	char buffer[BUFFERSIZE];
	fgets(buffer, BUFFERSIZE, stdin);
	fgets(buffer, BUFFERSIZE, stdin);
	glutInit(&argc,argv);

	glutCreateWindow(buffer);								//name of window
	
	fgets(buffer, BUFFERSIZE, stdin);
	fgets(buffer, BUFFERSIZE, stdin);	
	fgets(buffer, BUFFERSIZE, stdin);

	sscanf(buffer, "%*s %d", &numpoints);
	pointlist= new point[numpoints];
	for(int i=0; i<numpoints; ++i)
	{
		fgets(buffer, BUFFERSIZE, stdin);
		sscanf(buffer, "%f %f %f", &pointlist[i].x, &pointlist[i].y, &pointlist[i].z);
	}
	fgets(buffer, BUFFERSIZE, stdin);
	sscanf(buffer, "%*s %d", &numtriangle);

	trianglelist=new triangle[numtriangle];
	for(int i=0; i<numtriangle; ++i)
	{
		if(fgets(buffer, BUFFERSIZE, stdin)!=NULL)
			sscanf(buffer, "%*d %d %d %d", &trianglelist[i].a, &trianglelist[i].b, &trianglelist[i].c);
		else
		{
			numtriangle=i;
			break;
		}
	}

			
	init();
	
	glutDisplayFunc(draw_stuff);
	glutReshapeFunc(reshape);

	glutKeyboardFunc(kb);
	glutSpecialFunc(skb);
	glutMainLoop(); 	
	delete [] pointlist;
	delete [] trianglelist;
	return 0;
}
