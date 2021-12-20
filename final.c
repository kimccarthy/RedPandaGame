/*
 *  Used perspective view from example 9
 */
#include "CSCIx229.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>
#ifdef USEGLEW
#include <GL/glew.h>
#include <stdbool.h>
#endif
//  OpenGL with prototypes for glext
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
// Tell Xcode IDE to not gripe about OpenGL deprecation
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#else
#include <GL/glut.h>
#endif
//  Default resolution
//  For Retina displays compile with -DRES=2
#ifndef RES
#define RES 1
#endif
float objX = 0.6; //panda x
float objZ = -14.8; //panda z
float cameraX = 0; //camera x
float cameraZ = -5; //cameraz
float yaw = 0;
int axes=0;       //  Display axes
int mode=1;       //  Projection mode
int th=30;         //  Azimuth of view angle
int ph=30;         //  Elevation of view angle
int fov=55;       //  Field of view (for perspective)
double asp=1;     //  Aspect ratio
double dim=50;   //  Size of world
double distance = 40; //lights distance
int zh = 90; //light angle
double ylight = 10; //light height
int local = 0;
int obj = 0; //object mode
float ambient = 10; //to change ambient lighting
float spec = 10; //specular controls
int move = 1;
unsigned int texture[13]; //textures
float jsum = -2; //floor height, base of the jump
float jinc = 0.2; //jump speed
float speed = 1; //panda speed
float feets = 0; //feet movement y/n
int walkState = 0; //which direction are the feet moving
int walking = 0; //is the panda walking
int jumpState = 0; //is the panda jumping
int jumpEnd = 0; //is the panda falling
int lightDown = 0; //getting darker or lighter currently?
int current= 0;
int facing = 0; //which direction is the panda facing
int minutes = 0;
float seconds = 0;
int finished = 0;
typedef struct{
    float x;
    float y;
    float z;
    float xstretch;
    float ystretch;
    float zstretch;
    float color;
}platform;
#define MAX 37
platform p[MAX]={ {-30, 20, 30, 10, 0.5, 10, 1},//0
                  {5,4,-20, 5, 0.5, 5, 0.5},//1
                  {5,5,-30, 5, 0.5, 5, 0.8},//2
                  {5,6,-40, 5, 0.5, 5, 1},//3
                  {15,7,-40, 5, 0.5, 5, 0.7},//4
                  {25,8,-40, 5, 0.5, 5, 1},//5
                  {35,9,-40, 5, 0.5, 5, 0.5},//6
                  {35,10,-30, 5, 0.5, 5, 1},//7
                  {35,10,-20, 5, 0.5, 5, 0.7},//8
                  {35,10,-10, 5, 0.5, 5, 0.4},//9
                  {20,10,-10, 10, 0.5, 5, 1},//10
                  {20,10,0, 5, 0.5, 15, 1},//11
                  {20,10,17, 5, 0.5, 9, 0.6},//12
                  {5,13,20, 3, 0.5, 7, 1},//13
                  {10,14,22, 3, 0.5, 7, 0.5},//14
                  {5,15,24, 3, 0.5, 7, 1},//15
                  {0,16,26, 3, 0.5, 7, 0.6},//16
                  {-15, 7, 26, 20, 1, 8, 1},//17
                  {-20,9,15, 5, 0.5, 5, 0.7},//18
                  {-25,11,10, 5, 0.5, 5, 0.5},//19
                  {-35,13,10, 5, 0.5, 5, 0.9},//20
                  {-45,15,15, 5, 0.5, 5, 0.7},//21
                  {-45,17,25, 5, 0.5, 5, 0.5},//22
                  {-35,19,35, 5, 0.5, 5, 1},//23
                  {-45,17,35, 5, 0.5, 5, 1},//24
                  {-40,-2,-30,4,0.3,4, 0},//*****
                  {-30,0.5,-25,5,0.3,7, 0.7},
                  {-30,1,-10,5,0.3,10, 0.5},
                  {-23,-1,-5,7,0.3,10, 1},
                  {-17, 0.5, 0, 3, 0.4, 7, 0.8},
                  {-13, 1, 0, 3, 0.4, 7, 0.5},
                  {-9, 1.5, 0, 3, 0.4, 7, 1},
                  {-5, 2.5, 0, 3, 0.4, 7, 0.6},
                  {6, 4, 0, 5, 0.4, 14, 1},
                  {0, 4, 0, 2, 0.4, 6, 0.5},
                  {2, 4, 0, 2, 0.4, 6, 0.8},
                  {4, 4, 0, 2, 0.4, 6, 0.3}
    
};
#define MAX_IT 85
typedef struct{
    float x;
    float y;
    float z;
    float scale;
    float rot;
}item;

item items[MAX_IT] = {
    //ROCKS
    {-50, -2.2, -14, 1, 30},
    {-45, -2.5, -12, 0.6, 20},
    {-40, -2.5, -11, 0.6, 90},
    {-37, -2.5, -10, 0.9, 120},
    {-42, -2.2, -12, 0.5, 30},
    {-46, -2.2, -10, 0.5, 30},
    {-32, -2.2, -7, 0.7, 30},
    {-41, -2.5, -10, 1.2, 30},
    {-42, -2.2, -7, 1.5, 20},
    {-35, -2.2, -10, 1, 90},
    {-50, -2.2, 4, 1, 60},
    {-47, -2.2, 7, 0.6, 20},
    {-49, -2.2, 7, 0.4, 20},
    {-35, -2.2, 10, 1, 0},
    {-30, -2, 10, 0.5, 0},
    {-20, -2.2, 20, 1, 45},
    {-20, -2.5, 25, 1.3, 0},
    {-27, -2.2, 13, 1.5, -40},
    {-35, -2.2, 10, 1, -45},
    {-30, -2, 14, 0.5, 0},
    {-34, -2, 15, 0.7, 0},
    {-36, -2, 12, 0.3, 20},
    {-28, -2, 16, 0.7, 0},
    {-25, -2, 18, 0.4, 0},
    {-5, -2, 48, 0.5, 0},
    {-23, -2, 22, 0.4, 0},
    {-35, -2.2, -10, 1, -30},
    {-20, -2.2, 4, 1, 180},
    {-10, -2.2, 40, 1, 50},
    {-10, -2.2, 35, 1, 0},
    {-7, -2.2, 48, 0.8, 90},
    {-8, -2, 38, 0.5, 0},
    {-11, -2, 37, 0.5, 0},
    {-14, -2, 43, 0.6, 0},
    {-20, -2, 47, 0.3, 0},
    {-5, -2, 47, 0.7, 0},
    {-10, -2.2, 25, 1, 140},
    {-0, -2.2, 40, 1, 293},
    {-10, -2.2, 40, 1, 139},
    {-17, -2.2, 45, 1, 149},
    {-12, -2.2, 49, 0.5, 10},
    {-0, -2.2, 40, 1, 0},
    {10, -2.2, 32, 0.5, 30},
    {-10, -2.2, 40, 0.5, 30},
    {-7, -2.2, 20, 0.8, 30},
    {-20, -2.2, 30, 0.8, 30},
    {10, -2, 10, 0.5, 30},//END OF ROCKS: COUNT = 47
    //FLOWERS
    {5, -2, 15, 0.2, 0},
    {15, -1.5, 5, 0.2, 90},
    {30, -1.5, 15, 0.2, 30},
    {25, -1.5, -35, 0.3, 70},
    {-5, -2, 15, 0.2, 0},
    {-15, -1.5, -5, 0.2, 90},
    {-40, -1.5, 15, 0.2, 30},
    {-25, -1.5, 35, 0.3, 70},
    {-35, -1.5, 3, 0.1, 35},
    {-15, -1.5, -10, 0.3, 20},
    {42, -1.5, 15, 0.15, 170},
    {-25, -1.5, -35, 0.2, 70},
    {32, -1.5, -32, 0.1, 320},
    {-40, -1.5, 35, 0.2, 70},
    {1, -1.5, 25, 0.1, -20},
    {26, -1.5, -35, 0.2, 70},
    {-5, -1.5, 35, 0.1, 50},
    {-30, -1.5, 35, 0.2, 80},
    {-40, -1.5, 15, 0.1, 70},
    {-26, -1.5, 25, 0.2, 10},
    {-16, -1.5, 41, 0.1, 140},
    {-30, -1.5, 20, 0.2, 0},
    {-45, -1.5, 21, 0.15, 20},
    {-32, -1.5, 30, 0.2, 190},
    {-45, -1.5, 45, 0.1, 70},
    {-30, -1.5, 45, 0.2, 270}, //END OF FLOWERS: COUNT = 26
    
    //TREES
    {10,0,5,3,30},
    {0,0,5,3,13},
    {15,0,-5,3,140},
    {-15,1,-18,4,40},
    {-20,0,-35,3,29},
    {-5,2,-30,5,20},
    {30,2,30,5,5},
    {10,0,35,3,330},
    {35,1,5,5,52},
    {-35,0,-20,3,14},
    {-35,0,25,3.5,52},
    {20, 7, -25, 7, 30} //END OF TREES: COUNT = 12
    
};


//  Macro for sin & cos in degrees
#define Cos(th) cos(3.14159265/180*(th))
#define Sin(th) sin(3.14159265/180*(th))

/*
 *  Convenience routine to output raster text
 *  Use VARARGS to make this more flexible
 */
#define LEN 8192  //  Maximum length of text string
void Print(const char* format , ...)
{
   char    buf[LEN];
   char*   ch=buf;
   va_list args;
   //  Turn the parameters into a character string
   va_start(args,format);
   vsnprintf(buf,LEN,format,args);
   va_end(args);
   //  Display the characters one at a time at the current raster position
   while (*ch)
      glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18,*ch++);
}

/*
 *  Check for OpenGL errors
 */
void ErrCheck(const char* where)
{
   int err = glGetError();
   if (err) fprintf(stderr,"ERROR: %s [%s]\n",gluErrorString(err),where);
}

/*
 *  Print message to stderr and exit
 */
void Fatal(const char* format , ...)
{
   va_list args;
   va_start(args,format);
   vfprintf(stderr,format,args);
   va_end(args);
   exit(1);
}

/*
 *  Set projection
 */
static void Project()
{
   //  Tell OpenGL we want to manipulate the projection matrix
   glMatrixMode(GL_PROJECTION);
   //  Undo previous transformations
   glLoadIdentity();
   //  Perspective transformation
   if (mode)
      gluPerspective(fov,asp,dim/4,4*dim);
   //  Orthogonal projection
   else
      glOrtho(-asp*dim,+asp*dim, -dim,+dim, -dim,+dim);
   //  Switch to manipulating the model matrix
   glMatrixMode(GL_MODELVIEW);
   //  Undo previous transformations
   glLoadIdentity();
}

/*
 *  Draw a cube
 *     at (x,y,z)
 *     dimensions (dx,dy,dz)
 *     rotated th about the y axis
 */
//HERERERERERERE
static void Vertex(double th,double ph)
{
   //glColor3f(Cos(th)*Cos(th) , Sin(ph)*Sin(ph) , Sin(th)*Sin(th));
    double x = Sin(th)*Cos(ph);
    double y = Cos(th)*Cos(ph);
    double z = Sin(ph);
 
    glNormal3d(x,y,z);
    glVertex3d(x,y,z);
}

/*
 *  Draw a sphere (version 1)
 *     at (x,y,z)
 *     radius (r)
 */

static void rectangle(double x, double y, double z, double x1, double x2, double x3, double r1, double r2, double r3, double r4){
    glPushMatrix();
    glTranslated(x, y, z);
    glRotated(r1, r2, r3, r4);
    glScaled(x1, x2, x3);
    
    //float white[] = {1,1,1,1};
    //float shiny[] = {1,1,1,1};
    //float Emission[] = {0.0, 0.0, 0.01, 1.0};
    //float shiny = 1;
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, 1);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    //glMaterialfv(GL_FRONT, GL_EMISSION, Emission);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[0]);
    //front
    glBegin(GL_QUADS);
    glNormal3d(0,0,1);
    glTexCoord2f(0,0); glVertex3d(0,0,0);
    glTexCoord2f(0,1); glVertex3d(0,1,0);
    glTexCoord2f(1,1); glVertex3d(1,1,0);
    glTexCoord2f(1,0); glVertex3d(1,0,0);
    glEnd();
    
    //back
    glBegin(GL_QUADS);
    glNormal3d(0,0,-1);
    glTexCoord2f(0,0); glVertex3d(0,0,-1);
    glTexCoord2f(0,1); glVertex3d(0,1,-1);
    glTexCoord2f(1,1); glVertex3d(1,1,-1);
    glTexCoord2f(1,0); glVertex3d(1,0,-1);
    glEnd();
    
    //left
    glBegin(GL_QUADS);
    glNormal3d(-1,0,0);
    glTexCoord2f(0,0); glVertex3d(0,0,0);
    glTexCoord2f(0,1); glVertex3d(0,1,0);
    glTexCoord2f(1,1); glVertex3d(0,1,-1);
    glTexCoord2f(1,0); glVertex3d(0,0,-1);
    glEnd();
    
    //right
    glBegin(GL_QUADS);
    glNormal3d(1,0,0);
    glTexCoord2f(0,0); glVertex3d(1,0,0);
    glTexCoord2f(0,1); glVertex3d(1,1,0);
    glTexCoord2f(1,1); glVertex3d(1,1,-1);
    glTexCoord2f(1,0); glVertex3d(1,0,-1);
    glEnd();
    
    //top
    glBegin(GL_QUADS);
    glNormal3d(0,1,0);
    glTexCoord2f(0,0); glVertex3d(0,1,0);
    glTexCoord2f(0,1); glVertex3d(0,1,-1);
    glTexCoord2f(1,1); glVertex3d(1,1,-1);
    glTexCoord2f(1,0); glVertex3d(1,1,0);
    glEnd();
    
    //bottom
    glBegin(GL_QUADS);
    glNormal3d(0,-1,0);
    glTexCoord2f(0,0); glVertex3d(0,0,0);
    glTexCoord2f(0,1); glVertex3d(0,0,-1);
    glTexCoord2f(1,1); glVertex3d(1,0,-1);
    glTexCoord2f(1,0); glVertex3d(1,0,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
static void rectangle1(double x, double y, double z, double x1, double x2, double x3, double r1, double r2, double r3, double r4, int tex){
    glPushMatrix();
    glTranslated(x, y, z);
    glRotated(r1, r2, r3, r4);
    glScaled(x1, x2, x3);
    
    //float white[] = {1,1,1,1};
    //float shiny[] = {1,1,1,1};
    //float Emission[] = {0.0, 0.0, 0.01, 1.0};
    //float shiny = 1;
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, 1);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    //glMaterialfv(GL_FRONT, GL_EMISSION, Emission);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[tex]);
    //front
    glBegin(GL_QUADS);
    glNormal3d(0,0,1);
    glTexCoord2f(0,0); glVertex3d(0,0,0);
    glTexCoord2f(0,1); glVertex3d(0,1,0);
    glTexCoord2f(1,1); glVertex3d(1,1,0);
    glTexCoord2f(1,0); glVertex3d(1,0,0);
    glEnd();
    
    //back
    glBegin(GL_QUADS);
    glNormal3d(0,0,-1);
    glTexCoord2f(0,0); glVertex3d(0,0,-1);
    glTexCoord2f(0,1); glVertex3d(0,1,-1);
    glTexCoord2f(1,1); glVertex3d(1,1,-1);
    glTexCoord2f(1,0); glVertex3d(1,0,-1);
    glEnd();
    
    //left
    glBegin(GL_QUADS);
    glNormal3d(-1,0,0);
    glTexCoord2f(0,0); glVertex3d(0,0,0);
    glTexCoord2f(0,1); glVertex3d(0,1,0);
    glTexCoord2f(1,1); glVertex3d(0,1,-1);
    glTexCoord2f(1,0); glVertex3d(0,0,-1);
    glEnd();
    
    //right
    glBegin(GL_QUADS);
    glNormal3d(1,0,0);
    glTexCoord2f(0,0); glVertex3d(1,0,0);
    glTexCoord2f(0,1); glVertex3d(1,1,0);
    glTexCoord2f(1,1); glVertex3d(1,1,-1);
    glTexCoord2f(1,0); glVertex3d(1,0,-1);
    glEnd();
    
    //top
    glBegin(GL_QUADS);
    glNormal3d(0,1,0);
    glTexCoord2f(0,0); glVertex3d(0,1,0);
    glTexCoord2f(0,1); glVertex3d(0,1,-1);
    glTexCoord2f(1,1); glVertex3d(1,1,-1);
    glTexCoord2f(1,0); glVertex3d(1,1,0);
    glEnd();
    
    //bottom
    glBegin(GL_QUADS);
    glNormal3d(0,-1,0);
    glTexCoord2f(0,0); glVertex3d(0,0,0);
    glTexCoord2f(0,1); glVertex3d(0,0,-1);
    glTexCoord2f(1,1); glVertex3d(1,0,-1);
    glTexCoord2f(1,0); glVertex3d(1,0,0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
}
static void sphere1(double x,double y,double z,double r, double r2, double r3){
    
   const int d=15;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
   glScaled(r,r2,r3);
    //specular to white
    float white[] = {1,1,1,1};
    float shiny[] = {1,1,1,1};
    float Emission[] = {0.0, 0.0, 0.01, 1.0};
    //float shiny = 1;
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, 1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    glMaterialfv(GL_FRONT, GL_EMISSION, Emission);

   //  South pole cap
   glBegin(GL_TRIANGLE_FAN);

   Vertex(0,-90);
   for (int th=0;th<=360;th+=d)
   {
      Vertex(th,d-90);
   }
   glEnd();

   //  Latitude bands
   for (int ph=d-90;ph<=90-2*d;ph+=d)
   {
      glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=d)
      {
         Vertex(th,ph);
         Vertex(th,ph+d);
      }
      glEnd();
   }

   //  North pole cap
   glBegin(GL_TRIANGLE_FAN);
   Vertex(0,90);
   for (int th=0;th<=360;th+=d)
   {
      Vertex(th,90-d);
   }
   glEnd();

   //  Undo transformations
   glPopMatrix();
}
static void sphere2(double x, double y, double z, double r, double r2, double r3, double rot){
    const int d=15;

    //  Save transformation
    glPushMatrix();
    //  Offset and scale
    glTranslated(x,y,z);
    glRotated(rot, 1, 0, 0);
    glScaled(r,r2,r3);
     //specular to white
     float white[] = {1,1,1,1};
     float shiny[] = {1,1,1,1};
     float Emission[] = {0.0, 0.0, 0.01, 1.0};
     //float shiny = 1;
     //glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, 1);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
     glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
     glMaterialfv(GL_FRONT, GL_EMISSION, Emission);

    //  South pole cap
    glBegin(GL_TRIANGLE_FAN);

    Vertex(0,-90);
    for (int th=0;th<=360;th+=d)
    {
       Vertex(th,d-90);
    }
    glEnd();

    //  Latitude bands
    for (int ph=d-90;ph<=90-2*d;ph+=d)
    {
       glBegin(GL_QUAD_STRIP);
       for (int th=0;th<=360;th+=d)
       {
          Vertex(th,ph);
          Vertex(th,ph+d);
       }
       glEnd();
    }

    //  North pole cap
    glBegin(GL_TRIANGLE_FAN);
    Vertex(0,90);
    for (int th=0;th<=360;th+=d)
    {
       Vertex(th,90-d);
    }
    glEnd();

    //  Undo transformations
    glPopMatrix();
 }
static void sphere3(double x,double y,double z,double r, double r2, double r3, int rot, int tex){
    
   const int d=15;

   //  Save transformation
   glPushMatrix();
   //  Offset and scale
   glTranslated(x,y,z);
    glRotated(rot, 1, 0,0);
    //glRotated(180, 0, 0, 1);
    //glRotated(180, 0, 1, 0);
   glScaled(r,r2,r3);
   
    //specular to white
    float white[] = {1,1,1,1};
    float shiny[] = {1,1,1,1};
    float Emission[] = {0.0, 0.0, 0.01, 1.0};
    //float shiny = 1;
    //glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, 1);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, white);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shiny);
    glMaterialfv(GL_FRONT, GL_EMISSION, Emission);

    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[tex]);
    
   //  South pole cap
   glBegin(GL_TRIANGLE_FAN);

    glTexCoord2f(0, 0.5); Vertex(0,-90);
   for (int th=0;th<=360;th+=d)
   {
       glTexCoord2f(th/360.0, 0); Vertex(th,d-90);
   }
   glEnd();
   
   //  Latitude bands
   for (int ph=d-90;ph<=90-2*d;ph+=d)
   {
       glBegin(GL_QUAD_STRIP);
      for (int th=0;th<=360;th+=d)
      {
          //for just fur, ph+270 works
          glTexCoord2f((th)/360.0, (ph-d+90)/180.0-0); Vertex(th,ph);
          glTexCoord2f((th)/360.0, (ph+90)/180.0-0); Vertex(th,ph+d);
          
      }
       glEnd();
       
   }
    
   //  North pole cap
   glBegin(GL_TRIANGLE_FAN);
   glTexCoord2f(0, 0.5);  Vertex(0,90);
   for (int th=0;th<=360;th+=d)
   {
       glTexCoord2f(th/360.0, 1); Vertex(th,90-d);
   }
   glEnd();
   glDisable(GL_TEXTURE_2D);
   //  Undo transformations
   glPopMatrix();
}

static void ears(double x, double y, double z, double side, double os, double scale){
    //used to make the quads that form the ears
    
    glPushMatrix();
    glTranslated(x,y,z);
    glRotated(0,0,1,0);
    glScaled(scale, scale, scale);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[11]);
    glBegin(GL_QUADS);
    glColor3f(1,1,1);
    
    //ears!
    //glColor3f(1,1,1);
    glNormal3f(0,0,1);
    glTexCoord2f(0,1);glVertex3f(-1.3 *side,-1*os, 1);
    glTexCoord2f(0,0);glVertex3f(+0.7*side,-1*os, 1);
    glTexCoord2f(1,0); glVertex3f(+1.3*side,+1*os, 1);
    glTexCoord2f(1,1);glVertex3f(-0.7*side,+1*os, 1);
    //  Back
    glEnd();
    glBegin(GL_QUADS);
    glNormal3f(70*side, 10*os, -28);
    glTexCoord2f(0,1);glVertex3f(+0.5*side,-1*os,-1);
    glTexCoord2f(1,1);glVertex3f(-0.3*side,-1*os,-1);
    glTexCoord2f(1,0);glVertex3f(-0.3*side,+0.4*os,-0.5);
    glTexCoord2f(0,0);glVertex3f(+0.5*side,+0.4*os,-0.5);
    glEnd();
    //  Right
    glBegin(GL_QUADS);
    glNormal3f(140*side, 5*os, -14);
    glTexCoord2f(0,0);glVertex3f(+0.7*side,-1*os,+1);
    glTexCoord2f(1,0);glVertex3f(+0.5*side,-1*os,-1);
    glTexCoord2f(1,1);glVertex3f(+0.5*side,+0.4*os,-0.5);
    glTexCoord2f(0,1);glVertex3f(+1.3*side,+1*os,+1);
    glEnd();
    //  Left
    glBegin(GL_QUADS);
    glNormal3f(-10*side, -3*os, 5);
    glTexCoord2f(0,0);glVertex3f(-0.3*side,-1*os,-1);
    glTexCoord2f(1,0);glVertex3f(-1.3*side,-1*os,+1);
    glTexCoord2f(1,1);glVertex3f(-0.7*side,+1*os,+1);
    glTexCoord2f(0,1);glVertex3f(-0.3*side,+0.4*os,-0.5);
    glEnd();
    glBegin(GL_QUADS);
    //  Top
    glNormal3f(0, 15*os, -6);
    glTexCoord2f(0,0);glVertex3f(-0.7*side,+1*os,+1);
    glTexCoord2f(1,0);glVertex3f(+1.3*side,+1*os,+1);
    glTexCoord2f(1,1);glVertex3f(+0.5*side,+0.4*os,-0.5);
    glTexCoord2f(0,1);glVertex3f(-0.3*side,+0.4*os,-0.5);
    
    glEnd();
    glBegin(GL_QUADS);
    //  Bottom
    glNormal3f(0,-1*os,0);
    glVertex3f(-0.3*side,-1*os,-1);
    glVertex3f(+0.5*side,-1*os,-1);
    glVertex3f(+0.7*side,-1*os,+1);
    glVertex3f(-1.3*side,-1*os,+1);
    

    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
    
    
}
static void leaf(double x, double y, double z, double s, double r, double r2, double r3, double green){

    glPushMatrix();
    glTranslated(x,y,z);
    glRotated(r, 1, 0, 0);
    glRotated(r2, 0, 1, 0);
    glRotated(r3, 0, 0, 1);
    glScaled(s,s,s);
    
    glColor3f(0.5, green, 0.3);
    sphere3(0, 0.5, 0, 0.7, 0.7, 0.7, 270, 8);
    
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};

    glMaterialfv(GL_FRONT_AND_BACK,GL_SPECULAR,white);
    glMaterialfv(GL_FRONT_AND_BACK,GL_EMISSION,black);
    
    /*glColor3f(0,green,0);
    glVertex3f(0, 1, 0);
    glVertex3f(0.5, 0, 0);
    glVertex3f(0, -0.7, 0);
    glVertex3f(-0.5, 0, 0);*/
    
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[1]);
    glBegin(GL_QUADS);
    glColor3f(0.627, 0.322, 0.176);
    glNormal3d(0, 0, 1);
    glTexCoord2f(1,0);glVertex3f(0.05, -0.2, 0.05);
    glTexCoord2f(0,0);glVertex3f(-0.05, -0.2, 0.05);
    glTexCoord2f(0,1);glVertex3f(-0.05, -1, 0.05);
    glTexCoord2f(1,1);glVertex3f(0.05, -1, 0.05);
    glEnd();
    glBegin(GL_QUADS);
    glNormal3d(0, 0, -1);
    glTexCoord2f(1,0);glVertex3f(0.05, -0.2, -0.05);
    glTexCoord2f(0,0);glVertex3f(-0.05, -0.2, -0.05);
    glTexCoord2f(0,1);glVertex3f(-0.05, -1, -0.05);
    glTexCoord2f(1,1);glVertex3f(0.05, -1, -0.05);
    glEnd();
    glBegin(GL_QUADS);
    
   
    glNormal3d(1, 0, 0);
    glTexCoord2f(1,1);glVertex3f(0.05, -0.2, 0.05);
    glTexCoord2f(1,0);glVertex3f(0.05, -0.2, -0.05);
    glTexCoord2f(0,0);glVertex3f(0.05, -1, -0.05);
    glTexCoord2f(0,1);glVertex3f(0.05, -1, 0.05);
    glEnd();
    glBegin(GL_QUADS);
    glNormal3d(-1, 0, 0);
    glTexCoord2f(1,1);glVertex3f(-0.05, -0.2, 0.05);
    glTexCoord2f(1,0);glVertex3f(-0.05, -0.2, -0.05);
    glTexCoord2f(0,0);glVertex3f(-0.05, -1, -0.05);
    glTexCoord2f(0,1);glVertex3f(-0.05, -1, 0.05);
    glEnd();
    glBegin(GL_QUADS);
    glNormal3d(0, 1, 0);
    glTexCoord2f(0,1);glVertex3f(-0.05, -0.2, 0.05);
    glTexCoord2f(0,0);glVertex3f(-0.05, -0.2, -0.05);
    glTexCoord2f(1,0);glVertex3f(0.05, -0.2, -0.05);
    glTexCoord2f(1,1);glVertex3f(0.05, -0.2, 0.05);
    glEnd();
    glBegin(GL_QUADS);
    glNormal3d(0, -1, 0);
    glTexCoord2f(0,1);glVertex3f(-0.05, -1, 0.05);
    glTexCoord2f(0,0);glVertex3f(-0.05, -1, -0.05);
    glTexCoord2f(1,0);glVertex3f(0.05, -1, -0.05);
    glTexCoord2f(1,1);glVertex3f(0.05, -1, 0.05);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
}
static void branch(double x, double y, double z, double h, double r, double r2, double r3){
    
    glPushMatrix();
    glTranslated(x,y,z);
    glRotated(r, 1, 0, 0);
    glRotated(r2, 0, 1, 0);
    glRotated(r3, 0, 0, 1);
    glScaled(h,h,h);
    glColor3f(1,1,1);
    //glColor3f(0.957, 0.643, 0.376);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[7]);
    
    glBegin(GL_QUADS);
    //front
    
    glNormal3f(0, 0, 1);
    glTexCoord2f(0,1); glVertex3f(-1.7, -0.5, 0);
    glTexCoord2f(0,0); glVertex3f(-1.7, -1, 0);
    glTexCoord2f(1,0); glVertex3f(0, -1, 0);
    glTexCoord2f(1,1); glVertex3f(0, -0.5, 0);
    glEnd();
    glBegin(GL_QUADS);
    //front point
    //glColor3f(0.627, 0.322, 0.176);
    //line eq: 5x + 26z = 0
    glNormal3f(5, 0, 26);
    glTexCoord2f(0,0); glVertex3f(0, -1, 0);
    glTexCoord2f(0,1); glVertex3f(0, -0.5, 0);
    glTexCoord2f(0.5,1); glVertex3f(1.3, -0.2, -0.25);
    glTexCoord2f(0, 0); glVertex3f(0, -1, 0);
    glEnd();
    glBegin(GL_QUADS);
    //back
    glNormal3f(0, 0, -1);
    glTexCoord2f(0,1); glVertex3f(-1.7, -0.5, -0.5);
    glTexCoord2f(0,0); glVertex3f(-1.7, -1, -0.5);
    glTexCoord2f(1,0); glVertex3f(0, -1, -0.5);
    glTexCoord2f(1,1); glVertex3f(0, -0.5, -0.5);
    glEnd();
    glBegin(GL_QUADS);
    //back point
    glNormal3f(5, 0, -26);
    //glColor3f(0.627, 0.322, 0.176);
    glTexCoord2f(0,0); glVertex3f(0, -1, -0.5);
    glTexCoord2f(0,1); glVertex3f(0, -0.5, -0.5);
    glTexCoord2f(0.5,1); glVertex3f(1.3, -0.2, -0.25);
    glTexCoord2f(0,0); glVertex3f(0, -1, -0.5);
    glEnd();
    glBegin(GL_QUADS);
    //end cap
    glNormal3f(-1, 0, 0);
    //glColor3f(0.545, 0.271, 0.075);
    glTexCoord2f(1,1); glVertex3f(-1.7, -0.5, 0);
    glTexCoord2f(0,1); glVertex3f(-1.7, -1, 0);
    glTexCoord2f(0,0); glVertex3f(-1.7, -1, -0.5);
    glTexCoord2f(1,0); glVertex3f(-1.7, -0.5, -0.5);
    glEnd();
    glBegin(GL_QUADS);
    //top
    glNormal3f(0, 1, 0);
    //glColor3f(0.804, 0.522, 0.247);
    glTexCoord2f(0,1); glVertex3f(-1.7, -0.5, 0);
    glTexCoord2f(0,0); glVertex3f(-1.7, -0.5, -0.5);
    glTexCoord2f(1,0); glVertex3f(0, -0.5, -0.5);
    glTexCoord2f(1,1); glVertex3f(0, -0.5, 0);
    glEnd();
    glBegin(GL_QUADS);
    //bottom
    glNormal3f(0, -1, 0);
    glColor3f(0.545, 0.271, 0.075);
    glTexCoord2f(0,1); glVertex3f(-1.7, -1, 0);
    glTexCoord2f(0,0); glVertex3f(-1.7, -1, -0.5);
    glTexCoord2f(1,0); glVertex3f(0, -1, -0.5);
    glTexCoord2f(1,1); glVertex3f(0, -1, 0);
    glEnd();
    glBegin(GL_QUADS);
    //top point
    //glColor3f(0.804, 0.522, 0.247);
    glNormal3f(-6, 26, 0);
    glTexCoord2f(0,1); glVertex3f(0, -0.5, 0);
    glTexCoord2f(0,0); glVertex3f(0, -0.5, -0.5);
    glTexCoord2f(0.5,1); glVertex3f(1.3, -0.2, -0.25);
    glTexCoord2f(0,1); glVertex3f(0, -0.5, 0);
    glEnd();
    glBegin(GL_QUADS);
    //bottom point
    //glColor3f(0.545, 0.271, 0.075);
    glNormal3f(8, -13, 0);
    glTexCoord2f(0,1); glVertex3f(0, -1, 0);
    glTexCoord2f(0,0); glVertex3f(0, -1, -0.5);
    glTexCoord2f(0.5,1); glVertex3f(1.3, -0.2, -0.25);
    glTexCoord2f(0,1); glVertex3f(0, -1, 0);
    
    glEnd();
    glDisable(GL_TEXTURE_2D);
    leaf(0.6, 0, 0, 0.6, 20, 40, 0, 0.6);
    leaf(0.6, 0, -0.8, 0.6, -50, -30, 0, 0.7);
    leaf(0.6, -0.7, 0.3, 0.6, 100,-20, 0, 0.4);
    leaf(-0.3, -1.2, 0.3, 0.6, 130,0,0, 0.5);
    leaf(-0.3, -0.8, 0.2, 0.4, 60,0,0, 0.9);
    leaf(-0.7, -0.2, -0.25, 0.6, 0,-70,0, 0.3);
    leaf(-0.9, -1, -0.6, 0.5, -100,0,0, 0.4);
    leaf(1.5, -0.23, -0.4, 0.4, 20,20,-90, 0.6);
    leaf(1.2, -0.04, -0.36, 0.3, 0,20,-30, 0.8);
    leaf(0.5, -1.2, 0, 0.62, 0, -30, -150, 0.3);
    leaf(0.5, -1, -0.6, 0.4, 0, 30, -110, 0.8);
    leaf(-0.5, -1.2, -0.3, 0.42, 0, 0, -180, 0.8);
    leaf(-1.2, -0.2, -0.3, 0.52, 0, -30, 30, 0.6);
    leaf(-0.2, -1.2, -0.7, 0.62, 30, -30, -190, 0.9);
    leaf(-0.2, -0.4, -0.7, 0.62, 0, 40, -45, 0.5);
    glColor3f(0,1,0);
    sphere3(-0.5, -1, -0.2, 0.5, 0.5, 0.5, 270, 8);
    glPopMatrix();
}
static void apple(double x, double y, double z, double h, double r){
    //used to make the apple ring
    //const int d=15;

    //  Save transformation
    glPushMatrix();
    //  Offset and scale
    glTranslated(x,y,z);
    glRotated(h, 1, 0, 0);
    glScaled(r,r,r);
    
    glBegin(GL_QUADS);
    glColor3f(0,1,0);
    glNormal3f(0, 0, 1);
    glVertex3f(-0.1,0, 0);
    glVertex3f(+0.1,0, 0);
    glVertex3f(+0.1,1.5, 0);
    glVertex3f(-0.1,1.5, 0);
    
    glNormal3f(0, 0, 1);
    glVertex3f(0, 0.9, 0);
    glVertex3f(0.5, 1.0, 0);
    glVertex3f(0.6, 1.4, 0);
    glVertex3f(0.3, 1.3, 0);
    
    
    glEnd();
    //  Latitude bands
    glColor3f(1,0,0);
    sphere3(0,0,0,1,1,1, 270, 3);
    
    //  Undo transformations
    glPopMatrix();
    
    
}
static void tree(double x, double y, double z, double s, double r){
    glPushMatrix();
   
    
    //  Offset and scale
    glTranslated(x,y,z);
    glRotated(r, 0, 1, 0);
    glScaled(s,s,s);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[6]);
    
    glBegin(GL_QUADS);
    //glColor3f(0.957, 0.643, 0.376);
    glColor3f(1,1,1);
    //front
    glNormal3f(0, 2, 12);
    glTexCoord2f(0.4, 0.9); glVertex3f(-0.1, 0, 0.25);
    glTexCoord2f(0.6, 0.9); glVertex3f(0.1, 0, 0.25);
    glTexCoord2f(1, 0); glVertex3f(0.35, -1.5, 0.5);
    glTexCoord2f(0, 0); glVertex3f(-0.35, -1.5, 0.5);
    glEnd();
    
    //back
    glBegin(GL_QUADS);
    //glColor3f(0.7, 0.4, 0.2);
    glNormal3f(0, 2, -12);
    glTexCoord2f(0.4, 0.9); glVertex3f(-0.1, 0, -0.25);
    glTexCoord2f(0.6, 0.9); glVertex3f(0.1, 0, -0.25);
    glTexCoord2f(1, 0); glVertex3f(0.35, -1.5, -0.5);
    glTexCoord2f(0, 0); glVertex3f(-0.35, -1.5, -0.5);
    glEnd();
    
    //left
    glBegin(GL_QUADS);
    //glColor3f(0.627, 0.322, 0.176);
    glNormal3f(-12, 2, 0);
    glTexCoord2f(0.4, 0.9); glVertex3f(-0.25, 0, -0.1);
    glTexCoord2f(0.6, 0.9);  glVertex3f(-0.25, 0, 0.1);
    glTexCoord2f(1, 0); glVertex3f(-0.5, -1.5, 0.35);
    glTexCoord2f(0, 0); glVertex3f(-0.5, -1.5, -0.35);
    glEnd();
    
    glBegin(GL_QUADS);
    //right
    //glColor3f(0.627, 0.4, 0.176);
    glNormal3f(12, 2, 0);
    glTexCoord2f(0.4, 0.9); glVertex3f(0.25, 0, -0.1);
    glTexCoord2f(0.6, 0.9); glVertex3f(0.25, 0, 0.1);
    glTexCoord2f(1, 0); glVertex3f(0.5, -1.5, 0.35);
    glTexCoord2f(0, 0); glVertex3f(0.5, -1.5, -0.35);
    glEnd();

    glDisable(GL_TEXTURE_2D);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[7]);
    glBegin(GL_QUADS);
    //corner fl
    //glColor3f(0.545, 0.271, 0.075);
    glNormal3f(-60, 20, 60);
    glTexCoord2f(0, 1); glVertex3f(-0.1, 0, 0.25);
    glTexCoord2f(1, 1); glVertex3f(-0.25, 0, 0.1);
    glTexCoord2f(1, 0); glVertex3f(-0.5, -1.5, 0.35);
    glTexCoord2f(0, 0); glVertex3f(-0.35, -1.5, 0.5);
    glEnd();
    glBegin(GL_QUADS);
    
    //glColor3f(0.545, 0.271, 0.075);
    glNormal3f(60, 20, 60);
    glTexCoord2f(0, 1); glVertex3f(0.1, 0, 0.25);
    glTexCoord2f(1, 1); glVertex3f(0.25, 0, 0.1);
    glTexCoord2f(1, 0); glVertex3f(0.5, -1.5, 0.35);
    glTexCoord2f(0, 0); glVertex3f(0.35, -1.5, 0.5);
    glEnd();
    
    //glColor3f(0.627, 0.322, 0.176);
    glBegin(GL_QUADS);
    glNormal3f(-60, 20, -60);
    glTexCoord2f(0, 1); glVertex3f(-0.1, 0, -0.25);
    glTexCoord2f(1, 1); glVertex3f(-0.25, 0, -0.1);
    glTexCoord2f(1, 0);glVertex3f(-0.5, -1.5, -0.35);
    glTexCoord2f(0, 0); glVertex3f(-0.35, -1.5, -0.5);
    glEnd();
    
    //glColor3f(0.545, 0.271, 0.075);
    glBegin(GL_QUADS);
    glNormal3f(60, 20, -60);
    glTexCoord2f(0, 1); glVertex3f(0.1, 0, -0.25);
    glTexCoord2f(1, 1); glVertex3f(0.25, 0, -0.1);
    glTexCoord2f(1, 0); glVertex3f(0.5, -1.5, -0.35);
    glTexCoord2f(0, 0); glVertex3f(0.35, -1.5, -0.5);
    glEnd();
    
    glBegin(GL_QUADS);
    glNormal3f(0, 2, -12);
    glTexCoord2f(0, 1); glVertex3f(-0.1, 0, 0.25);
    glTexCoord2f(1, 1); glVertex3f(0.1, 0, 0.25);
    glTexCoord2f(1, 0); glVertex3f(0.35, -1.5, 0.5);
    glTexCoord2f(0, 0); glVertex3f(-0.35, -1.5, 0.5);
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    //begin flipped
    //glColor3f(0.6, 0.3, 0.1);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[6]);
   
    glBegin(GL_QUADS);
    //front
    glNormal3f(0, -2, 12);
    glTexCoord2f(0.4, 0.9); glVertex3f(-0.1, 0, 0.25);
    glTexCoord2f(0.6, 0.9); glVertex3f(0.1, 0, 0.25);
    glTexCoord2f(1, 0); glVertex3f(0.35, 1.5, 0.5);
    glTexCoord2f(0, 0); glVertex3f(-0.35, 1.5, 0.5);
    glEnd();
    //back
    //glColor3f(0.7, 0.4, 0.2);
    glBegin(GL_QUADS);
    glNormal3f(0, -2, -12);
    glTexCoord2f(0.4, 0.9); glVertex3f(-0.1, 0, -0.25);
    glTexCoord2f(0.6, 0.9); glVertex3f(0.1, 0, -0.25);
    glTexCoord2f(1, 0); glVertex3f(0.35, 1.5, -0.5);
    glTexCoord2f(0, 0);  glVertex3f(-0.35, 1.5, -0.5);
    glEnd();
    //left
    //glColor3f(0.627, 0.322, 0.176);
    glBegin(GL_QUADS);
    glNormal3f(-12, -2, 0);
    glTexCoord2f(0.4, 0.9); glVertex3f(-0.25, 0, -0.1);
    glTexCoord2f(0.6, 0.9);  glVertex3f(-0.25, 0, 0.1);
    glTexCoord2f(1, 0); glVertex3f(-0.5, 1.5, 0.35);
    glTexCoord2f(0, 0); glVertex3f(-0.5, 1.5, -0.35);
    glEnd();
    
    //right
    //glColor3f(0.627, 0.4, 0.176);
    glBegin(GL_QUADS);
    glNormal3f(12, -2, 0);
    glTexCoord2f(0.4, 0.9); glVertex3f(0.25, 0, -0.1);
    glTexCoord2f(0.6, 0.9); glVertex3f(0.25, 0, 0.1);
    glTexCoord2f(1, 0); glVertex3f(0.5, 1.5, 0.35);
    glTexCoord2f(0, 0); glVertex3f(0.5, 1.5, -0.35);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[7]);
    //corner fl
   // glColor3f(0.545, 0.271, 0.075);
    glBegin(GL_QUADS);
    glNormal3f(-60, -20, 60);
    glTexCoord2f(0, 1); glVertex3f(-0.1, 0, 0.25);
    glTexCoord2f(1, 1); glVertex3f(-0.25, 0, 0.1);
    glTexCoord2f(1, 0); glVertex3f(-0.5, 1.5, 0.35);
    glTexCoord2f(0, 0); glVertex3f(-0.35, 1.5, 0.5);
    glEnd();
    
    //glColor3f(0.545, 0.271, 0.075);
    glBegin(GL_QUADS);
    glNormal3f(60, -20, 60);
    glTexCoord2f(0, 1); glVertex3f(0.1, 0, 0.25);
    glTexCoord2f(1, 1); glVertex3f(0.25, 0, 0.1);
    glTexCoord2f(1, 0); glVertex3f(0.5, 1.5, 0.35);
    glTexCoord2f(0, 0); glVertex3f(0.35, 1.5, 0.5);
    glEnd();
    
    //glColor3f(0.627, 0.322, 0.176);
    glBegin(GL_QUADS);
    glNormal3f(-60, -20, -60);
    glTexCoord2f(0, 1); glVertex3f(-0.1, 0, -0.25);
    glTexCoord2f(1, 1); glVertex3f(-0.25, 0, -0.1);
    glTexCoord2f(1, 0); glVertex3f(-0.5, 1.5, -0.35);
    glTexCoord2f(0, 0); glVertex3f(-0.35, 1.5, -0.5);
    glEnd();
    
    //glColor3f(0.545, 0.271, 0.075);
    glBegin(GL_QUADS);
    glNormal3f(60, 20, -60);
    glTexCoord2f(0, 1); glVertex3f(0.1, 0, -0.25);
    glTexCoord2f(1, 1); glVertex3f(0.25, 0, -0.1);
    glTexCoord2f(1, 0); glVertex3f(0.5, 1.5, -0.35);
    glTexCoord2f(0, 0); glVertex3f(0.35, 1.5, -0.5);
    glEnd();
    
    glBegin(GL_QUADS);
    glNormal3f(0, -2, -12);
    glTexCoord2f(0, 1); glVertex3f(-0.1, 0, 0.25);
    glTexCoord2f(1, 1); glVertex3f(0.1, 0, 0.25);
    glTexCoord2f(1, 0); glVertex3f(0.35, 1.5, 0.5);
    glTexCoord2f(0, 0); glVertex3f(-0.35, 1.5, 0.5);
    //end flipped
    glEnd();
    
    glDisable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[6]);
    //START CHANGING HERE
    //begin top cap
    //glColor3f(0.627, 0.322, 0.176);
    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-0.5, 1.5, 0.35);
    glTexCoord2f(0.3, 1); glVertex3f(-0.35, 1.5, 0.5);
    glTexCoord2f(0.8, 1); glVertex3f(0.35, 1.5, 0.5);
    glTexCoord2f(1, 0); glVertex3f(0.5, 1.5, 0.35);
    glEnd();
    
    glBegin(GL_QUADS);
    //glColor3f(0.627, 0.322, 0.176);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(-0.5, 1.5, -0.35);
    glTexCoord2f(0.3, 1); glVertex3f(-0.35, 1.5, -0.5);
    glTexCoord2f(0.8, 1); glVertex3f(0.35, 1.5, -0.5);
    glTexCoord2f(1, 0); glVertex3f(0.5, 1.5, -0.35);
    glEnd();
    
    glBegin(GL_QUADS);
    //glColor3f(0.627, 0.322, 0.176);
    glNormal3f(0, 1, 0);
    glTexCoord2f(0, 0); glVertex3f(0.5, 1.5, -0.35);
    glTexCoord2f(0, 1); glVertex3f(0.5, 1.5, 0.35);
    glTexCoord2f(1, 1); glVertex3f(-0.5, 1.5, 0.35);
    glTexCoord2f(1, 0); glVertex3f(-0.5, 1.5, -0.35);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    
    
    //end top cap
    
    
    glBegin(GL_QUADS);
    //begin bottom cap
    //glColor3f(0.627, 0.322, 0.176);
    glNormal3f(0, -1, 0);
    glVertex3f(-0.5, -1.5, 0.35);
    glVertex3f(-0.35, -1.5, 0.5);
    glVertex3f(0.35, -1.5, 0.5);
    glVertex3f(0.5, -1.5, 0.35);
    
    glNormal3f(0, -1, 0);
    //glColor3f(0.627, 0.322, 0.176);
    glVertex3f(-0.5, -1.5, -0.35);
    glVertex3f(-0.35, -1.5, -0.5);
    glVertex3f(0.35, -1.5, -0.5);
    glVertex3f(0.5, -1.5, -0.35);
    
    glNormal3f(0, -1, 0);
    //glColor3f(0.627, 0.322, 0.176);
    glVertex3f(0.5, -1.5, -0.35);
    glVertex3f(0.5, -1.5, 0.35);
    glVertex3f(-0.5, -1.5, 0.35);
    glVertex3f(-0.5, -1.5, -0.35);
    //end bottom cap
    glEnd();
    
    //big 4
    branch(1.2, 1.74, 0, 0.5, 0,0,0);
    branch(-1.2, 1.74, 0, 0.5, 0,180,0);
    branch(0, 1.74, 1.2, 0.5, 0,-90,0);
    branch(0, 1.74, -1.2, 0.5, 0,90,0);
    
    //big 4 out of top
    branch(0.5, 2.2, -0.23, 0.6, 0,45,45);
    branch(0.3, 2.2, 0.5, 0.6, 0,-45,45);
    branch(-0.5, 2.2, 0.23, 0.6, 0,225,45);
    branch(-0.3, 2.2, -0.5, 0.6, 0,-225,45);
    
    //smaller
    branch(1, 1.8, -0.3, 0.3, 0, 45, 45);
    
    branch(0.9, 1.3, 0.6, 0.4, 0, -20, -20);
    branch(-0.9, 1.3, -0.6, 0.4, 0, -200, -20);
    branch(-0.3, 0.8, 0.6, 0.3, 0, -110, 5);
    branch(0.4, 0.8, -0.6, 0.35, 0, 70, 10);
    
  // small on the big 4 out of top
    branch(0.1, 1.8, 0.5, 0.3, 0, -132, 20);
    branch(0.3, 2.4, 0.5, 0.3, 0, -30, 90);
    branch(-0.7, 1.2, 0.7, 0.4, 0, 45, 180);
    
    apple(-0.2,0.25,0.6, 0, 0.15);
    apple(-0.2,1.2,-1.4, 0, 0.15);
    apple(0.5,0.25,-0.6, 0, 0.15);
    glPopMatrix();
    
    
    
}
static void rock(double x, double y, double z, double size, double r){
    //ax(10);
    glPushMatrix();
    glTranslated(x,y,z);
    glRotated(r, 0, 1, 0);
    glScaled(size,size,size);
    
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[3]);
    glBegin(GL_QUADS);
    //cap 1
    glColor3f(1,1,1);
    //glColor3f(0.35, 0.6, 0.7);
    glNormal3f(1, 10, -2);
    glTexCoord2f(0.5, 1); glVertex3f(0, 0.4, 0);
    glTexCoord2f(1, 1); glVertex3f(1, 0.5, 0);
    glTexCoord2f(0, 0); glVertex3f(-1, 0.5, -1);
    glTexCoord2f(0.5, 1); glVertex3f(0, 0.4, 0);
    glEnd();
    
    
    glBegin(GL_QUADS);
    //cap 2
    //glColor3f(0.23, 0.23, 1);
    glNormal3f(2, 6, -2);
    glTexCoord2f(1, 0); glVertex3f(1, 0.5, 0);
    glTexCoord2f(0, 0); glVertex3f(-1, 0.5, -1);
    glTexCoord2f(0, 1); glVertex3f(0,0, -1.5);
    glTexCoord2f(1, 1); glVertex3f(1, 0, -1);
    glEnd();
    
    
    glBegin(GL_QUADS);
    
    
    //top center
   // glColor3f(0.5,0.5,0.5);
    glNormal3f(1, 10, 18);
    glTexCoord2f(0, 1);glVertex3f(0, 0.4, 0);
    glTexCoord2f(0, 0);glVertex3f(-1, -1.5, 1);
    glTexCoord2f(1, 0);glVertex3f(2, -1, 1);
    glTexCoord2f(1, 1);glVertex3f(1, 0.5, 0);
    glEnd();
    
    
    glBegin(GL_QUADS);
    
    //top left
    //glColor3f(1,0,0);
    glNormal3f(-9, 10, 10);
    glTexCoord2f(0, 0);glVertex3f(0,0.4,0);
    glTexCoord2f(0, 1);glVertex3f(-1, -1.5, 1);
    glTexCoord2f(1, 1); glVertex3f(-1.5, -1.4, -1);
    glTexCoord2f(1, 0);glVertex3f(-1, 0.5, -1);
    glEnd();
    
    
    glBegin(GL_QUADS);
    //top right
    //glColor3f(0,1,0);
    glNormal3f(2, 2, 0);
    glTexCoord2f(0, 0);glVertex3f(2, -1, 1);
    glTexCoord2f(1, 0);glVertex3f(2, -1, -1);
    glTexCoord2f(1, 1);glVertex3f(1, 0, -1);
    glTexCoord2f(0, 1);glVertex3f(1, 0.5, 0);
    glEnd();
    
    
    glBegin(GL_QUADS);
    //back left
    //glColor3f(0,0,1);
    glNormal3f(-38, 10, -106);
    glTexCoord2f(0, 0);glVertex3f(0,-1,-1.5);
    glTexCoord2f(0, 1);glVertex3f(-1.5, -1.4, -1);
    glTexCoord2f(1, 1);glVertex3f(-1, 0.5, -1);
    glTexCoord2f(1, 0);glVertex3f(0, 0, -1.5);
    glEnd();
    
    
    glBegin(GL_QUADS);
    //back center
    //glColor3f(1,1,0);
    glNormal3f(1, 0, -4);
    glTexCoord2f(0, 0);glVertex3f(0,0, -1.5);
    glTexCoord2f(1, 0);glVertex3f(0,-1, -1.5);
    glTexCoord2f(1, 1);glVertex3f(2, -1, -1);
    glTexCoord2f(0, 1);glVertex3f(1, 0, -1);
    glEnd();
    
    
    glBegin(GL_QUADS);
    //bottom center
    glNormal3f(2, -12, 23);
    glTexCoord2f(0, 0);glVertex3f(-1, -1.5, 1);
    glTexCoord2f(1, 0);glVertex3f(2, -1, 1);
    glTexCoord2f(1, 1);glVertex3f(1.75, -2, 0.5);
    glTexCoord2f(0, 1);glVertex3f(-0.75, -2, 0.5);
    glEnd();
    
    
    glBegin(GL_QUADS);
    //bottom left
    //glColor3f(0,1,1);
    glNormal3f(-46, -50, 9);
    glTexCoord2f(0, 0);glVertex3f(-1, -1.5, 1);
    glTexCoord2f(0, 1);glVertex3f(-1.5, -1.4, -1);
    glTexCoord2f(1, 1);glVertex3f(-0.75, -2, -0.5);
    glTexCoord2f(1, 0);glVertex3f(-0.75, -2, 0.5);
    glEnd();
    
    
    glBegin(GL_QUADS);
    //bottom right
    //glColor3f(1,0,1);
    glNormal3f(4, -1, 9);
    glTexCoord2f(0, 0);glVertex3f(2,-1, 1);
    glTexCoord2f(0, 1);glVertex3f(2, -1, -1);
    glTexCoord2f(1, 1);glVertex3f(1.75, -2, -0.25);
    glTexCoord2f(1, 0);glVertex3f(1.75, -2, 0.5);
    glEnd();
    
    
    glBegin(GL_QUADS);
    //bottom back left
    //glColor3f(1,0,0);
    glNormal3f(-4, -45, -48);
    glTexCoord2f(0, 0);glVertex3f(0, -1, -1.5);
    glTexCoord2f(0, 1);glVertex3f(-1.5, -1.4, -1);
    glTexCoord2f(1, 1);glVertex3f(-0.75, -2, -0.5);
    glTexCoord2f(1, 0);glVertex3f(0, -2, -0.4);
    glEnd();
    
    
    glBegin(GL_QUADS);
    //bottom back
    //glColor3f(1,0.5,0.5);
    glNormal3f(3, -27, -35);
    glTexCoord2f(0, 0);glVertex3f(0,-2, -0.4);
    glTexCoord2f(0, 1);glVertex3f(1.75, -2, -0.25);
    glTexCoord2f(1, 1);glVertex3f(2, -1, -1);
    glTexCoord2f(1, 0);glVertex3f(0,-1, -1.5);
    glEnd();
    
    
    glBegin(GL_QUADS);
    //base
    //glColor3f(0.5, 0.75, 1);
    glNormal3f(0, -1, 0);
    glTexCoord2f(0, 0);glVertex3f(1.75, -2, -0.25);
    glTexCoord2f(0, 1);glVertex3f(-0.75, -2, -0.5);
    glTexCoord2f(1, 1);glVertex3f(-0.75, -2, 0.5);
    glTexCoord2f(1, 0);glVertex3f(1.75, -2, 0.5);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();
    
}
static void flowerbloom(double x, double y, double z, double size, double r){
    glPushMatrix();
    glTranslated(x, y, z);
    glRotated(r, 0, 1, 0);
    glScaled(size, size, size);
    glColor3f(1,1,1);
    
    
    
    sphere2(0,0,0,0.5,1,2,120);
    
    sphere2(0,0,0,0.5,1,2,180);
    sphere2(0,0,0,0.5,1,2, 240);
    sphere2(0,0,0,0.5,1,2,300);
    glColor3f(1,1,0);
    sphere2(0,0,0,0.75,0.75,0.75,0);
    glColor3f(1,1,1);
    rectangle(-0.25, -1, 0, 0.75, 3, 0.5, -170, 1, 0, 0);
    rectangle(-0.25, -4, -0.7, 0.75, 5, 0.5, 170, 1, 0, 0);
    glColor3f(0,1,0);
    glBegin(GL_QUADS);
    glVertex3f(0, -2.5, -2);
    glVertex3f(0, -3.5, -5);
    glVertex3f(0, -4.5, -2);
    glVertex3f(0, -3, -0.5);
    
    glEnd();
    
    glPopMatrix();
}
static void flowerbunch(double x, double y, double z, double size, double r){
    glPushMatrix();
    glTranslated(x, y, z);
    glRotated(r, 0, 1, 0);
    glScaled(size, size, size);
    flowerbloom(1,1,1,1, 30);
    flowerbloom(10,1,0.5,1, 130);
    flowerbloom(0.5,0,5,0.9, 60);
    flowerbloom(15,0,5,0.9, -70);
    flowerbloom(5,0,10,0.9, 10);
    flowerbloom(5,-0.5,-5,1.2, 0);
    glPopMatrix();
    
}
void reset(){
    if(mode==1){
       objX = 0.6; //panda x
        objZ = -14.8;
        ph = 30;
        th = 30;
        jsum = -2;
        
    }
    else if(mode==2){
       
        ph = 30;
        th = 0;
        objX = 0.6;
        objZ = -14.8;
        jsum = -2;
    }
    
}

void reset_view(){
    //to change views, not reset panda.
    if(mode==1){
        ph = 30;
        th = 30;
    }
    else{
        ph = 30;
        th = 0;
        
    }
    
}
static void redPanda(double x, double y, double z, double s, double r){
    glPushMatrix();
    //  Offset and scale
    glTranslated(x,y,z);
    glRotated(r, 0, 1, 0);
    glScaled(s,s,s);
    glColor3f(1, 0.55, 0);
    sphere3(0, 1, 0, 1.3, 1, 1,0,11);//head
    sphere3(0, 0, 0, 0.5, 0.7, 0.5, 0, 11);//body
    glColor3f(0.3, 0.2, 0.1);
   // sphere1(0.4, -0.65, 0, 0.25, 0.15, 0.25);//feet
   // sphere1(-0.4, -0.65, 0, 0.25, 0.15, 0.25);//feet
    sphere3(0, 0.51, 1.14, 0.08, 0.12, 0.03, 0, 11);
    glColor3f(1, 0.7, 0);
    //glPushMatrix();
    //glRotated(10,0, 0,0);
    //tail
    //glPopMatrix();
    
    glColor3f(1, 1, 1);
    sphere3(0, 0.6, 0.75, 0.5,0.5, 0.4, 0 ,11);//snout
    sphere3(0, -0.3, -0.8, 0.25, 0.25, 0.5, 0, 12);
   
    ears(1.2,1.55,0,1,1,0.4);
    ears(-1.2, 1.55, 0, -1,1, 0.4);
    ears(1.2,1.1,0.2,1,-1,0.2);
    ears(-1.2, 1.1, 0.2, -1,-1, 0.2);
    
    //stomach/ears
    glColor3f(0.3, 0.2,0.1);
    sphere3(0, 0.2, 0.42, 0.3, 0.7, 0.1,0,11);
    glPushMatrix();
    glRotated(45,-1,0.5,-0.3);
    sphere3(0.5,0,0, 0.2,0.4,0.2,0,11);
    glPopMatrix();
    glPushMatrix();
    glRotated(45,-1,-0.5,0.3);
    sphere3(-0.5,0,0, 0.2,0.4,0.2,0,11);
    glPopMatrix();
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D,texture[11]);
    glBegin(GL_QUADS);
    
    glColor3f(1,1,1);
    glNormal3f(0,0,1);
    glTexCoord2f(0,0);glVertex3f(0.53,0.75, 0.9);//top left
    glTexCoord2f(1,0);glVertex3f(0.6,0.85,0.9);//top right
    glTexCoord2f(1,1);glVertex3f(0.8,0.5, 0.7);//bottom right
    glTexCoord2f(0,1);glVertex3f(0.6,0.5, 0.74);//bottom left
    glEnd();
    glBegin(GL_QUADS);
    //**this purposely clips into the face**
    glTexCoord2f(0,0);glVertex3f(0.6, 0.5, 0.74);
    glTexCoord2f(1,0); glVertex3f(0.8, 0.5, 0.7);
    glTexCoord2f(1,1);glVertex3f(0.53, 0.3, 0.6);
    glTexCoord2f(0,1);glVertex3f(0.6, 0.3, 0.6);
    glEnd();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);glVertex3f(-0.53,0.75, 0.9);//top left
    glTexCoord2f(1,0);glVertex3f(-0.6,0.85,0.9);//top right
    glTexCoord2f(1,1);glVertex3f(-0.8,0.5, 0.7);//bottom right
    glTexCoord2f(0,1);glVertex3f(-0.6,0.5, 0.74);//bottom left
    glEnd();
    
    glBegin(GL_QUADS);
    //**this purposely clips into the face**
    glTexCoord2f(0,0);glVertex3f(-0.6, 0.5, 0.74);
    glTexCoord2f(1,0);glVertex3f(-0.8, 0.5, 0.7);
    glTexCoord2f(1,1);glVertex3f(-0.53, 0.3, 0.6);
    glTexCoord2f(0,1);glVertex3f(-0.6, 0.3, 0.6);
    glEnd();
    
    glBegin(GL_QUADS);
    //eyebrows
    glTexCoord2f(0,0);glVertex3f(0.35, 1.15, 0.95); //left bottom
    glTexCoord2f(1,0);glVertex3f(0.45, 1.4, 0.86); //middle top
    glTexCoord2f(1,1);glVertex3f(0.7, 1.15, 0.84); //right bottom
    glTexCoord2f(0,1);glVertex3f(0.51, 1.25, 0.9); //middle bottom
    glEnd();
    
    glBegin(GL_QUADS);
    glTexCoord2f(0,0);glVertex3f(-0.35, 1.15, 0.95);
    glTexCoord2f(1,0); glVertex3f(-0.45, 1.4, 0.86);
    glTexCoord2f(1,1);glVertex3f(-0.7, 1.15, 0.84);
    glTexCoord2f(0,1);glVertex3f(-0.51, 1.25, 0.9);
    glEnd();
    glDisable(GL_TEXTURE_2D);
  
    //ear shadow
    glBegin(GL_QUADS);
    glColor3f(1,1,1);
    glVertex3f(1.1, 1.8, 0.44);
    glVertex3f(1.35, 1.65, 0.44);
    glVertex3f(1.42, 1.4, 0.44);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(1.55, 1.8, 0.44);
    
    glColor3f(1,1,1);
    glVertex3f(-1.1, 1.8, 0.44);
    glVertex3f(-1.35, 1.65, 0.44);
    glVertex3f(-1.42, 1.4, 0.44);
    glColor3f(0.2,0.2,0.2);
    glVertex3f(-1.55, 1.8, 0.44);
    
    //
  
    
    glEnd();
    
    glColor3f(0.3, 0.2, 0.1);
    sphere1(0.5, 0.9, 0.95, 0.1, 0.2, 0.1);//eye1
    sphere1(-0.5, 0.9, 0.95, 0.1, 0.2, 0.1);//eye2
    sphere1(0, 0.8, 1.15, 0.15, 0.1, 0.1);//nose
    //feet, feets creates walk cycle
    if(walking){
    sphere2(0.2,-0.5,0,0.2,0.5,0.2, feets);
        sphere2(-0.2,-0.5,0,0.2,0.5,0.2, -feets);}
    else{
        sphere2(0.2,-0.5,0,0.2,0.5,0.2, 0);
            sphere2(-0.2,-0.5,0,0.2,0.5,0.2, 0);
    }
    glPopMatrix();
    
}
static int onPlatform(platform ps){
    if((objX>(ps.x))&&(objX<(ps.x+ps.xstretch))&&(objZ>(ps.z-ps.zstretch))&&(objZ<(ps.z))){
       
            return 1;
        
    }
    return 0;
}
/*
 *  OpenGL (GLUT) calls this routine to display the scene
 */
void display()
{
   glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
   glEnable(GL_DEPTH_TEST);
   glLoadIdentity();
    
   if (mode==2)
   {
       double Ex = -2*dim*Sin(th)*Cos(ph);
       double Ey = +2*dim        *Sin(ph);
       double Ez = +2*dim*Cos(th)*Cos(ph);
      gluLookAt(Ex,Ey,Ez, objX ,jsum ,objZ, 0,Cos(ph),0);
   }
   //  Orthogonal - set world orientation

   else if(mode<2){
       
          double Ex = -2*10*Sin(th)*Cos(ph);
          double Ey = +2*10        *Sin(ph);
          double Ez = +2*10*Cos(th)*Cos(ph);
       gluLookAt(objX + Ex, Ey,objZ-Ez, objX,jsum ,objZ, 0,Cos(ph),0);
       
       //back wall, which will only be there in the case of first person
       glColor3f(1,1,1);
       glEnable(GL_TEXTURE_2D);
       glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
       glBindTexture(GL_TEXTURE_2D,texture[4]);
       glBegin(GL_QUADS);
       glTexCoord2f(0,0);glVertex3f(-50, -10, 50);
       glTexCoord2f(0,1);glVertex3f(-50, 50, 50);
       glTexCoord2f(1,1);glVertex3f(50, 50, 50);
       glTexCoord2f(1,0);glVertex3f(50, -10, 50);
       glEnd();
       glDisable(GL_TEXTURE_2D);
       
   }
    //LIGHTING
    glShadeModel(GL_SMOOTH);
    int local = 0;
    float Ambient[] = {0.05*ambient, 0.05*ambient, 0.05*ambient, 1.0};
    float Diffuse[] = {0.5, 0.2, 0, 1.0};
    float Specular[] = {0.01*spec, 0.01*spec, 0.01*spec, 1.0};
    float Position[]  = {distance*Cos(zh),ylight,distance*Sin(zh),1.0};
    
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER,local);
    glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0,GL_AMBIENT ,Ambient);
    glLightfv(GL_LIGHT0,GL_DIFFUSE ,Diffuse);
    glLightfv(GL_LIGHT0,GL_SPECULAR,Specular);
    glLightfv(GL_LIGHT0,GL_POSITION,Position);

    
    float Ambient2[] = {0.0, 0.0, 0.0, 1.0};
    float Diffuse2[] = {0.2, 0, 0.5, 1.0};
    float Specular2[] = {0.01*spec, 0.01*spec, 0.01*spec, 1.0};
    float Position2[]  = {distance*Cos(zh+180),ylight,distance*Sin(zh+180),1.0};
    
    
    glEnable(GL_LIGHT2);
    glLightfv(GL_LIGHT2,GL_AMBIENT ,Ambient2);
    glLightfv(GL_LIGHT2,GL_DIFFUSE ,Diffuse2);
    glLightfv(GL_LIGHT2,GL_SPECULAR,Specular2);
    glLightfv(GL_LIGHT2,GL_POSITION,Position2);

    
    if(obj==0){
        //floor
        glColor3f(0.2,0.3,0);
        for(int i = -50; i<50; i+=10){
            for(int j = -40; j<=50; j+=10){
                rectangle1(i,-5,j, 10, 2, 10, 0, 0, 0, 0, 9);
                
            }
            
        }
        
        //RIVER
        glColor3f(0.5,0.5,0.5);
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D,texture[10]);
        glBegin(GL_QUADS);
        glNormal3f(0,1,0);
        glTexCoord2f(0,0);glVertex3f(-50, -2.9, -14);
        glTexCoord2f(0,1);glVertex3f(-35, -2.9, -10);
        glTexCoord2f(1,1);glVertex3f(-35, -2.9, 10);
        glTexCoord2f(1,0);glVertex3f(-50, -2.9, 4);
        glEnd();
        
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);glVertex3f(-20, -2.9, 20);
        glTexCoord2f(0,1);glVertex3f(-35, -2.9, 10);
        glTexCoord2f(1,1);glVertex3f(-35, -2.9, -10);
        glTexCoord2f(1,0);glVertex3f(-20, -2.9, 4);
        glEnd();
        
        glBegin(GL_QUADS);
        glTexCoord2f(1,1);glVertex3f(-20, -2.9, 20);
        glTexCoord2f(1,0);glVertex3f(-10, -2.9, 35);
        glTexCoord2f(0,0);glVertex3f(-10, -2.9, 25);
        glTexCoord2f(0,1);glVertex3f(-20, -2.9, 4);
        glEnd();
        
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);glVertex3f(-10, -2.9, 40);
        glTexCoord2f(0,1);glVertex3f(-10, -2.9, 35);
        glTexCoord2f(1,1);glVertex3f(-10, -2.9, 25);
        glTexCoord2f(1,0);glVertex3f(-0, -2.9, 40);
        glEnd();
        
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);glVertex3f(-10, -2.9, 40);
        glTexCoord2f(0,1);glVertex3f(-17, -2.9, 45);
        glTexCoord2f(1,1);glVertex3f(-12, -2.9, 48);
        glTexCoord2f(1,0);glVertex3f(-0, -2.9, 40);
        glEnd();
        
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);glVertex3f(-25, -2.9, 50);
        glTexCoord2f(0,1);glVertex3f(-17, -2.9, 45);
        glTexCoord2f(1,1);glVertex3f(-12, -2.9, 48);
        glTexCoord2f(1,0);glVertex3f(-15, -2.9, 50);
        glEnd();
        glDisable(GL_TEXTURE_2D);
        
        
        
        glColor3f(1,1,1);
        redPanda(objX, jsum, objZ, 1, facing);
        //Drawing platforms from data structure
        for(int i = 0; i<MAX; i++){
            glColor3f(p[i].color,p[i].color,p[i].color);
            rectangle(p[i].x, p[i].y, p[i].z, p[i].xstretch, p[i].ystretch, p[i].zstretch, 0,0,0,0);
            
        }
        
        //Drawing all other items from data structure
        for(int i = 0; i<MAX_IT; i++){
            if(i<47) rock(items[i].x, items[i].y, items[i].z, items[i].scale, items[i].rot);
            else if (i<73) flowerbunch(items[i].x, items[i].y, items[i].z, items[i].scale, items[i].rot);
            else tree(items[i].x, items[i].y, items[i].z, items[i].scale, items[i].rot);
            
        }
        
        //HOUSE IN THE MIDDLE
        glColor3f(1,1,1);
        //support beams
        rectangle(5.4, -5.0, 0.4, 0.6, 15, 0.4, 0, 0, 0, 0);
        rectangle(0, -5.0, 0.4, 0.6, 15, 0.4, 0, 0, 0, 0);
        rectangle(5.4, -5.0, -6, 0.6, 15, 0.4, 0, 0, 0, 0);
        rectangle(0, -5.0, -6, 0.6, 15, 0.4, 0, 0, 0, 0);
        
        glColor3f(0.6,0.6,0.6);
        //horizontal supports
        rectangle(0.4, 1, 0.3, 5, 0.6, 0.3, 0, 0, 0, 0);
        rectangle(0.4, 1, -6.3, 5, 0.6, 0.3, 0, 0, 0, 0);
        rectangle(-0.2, 1, -0, 0.3, 0.6, 6, 0, 0, 0, 0);
        rectangle(6, 1, -0, 0.3, 0.6, 6, 0, 0, 0, 0);
        
        glColor3f(1,1,1);
        //roof
        rectangle(-1,8.7,1.5, 8, 0.4, 6, 45, 1, 0, 0);
        rectangle(-1,13,-3, 8, 0.4, 6, -45, 1, 0, 0);
       
        glColor3f(0.5, 0.5, 0.5);
        //top of roof
        rectangle(-1.5, 13, -2.6, 9, 0.4, 0.6, 0,0,0,0);
        
        glColor3f(0.2,0.2,0.2);
        //little roof supports
        rectangle(-0.2, 10, -0, 0.3, 0.6, 6, 0, 0, 0, 0);
        rectangle(6, 10, -00, 0.3, 0.6, 6, 0, 0, 0, 0);
        rectangle(0.2, 10.5, -2.6, 0.3, 2.6, 0.6, 0,0,0,0);
        rectangle(6, 10.5, -2.6, 0.3, 2.6, 0.6, 0,0,0,0);
        rectangle(0.2, 10.5, -2.6, 0.2, 1.6, 0.6, 45,0,0,0);
        rectangle(6, 10.5, -2.6, 0.2, 1.6, 0.6, 45,0,0,0);
        rectangle(0.2, 10.5, -2.6, 0.2, 2.3, 0.6, -45,0,0,0);
        rectangle(6, 10.5, -2.6, 0.2, 2.3, 0.6, -45,0,0,0);
    
        
        
        //Walls
        glColor3f(1,1,1);
        glEnable(GL_TEXTURE_2D);
        glTexEnvi(GL_TEXTURE_ENV , GL_TEXTURE_ENV_MODE , GL_MODULATE);
        glBindTexture(GL_TEXTURE_2D,texture[4]);
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);glVertex3f(50, -10, -50);
        glTexCoord2f(0,1);glVertex3f(50, 50, -50);
        glTexCoord2f(1,1);glVertex3f(50, 50, 50);
        glTexCoord2f(1,0);glVertex3f(50, -10, 50);
        glEnd();
        
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);glVertex3f(-50, -10, -50);
        glTexCoord2f(0,1);glVertex3f(-50, 50, -50);
        glTexCoord2f(1,1);glVertex3f(-50, 50, 50);
        glTexCoord2f(1,0);glVertex3f(-50, -10, 50);
        glEnd();
        
        glBegin(GL_QUADS);
        glTexCoord2f(0,0);glVertex3f(-50, -10, -50);
        glTexCoord2f(0,1);glVertex3f(-50, 50, -50);
        glTexCoord2f(1,1);glVertex3f(50, 50, -50);
        glTexCoord2f(1,0);glVertex3f(50, -10, -50);
        glEnd();
        
        glColor3f(1,0.8,0.8);
        
       
         
         glBindTexture(GL_TEXTURE_2D,texture[5]);
        glBegin(GL_QUADS);
        
        glTexCoord2f(0,0);glVertex3f(-50, 50, -50);
        glTexCoord2f(0,1);glVertex3f(-50, 50, 50);
        glTexCoord2f(1,1);glVertex3f(50, 50, 50);
        glTexCoord2f(1,0);glVertex3f(50, 50, -50);
        glEnd();
        
        
        glDisable(GL_TEXTURE_2D);
    }
    if(obj ==1){
        redPanda(0,-10,0,10, 0);
     
    }
    glColor3f(1,1,1);
   //walls for testing
   glDisable(GL_LIGHTING);
   glWindowPos2i(5,5);
    if(fmod(seconds, 60.0)<10){
        Print("%d:0%.1f, %d", minutes, fmod(seconds, 60.0), mode);
        
    }
    else{
        Print("%d:%.1f, %d", minutes, fmod(seconds, 60.0), mode);
    }
   ErrCheck("display");
   glFlush();
   glutSwapBuffers();
}



void idle()
{
    if(move){
   //  Elapsed time in seconds
   double t = glutGet(GLUT_ELAPSED_TIME)/1000.0;
   zh = fmod(25*t,360.0);
        seconds = t;
        minutes = (int)(seconds/60);
        for(int i = 0; i<MAX; i++){
            if(onPlatform(p[i])){
                current = i;
            }
            
        }
        if(lightDown){
            ambient-=0.1;
            if(ambient<=0) lightDown = 0;
        }
        else{
            ambient+=0.1;
            if(ambient>=10) lightDown = 1;
        }
        if(walking){
        if(walkState){
            feets += 10;
            if(feets>30){
                walkState = 0;
            }
        }
        else{
            feets -= 10;
            if(feets<-30){
                walkState = 1;
            }
        }}
        
        if(jumpState&&!jumpEnd){
            if(jsum<10){
                jsum+=1;
                
            }
            else{
                //jumpState = 0;
                jumpEnd = 1;
            }
        }
        else if(jumpEnd){
                if(onPlatform(p[current])){
                    if(jsum>((p[current].y + 2))){
                        jsum-=1;
                    }
                    else{
                        jumpEnd = 0;
                        jumpState = 0;
                    }
                }
            
    
            //{-40,-2,-30,4,0.3,4, 0}
        
            else if(jsum>-2){
                jsum-=1;
            }
            else{
                jumpEnd = 0;
                jumpState = 0;
            }
                
        }
        if(jsum>-2&&(onPlatform(p[current])==0)&&!jumpState){
            jumpEnd = 1;
        }
    }
        glutPostRedisplay();
        
    }


void special(int key,int x,int y)
{
   //  Right arrow key - increase angle by 5 degrees
    if (key == GLUT_KEY_RIGHT){
        th += 5;
        
    }
      
   //  Left arrow key - decrease angle by 5 degrees
    else if (key == GLUT_KEY_LEFT){
        th -= 5;
        
    }
   //  Up arrow key - move forwards
    else if (key == GLUT_KEY_UP){

            if(ph<90) ph+=5;
        
    }
    
   //  Down arrow key - move backwards
   else if (key == GLUT_KEY_DOWN){
      
           if(ph>35) ph-=5;
       
    }
   //  PageUp key - increase dim
   else if (key == GLUT_KEY_PAGE_UP)
      dim += 0.1;
   //  PageDown key - decrease dim
   else if (key == GLUT_KEY_PAGE_DOWN && dim>1)
      dim -= 0.1;
   //  Keep angles to +/-360 degrees
   th %= 360;
    if(mode==2){
        if(ph>=75) ph = 75;
        else if (ph<=-60) ph = -60;
        
    }
   ph %= 360;
   //  Update projection
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}

/*
 *  GLUT calls this routine when a key is pressed
 */

void key(unsigned char ch,int x,int y)
{
   //  Exit on ESC
   if (ch == 27)
      exit(0);
   //  Reset view angle
   else if (ch == 'r') reset();
   
   else if ((ch == 'm')|| (ch == 'M')){
       mode++;
    if (mode==3) {
        mode = 1;
    }
       reset();
   }
    
   else if ((ch == 'w')||(ch == 'W')){
       //walk "forward" from camera
       if(mode==1){
           objX+=Sin(th);
           objZ+=Cos(th);
           facing = th;
           walking = 1;}
       else{
           objZ--;
           facing = 180;
           walking = 1;
       }
            
       
   }
   else if ((ch == 's')||(ch == 'S')){
       //walk "towards" camera
       if(mode==1){
       objX-=Sin(th);
       objZ-=Cos(th);
       facing = th+180;
       walking = 1;
       }
       else{
           objZ++;
           facing = 0;
           walking = 1;
       }
       
   }
   else if ((ch == 'a')||(ch == 'A')){
       //walk to "left"
       if(mode==1){
        objX+=Cos(th);
        objZ-=Sin(th);
        facing = th+90;
           walking = 1;
           
       }
       else{
           objX--;
           facing = -90;
           walking = 1;
       }
       
   }
   else if (ch == 'd'||ch == 'D'){
       
           //walk to "right"
       if(mode ==1){
       objX-=Cos(th);
       objZ+=Sin(th);
    facing = th-90;
           walking = 1;
           
       }
       else{
           objX++;
           facing = 90;
           walking = 1;
       }
       
   }
   else if((ch == 'j')||(ch=='J')){
       //jump
       if(jsum<10&&!jumpEnd){
       jumpState = 1;
       jumpEnd = 0;
       
       }
   }
   
  
    
   
   //  Change field of view angle
   else if (ch == '-' && ch>1)
      fov++;
   else if (ch == '+' && ch<179)
      fov--;
   else if(ch == 'o'||ch == 'O'){
       obj++;
       if(obj>1) obj = 0;
   }
   //  Reproject
   Project();
   //  Tell GLUT it is necessary to redisplay the scene
   glutPostRedisplay();
}
void keyUp(unsigned char ch,int x,int y){
    //if you let up a key you've pressed
    if(ch=='j'){
        //fall
        jumpState = 0;
        jumpEnd = 1;
        //fall();
    }
    else if(ch=='w'||ch=='a'||ch=='s'||ch=='d'){
        //stop walking
        walking=0;
        //fall();
    }
    glutPostRedisplay();
}

/*
 *  GLUT calls this routine when the window is resized
 */
void reshape(int width,int height)
{
   //  Ratio of the width to the height of the window
   asp = (height>0) ? (double)width/height : 1;
   //  Set the viewport to the entire window
   glViewport(0,0, RES*width,RES*height);
   //  Set projection
   Project();
}

/*
 *  Start up GLUT and tell it what to do
 */
int main(int argc,char* argv[])
{
   //  Initialize GLUT
   glutInit(&argc,argv);
   //  Request double buffered, true color window with Z buffering at 600x600
   glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH | GLUT_DOUBLE);
   glutInitWindowSize(600,600);
   glutCreateWindow("Kate McCarthy Final Project");
#ifdef USEGLEW
   //  Initialize GLEW
   if (glewInit()!=GLEW_OK) Fatal("Error initializing GLEW\n");
#endif
   //  Set callbacks
   glutDisplayFunc(display);
   glutReshapeFunc(reshape);
   glutSpecialFunc(special);
   glutKeyboardFunc(key);
    glutKeyboardUpFunc(keyUp);
    glutIdleFunc(move?idle:NULL);
   //textures
    texture[0] = LoadTexBMP("wood.bmp");
    texture[1] = LoadTexBMP("barktex.bmp");
    texture[2] = LoadTexBMP("foresttex.bmp");
    texture[3] = LoadTexBMP("stonetex.bmp");
    texture[4] = LoadTexBMP("back.bmp");
    texture[5] = LoadTexBMP("sky.bmp");
    texture[6] = LoadTexBMP("cartoontree.bmp");
    texture[7] = LoadTexBMP("sidetree2.bmp");
    texture[8] = LoadTexBMP("leaftake1.bmp");
    texture[9] = LoadTexBMP("grass.bmp");
    texture[10] = LoadTexBMP("river.bmp");
    texture[11] = LoadTexBMP("cartoonfur1.bmp");
    texture[12]= LoadTexBMP("tail.bmp");
   glutMainLoop();
    
   return 0;
}
