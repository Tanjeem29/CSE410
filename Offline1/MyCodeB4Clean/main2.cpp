#include <windows.h>  // for MS Windows
#include <GL/glut.h>  // GLUT, include glu.h and gl.h
#include <cmath>

#include<iostream>
#include<stdio.h>
#include<stdlib.h>
using namespace std;

#define phi (70.53 * M_PI / 180)

double  pX_x=1, pX_y=0, pX_z=0,
        pY_x=0, pY_y=1, pY_z=0,
        pZ_x=0, pZ_y=0, pZ_z=1;

const int subdivisions = 5;
// #define subdivisions (5);
const int numRows = (int)pow(2,subdivisions) + 1;

double sphereVertices[3][40][40];
int objAngle = 0;
int num_moves = 16;
double inc_1 = (1.0/3 - 1)/num_moves;
double inc_2 = (1.0/3 - 0)/num_moves;
double cylinderLength = sqrt((pX_x-pY_x)*(pX_x-pY_x) + (pX_y-pY_y)*(pX_y-pY_y) + (pX_z-pY_z)*(pX_z-pY_z));
double cylinderRadius = pX_z / sin(phi/2);
// double sphereCenter = 1.0;
// double sphereRadius = 1/sqrt(3);
// double sphereCenter_inc = (1.0-0)/num_moves;
// double sphereRadius_inc = (0-1/sqrt(3))/num_moves;
double sphereCenter = pX_x - pX_z;
double sphereRadius = sqrt(3) * pX_z;


double camMoveSmooth = 0.25;

/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

// Global variables
GLfloat eyex = 4, eyey = 4, eyez = 4;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;
bool isAxes = true, isCube = false, isPyramid = false;


double eyeat_x = 4, eyeat_y = 4, eyeat_z = 4;
double lookdir_x=-1/sqrt(3), lookdir_y =-1/sqrt(3), lookdir_z= -1/sqrt(3);
double lookright_x = 0.707, lookright_y = 0, lookright_z = -0.707;
double lookup_x = -1/sqrt(6), lookup_y = 2/sqrt(6), lookup_z = -1/sqrt(6);
double camRotateRate = 0.01;


/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxes() {
    glLineWidth(3);
    glBegin(GL_LINES);
        glColor3f(1,0,0);   // Red
        // X axis
        glVertex3f(0,0,0);
        glVertex3f(1,0,0);

        glColor3f(0,1,0);   // Green
        // Y axis
        glVertex3f(0,0,0);
        glVertex3f(0,1,0);

        glColor3f(0,0,1);   // Blue
        // Z axis
        glVertex3f(0,0,0);
        glVertex3f(0,0,1);
    glEnd();
}

/* Draw a cube centered at the origin */
void drawCube() {
    glBegin(GL_QUADS);                // Begin drawing the color cube with 6 quads
        // Top face (y = 1.0f)
        // Define vertices in counter-clockwise (CCW) order with normal pointing out
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f( 1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f, -1.0f);
        glVertex3f(-1.0f, 1.0f,  1.0f);
        glVertex3f( 1.0f, 1.0f,  1.0f);

        // Bottom face (y = -1.0f)
        glColor3f(1.0f, 0.5f, 0.0f);     // Orange
        glVertex3f( 1.0f, -1.0f,  1.0f);
        glVertex3f(-1.0f, -1.0f,  1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f( 1.0f, -1.0f, -1.0f);

        // Front face  (z = 1.0f)
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f( 1.0f,  1.0f, 1.0f);
        glVertex3f(-1.0f,  1.0f, 1.0f);
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glVertex3f( 1.0f, -1.0f, 1.0f);

        // Back face (z = -1.0f)
        glColor3f(1.0f, 1.0f, 0.0f);     // Yellow
        glVertex3f( 1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);
        glVertex3f( 1.0f,  1.0f, -1.0f);

        // Left face (x = -1.0f)
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(-1.0f,  1.0f,  1.0f);
        glVertex3f(-1.0f,  1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f, -1.0f);
        glVertex3f(-1.0f, -1.0f,  1.0f);

        // Right face (x = 1.0f)
        glColor3f(1.0f, 0.0f, 1.0f);     // Magenta
        glVertex3f(1.0f,  1.0f, -1.0f);
        glVertex3f(1.0f,  1.0f,  1.0f);
        glVertex3f(1.0f, -1.0f,  1.0f);
        glVertex3f(1.0f, -1.0f, -1.0f);
    glEnd();  // End of drawing color-cube
}

/* Draw a pyramid centered at the origin */
void drawPyramid() {
    glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
        // Front
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f(-1.0f, -1.0f, 1.0f);
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(1.0f, -1.0f, 1.0f);

        // Right
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f(0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(1.0f, -1.0f, 1.0f);
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f(1.0f, -1.0f, -1.0f);

        // Back
        glColor3f(1.0f, 0.0f, 0.0f);     // Red
        glVertex3f(0.0f, 1.0f, 0.0f);
        glColor3f(0.0f, 1.0f, 0.0f);     // Green
        glVertex3f(1.0f, -1.0f, -1.0f);
        glColor3f(0.0f, 0.0f, 1.0f);     // Blue
        glVertex3f(-1.0f, -1.0f, -1.0f);

        // Left
        glColor3f(1.0f,0.0f,0.0f);       // Red
        glVertex3f( 0.0f, 1.0f, 0.0f);
        glColor3f(0.0f,0.0f,1.0f);       // Blue
        glVertex3f(-1.0f,-1.0f,-1.0f);
        glColor3f(0.0f,1.0f,0.0f);       // Green
        glVertex3f(-1.0f,-1.0f, 1.0f);
    glEnd();   // Done drawing the pyramid
}


void drawOctahedronFace(){
    glBegin(GL_TRIANGLES);           // Begin drawing the pyramid with 4 triangles
        // Front
        glVertex3f( pX_x, pX_y, pX_z);
        glVertex3f( pY_x, pY_y, pY_z);
        glVertex3f( pZ_x, pZ_y, pZ_z);
    glEnd();   // Done drawing the pyramid
}

void drawOctahedron(){
    glColor3f(0,1,1);
    drawOctahedronFace();
    glPushMatrix();
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        drawOctahedronFace();
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        drawOctahedronFace();
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        drawOctahedronFace();
    glPopMatrix();

    glColor3f(1,0,1);
    
    glPushMatrix();
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        drawOctahedronFace();
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        drawOctahedronFace();
        glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
        drawOctahedronFace();
        glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
        drawOctahedronFace();
    glPopMatrix();

    // printf("%lf %lf", inc_1, inc_2);

}


void O2S(){
    // printf("OOOO");
    if(pX_x + inc_1 - 1.0 /3 <= 0){
        // printf("%lf\n", pX_x);
        pX_x = pX_y = pX_z = pY_x = pY_y = pY_z = pZ_x = pZ_y = pZ_z = 1.0/3;
        // printf("Ohnoh\n");
        // printf("-----%lf\n", pX_x);
    }
    else{
        // printf("%lf\n", pX_x);
        pX_x +=inc_1;
        pY_y +=inc_1;
        pZ_z +=inc_1;

        pX_y+=inc_2;
        pX_z+=inc_2;
        pY_x+=inc_2;
        pY_z+=inc_2;
        pZ_x+=inc_2;
        pZ_y+=inc_2;
    }
    cylinderLength = sqrt((pX_x-pY_x)*(pX_x-pY_x) + (pX_y-pY_y)*(pX_y-pY_y) + (pX_z-pY_z)*(pX_z-pY_z));
    cylinderRadius = pX_z / sin(phi/2);
    sphereCenter = pX_x - pX_z;
    sphereRadius = sqrt(3) * pX_z;
    // sphereCenter += sphereCenter_inc;
    // sphereRadius += sphereRadius_inc;
}

void S2O(){
    if(pX_x - inc_1 - 1.0  > 0){
        // printf("Ohnoh2\n");
        // printf("%lf\n", pX_x);
        pX_y = pX_z = pY_x  = pY_z = pZ_x = pZ_y =  0;
        pX_x = pY_y = pZ_z = 1;
    }
    else{
        // printf("%lf\n", pX_x);
        pX_x -=inc_1;
        pY_y -=inc_1;
        pZ_z -=inc_1;

        pX_y-=inc_2;
        pX_z-=inc_2;
        pY_x-=inc_2;
        pY_z-=inc_2;
        pZ_x-=inc_2;
        pZ_y-=inc_2;
    }
    cylinderLength = sqrt((pX_x-pY_x)*(pX_x-pY_x) + (pX_y-pY_y)*(pX_y-pY_y) + (pX_z-pY_z)*(pX_z-pY_z));
    cylinderRadius = pX_z / sin(phi/2);
    // sphereCenter -= sphereCenter_inc;
    // sphereRadius -= sphereRadius_inc;
    sphereCenter = pX_x - pX_z;
    sphereRadius = sqrt(3) * pX_z;
}

void drawCylinder(double height, double radius, int segments) {
    double tempx = radius, tempy = 0;
    double currx, curry;
    glBegin(GL_QUADS);
        for (int i = 1; i <= segments; i++) {
            double theta = i * 2.0 * M_PI / segments;
            currx = radius * cos(theta);
            curry = radius * sin(theta);

            GLfloat c = (2+cos(theta))/3;
            // glColor3f(c,c,c);
            glVertex3f(currx, curry, height/2);
            glVertex3f(currx, curry, -height/2);

            glVertex3f(tempx, tempy, -height/2);
            glVertex3f(tempx, tempy, height/2);

            tempx = currx;
            tempy = curry;
        }
    glEnd();
}

void drawCylinderEdge(){
    double YZmp_x = (pY_x + pZ_x)/2;
    double YZmp_y = (pY_y + pZ_y)/2;
    double YZmp_z = (pY_z + pZ_z)/2;
    double temp_x = YZmp_x;
    double temp_y = YZmp_y * cos(M_PI/4) - YZmp_z * sin(M_PI/4);
    double temp_z = YZmp_y * cos(M_PI/4) + YZmp_z * sin(M_PI/4);
    double tr_x = 0;
    double tr_y = 0;
    double tr_z = temp_z - cylinderRadius * cos(phi/2);
    // printf("%lf, %lf, %lf\n", YZmp_x,YZmp_y,YZmp_z);
    // printf("%lf, %lf, %lf\n", temp_x,temp_y,temp_z);
    // glBegin(GL_LINE_LOOP);
    //     glColor3f(1,0,0);   // Red
    //     // X axis
    //     glVertex3f(YZmp_x,YZmp_y,YZmp_z);
    //     glColor3f(0,1,0);   // Green
    //     glVertex3f(temp_x,temp_y,temp_z);
    //     glVertex3f(tr_x,tr_y,tr_z);

    //     // glColor3f(0,1,0);   // Green
    //     // // Y axis
    //     // glVertex3f(0,0,0);
    //     // glVertex3f(0,1,0);

    //     // glColor3f(0,0,1);   // Blue
    //     // // Z axis
    //     // glVertex3f(0,0,0);
    //     // glVertex3f(0,0,1);
    // glEnd();

    
    glPushMatrix();
        glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(tr_x,tr_y, tr_z);
        // glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        
        
        drawCylinder(cylinderLength, cylinderRadius, 90);
        // glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glPopMatrix();
    //     glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    //     drawOctahedronFace();
    //     glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    //     drawOctahedronFace();
    //     glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    //     drawOctahedronFace();
    // glPopMatrix();
}

void drawAllEdges(){
    glPushMatrix();
        drawCylinderEdge();
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        drawCylinderEdge();
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        drawCylinderEdge();
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        drawCylinderEdge();
    glPopMatrix();
    glPushMatrix();
        glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
        drawCylinderEdge();
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f); //axes also shifted, -x -> y, y -> x 
        drawCylinderEdge();
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f); //axes also shifted, -x -> y, y -> x 
        drawCylinderEdge();
        glRotatef(90.0f, 1.0f, 0.0f, 0.0f); //axes also shifted, -x -> y, y -> x 
        drawCylinderEdge();
    glPopMatrix();
    glPushMatrix();
        glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
        drawCylinderEdge();
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        drawCylinderEdge();
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        drawCylinderEdge();
        glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
        drawCylinderEdge();
    glPopMatrix();
}

void calcVertices(){
    double n1[3];
    double n2[3];
    double v[3];
    double a1;
    double a2;
    // printf("%d, %d\n", numRows);
    for(int i =0; i<numRows; i++){
        a2 = (M_PI /180.0)*(45.0 - 90.0 * i/(numRows-1));
        n2[0] = -sin(a2);
        n2[1] = cos(a2);
        n2[2] = 0;
        for(int j = 0; j<numRows;j++){
            a1 = (M_PI /180.0)*(-45.0 + 90.0 * j/(numRows-1));
            n1[0] = -sin(a1);
            n1[1] = 0;
            n1[2] = -cos(a1);

            v[0] = n1[1] * n2[2] - n1[2] * n2[1];
            v[1] = n1[2] * n2[0] - n1[0] * n2[2];
            v[2] = n1[0] * n2[1] - n1[1] * n2[0];

            double scale = 1 / sqrt(v[0]*v[0] + v[1]*v[1] + v[2]*v[2]);
            v[0] *= scale;
            v[1] *= scale;
            v[2] *= scale;

            sphereVertices[0][i][j] = v[0];
            sphereVertices[1][i][j] = v[1];
            sphereVertices[2][i][j] = v[2];
            // printf("%d %d %lf %lf %lf\n",i,j,  v[0],v[1],v[2]);

            // printf("%d %d %lf %lf %lf\n",i,j, sphereRadius* sphereVertices[0][i][j],sphereRadius* sphereVertices[1][i][j],sphereRadius* sphereVertices[2][i][j]);
                        // printf("%d %d %lf %lf %lf\n",i,j,  sphereVertices[0][i][j], sphereVertices[1][i][j], sphereVertices[2][i][j]);

        }
    }
}

void drawSpherePart(){
    glPushMatrix();
    glTranslatef(sphereCenter, 0, 0);
    glBegin(GL_QUADS);           // Begin drawing the pyramid with 4 triangles
    for(int i=0;i<numRows-1;i++){
        for(int j=0;j<numRows-1;j++){
                // glVertex3f(0,0,1);
                // glVertex3f(0,1,1);
                // glVertex3f(0,-1,1);
                // glVertex3f(1,0,1);
                // Front
                // printf("%lf, %lf, %lf\n", sphereRadius* sphereVertices[0][i][j],sphereRadius* sphereVertices[1][i][j],sphereRadius* sphereVertices[2][i][j]);
                glVertex3f( sphereRadius* sphereVertices[0][i][j],sphereRadius* sphereVertices[1][i][j],sphereRadius* sphereVertices[2][i][j]);
                glVertex3f( sphereRadius*sphereVertices[0][i+1][j], sphereRadius*sphereVertices[1][i+1][j], sphereRadius*sphereVertices[2][i+1][j]);
                glVertex3f( sphereRadius*sphereVertices[0][i+1][j+1], sphereRadius*sphereVertices[1][i+1][j+1], sphereRadius*sphereVertices[2][i+1][j+1]);
                glVertex3f( sphereRadius*sphereVertices[0][i][j+1], sphereRadius*sphereVertices[1][i][j+1], sphereRadius*sphereVertices[2][i][j+1]);

                // glVertex3f( sphereVertices[0][i][j],sphereVertices[1][i][j],sphereVertices[2][i][j]);
                // glVertex3f( sphereVertices[0][i+1][j], sphereVertices[1][i+1][j], sphereVertices[2][i+1][j]);
                // glVertex3f( sphereVertices[0][i+1][j+1], sphereVertices[1][i+1][j+1], sphereVertices[2][i+1][j+1]);
                // glVertex3f( sphereVertices[0][i][j+1], sphereVertices[1][i][j+1], sphereVertices[2][i][j+1]);

        }
    }
    glEnd();   // Done drawing the pyramid
    glPopMatrix();
}

void drawCorners(){
    glColor3f(1,0,0);
    drawSpherePart();
    glPushMatrix();
        glRotatef(180,0,1.0,0);
        drawSpherePart();
    glPopMatrix();

    glColor3f(0,1,0);
    glPushMatrix();
        glRotatef(90,0,1.0,0);
        drawSpherePart();
        glRotatef(180,0,1.0,0);
        drawSpherePart();
    glPopMatrix();

    glColor3f(0,0,1);
    glPushMatrix();
        glRotatef(90,0,0,1.0);
        drawSpherePart();
        glRotatef(180,0,0,1.0);
        drawSpherePart();
    glPopMatrix();


}

/*  Handler for window-repaint event. Call back when the window first appears and
    whenever the window needs to be re-painted. */
void display() {
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
    glLoadIdentity();                       // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    // control viewing (or camera)
    // gluLookAt(eyex,eyey,eyez,
    //           centerx,centery,centerz,
    //           upx,upy,upz);
    gluLookAt(eyeat_x,eyeat_y,eyeat_z,
              eyeat_x + lookdir_x,eyeat_y + lookdir_y,eyeat_z + lookdir_z,
              lookup_x,lookup_y,lookup_z);
    // draw
    if (isAxes) drawAxes();
    if (isCube) drawCube();
    if (isPyramid) drawPyramid();

    
    // drawCylinder(cylinderLength,cylinderRadius,90);
    // drawCylinderEdge();


    glRotatef(objAngle, 0,1,0);
    drawOctahedron();
    glColor3f(1,1,0);
    drawAllEdges();
    glColor3f(1,0,0);
    drawCorners();
    // drawSpherePart();
    glutSwapBuffers();  // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshapeListener(GLsizei width, GLsizei height) {  // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset the projection matrix
    /*if (width >= height) {
        // aspect >= 1, set the height from -1 to 1, with larger width
        gluOrtho2D(-1.0 * aspect, 1.0 * aspect, -1.0, 1.0);
    } else {
        // aspect < 1, set the width to -1 to 1, with larger height
        gluOrtho2D(-1.0, 1.0, -1.0 / aspect, 1.0 / aspect);
    }*/
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    float v = 0.1;
    switch (key) {
    // Control eye (location of the eye)
    // look left/right, so change only lookright and lookdir vectors
    case '1':
        lookright_x = lookright_x * cos(camRotateRate) + lookdir_x * sin(camRotateRate);
        lookright_y = lookright_y * cos(camRotateRate) + lookdir_y * sin(camRotateRate);
        lookright_z = lookright_z * cos(camRotateRate) + lookdir_z * sin(camRotateRate);

        lookdir_x = - lookright_x * sin(camRotateRate) + lookdir_x * cos(camRotateRate);
        lookdir_y = - lookright_y * sin(camRotateRate) + lookdir_y * cos(camRotateRate);
        lookdir_z = - lookright_z * sin(camRotateRate) + lookdir_z * cos(camRotateRate);        
        break;
    case '2':
        lookright_x = lookright_x * cos(-camRotateRate) + lookdir_x * sin(-camRotateRate);
        lookright_y = lookright_y * cos(-camRotateRate) + lookdir_y * sin(-camRotateRate);
        lookright_z = lookright_z * cos(-camRotateRate) + lookdir_z * sin(-camRotateRate);

        lookdir_x = - lookright_x * sin(-camRotateRate) + lookdir_x * cos(-camRotateRate);
        lookdir_y = - lookright_y * sin(-camRotateRate) + lookdir_y * cos(-camRotateRate);
        lookdir_z = - lookright_z * sin(-camRotateRate) + lookdir_z * cos(-camRotateRate);

        break;
    // Look up/down, so change only lookdir and lookup vectors
    case '3':
        lookdir_x = lookdir_x * cos(camRotateRate) + lookup_x * sin(camRotateRate);
        lookdir_y = lookdir_y * cos(camRotateRate) + lookup_y * sin(camRotateRate);
        lookdir_z = lookdir_z * cos(camRotateRate) + lookup_z * sin(camRotateRate);

        lookup_x = - lookdir_x * sin(camRotateRate) + lookup_x * cos(camRotateRate);
        lookup_y = - lookdir_y * sin(camRotateRate) + lookup_y * cos(camRotateRate);
        lookup_z = - lookdir_z * sin(camRotateRate) + lookup_z * cos(camRotateRate);
        break;
    case '4':
        lookdir_x = lookdir_x * cos(-camRotateRate) + lookup_x * sin(-camRotateRate);
        lookdir_y = lookdir_y * cos(-camRotateRate) + lookup_y * sin(-camRotateRate);
        lookdir_z = lookdir_z * cos(-camRotateRate) + lookup_z * sin(-camRotateRate);

        lookup_x = - lookdir_x * sin(-camRotateRate) + lookup_x * cos(-camRotateRate);
        lookup_y = - lookdir_y * sin(-camRotateRate) + lookup_y * cos(-camRotateRate);
        lookup_z = - lookdir_z * sin(-camRotateRate) + lookup_z * cos(-camRotateRate);
        break;
    // tilt anticlock/clock, so change only lookup and lookright vectors
    case '5':
        lookup_x = lookup_x * cos(camRotateRate) + lookright_x * sin(camRotateRate);
        lookup_y = lookup_y * cos(camRotateRate) + lookright_y * sin(camRotateRate);
        lookup_z = lookup_z * cos(camRotateRate) + lookright_z * sin(camRotateRate);

        lookright_x = - lookup_x * sin(camRotateRate) + lookright_x * cos(camRotateRate);
        lookright_y = - lookup_y * sin(camRotateRate) + lookright_y * cos(camRotateRate);
        lookright_z = - lookup_z * sin(camRotateRate) + lookright_z * cos(camRotateRate);
        break;
    case '6':
        lookup_x = lookup_x * cos(-camRotateRate) + lookright_x * sin(-camRotateRate);
        lookup_y = lookup_y * cos(-camRotateRate) + lookright_y * sin(-camRotateRate);
        lookup_z = lookup_z * cos(-camRotateRate) + lookright_z * sin(-camRotateRate);

        lookright_x = - lookup_x * sin(-camRotateRate) + lookright_x * cos(-camRotateRate);
        lookright_y = - lookup_y * sin(-camRotateRate) + lookright_y * cos(-camRotateRate);
        lookright_z = - lookup_z * sin(-camRotateRate) + lookright_z * cos(-camRotateRate);
        break;
    

    // Control center (location where the eye is looking at)
    // control centerx
    case 'q':
        centerx += v;
        break;
    case 'w':
        centerx -= v;
        break;
    // control centery
    case 'e':
        centery += v;
        break;
    case 'r':
        centery -= v;
        break;
    // control centerz
    case 't':
        centerz += v;
        break;
    case 'y':
        centerz -= v;
        break;

    // Control what is shown
    // case 'a':
    //     isAxes = !isAxes;   // show/hide Axes if 'a' is pressed
    //     break;
    // case 'c':
    //     isCube = !isCube;   // show/hide Cube if 'c' is pressed
    //     break;
    case 'a':
        objAngle-=10;   // show/hide Axes if 'a' is pressed
        objAngle%=360;
        break;
    case 'd':
        objAngle+=10;   // show/hide Cube if 'c' is pressed
        objAngle%=360;
        break;
    case 'p':
        isPyramid = !isPyramid; // show/hide Pyramid if 'p' is pressed
        break;
    case ',':
        O2S();
        break;
    case '.':
        S2O();
        break;

    // Control exit
    case 27:    // ESC key
        exit(0);    // Exit window
        break;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x,int y) {
    double v = 0.25;
    double lx = centerx - eyex;
    double lz = centerz - eyez;
    double s;
    switch (key) {
    // case GLUT_KEY_LEFT:
    //     eyex += v * (upy*lz);
    //     eyez += v * (-lx*upy);
    //     s = sqrt(eyex*eyex + eyez*eyez) / (4 * sqrt(2));
    //     eyex /= s;
    //     eyez /= s;
    //     break;
    // case GLUT_KEY_RIGHT:
    //     eyex += v * (-upy*lz);
    //     eyez += v * (lx*upy);
    //     s = sqrt(eyex*eyex + eyez*eyez) / (4 * sqrt(2));
    //     eyex /= s;
    //     eyez /= s;
    //     break;
    // case GLUT_KEY_UP:
    //     eyey += v;
    //     break;
    // case GLUT_KEY_DOWN:
    //     eyey -= v;
    //     break;
    case GLUT_KEY_LEFT:
        // centerx+=v;
        // eyex+=v;
        // centerz-=v;
        // eyez-=v;
        eyeat_x -= lookright_x *camMoveSmooth;
        eyeat_y -= lookright_y *camMoveSmooth;
        eyeat_z -= lookright_z *camMoveSmooth;
        break;
    case GLUT_KEY_RIGHT:
        // centerx-=v;
        // eyex-=v;
        // centerz+=v;
        // eyez+=v;
        eyeat_x += lookright_x *camMoveSmooth;
        eyeat_y += lookright_y *camMoveSmooth;
        eyeat_z += lookright_z *camMoveSmooth;
        break;
    case GLUT_KEY_UP:
        // centerx-=v;
        // eyex-=v;
        // centerz-=v;
        // eyez-=v;
        // centery-=v;
        // eyey-=v;
        eyeat_x += lookdir_x *camMoveSmooth;
        eyeat_y += lookdir_y *camMoveSmooth;
        eyeat_z += lookdir_z *camMoveSmooth;
        break;
    case GLUT_KEY_DOWN:
        // centerx+=v;
        // eyex+=v;
        // centerz+=v;
        // eyez+=v;
        // centery+=v;
        // eyey+=v;
        eyeat_x -= lookdir_x *camMoveSmooth;
        eyeat_y -= lookdir_y *camMoveSmooth;
        eyeat_z -= lookdir_z *camMoveSmooth;
        break;
    case GLUT_KEY_PAGE_DOWN:
        // centery -= v;
        eyeat_x-=lookup_x * camMoveSmooth;
        eyeat_y-=lookup_y * camMoveSmooth;
        eyeat_z-=lookup_z * camMoveSmooth;

        break;
    case GLUT_KEY_PAGE_UP:
        eyeat_x+=lookup_x * camMoveSmooth;
        eyeat_y+=lookup_y * camMoveSmooth;
        eyeat_z+=lookup_z * camMoveSmooth;
        // centery += v;
        break;
    // case GLUT_KEY_COMMA:
    //     eyey -= v;
    //     break;
    
    default:
        return;
    }
    glutPostRedisplay();    // Post a paint request to activate display()
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char** argv) {
    glutInit(&argc, argv);                      // Initialize GLUT
    glutInitWindowSize(640, 640);               // Set the window's initial width & height
    glutInitWindowPosition(50, 50);             // Position the window's initial top-left corner
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing");      // Create a window with the given title
    calcVertices();
    glutDisplayFunc(display);                   // Register display callback handler for window re-paint
    glutReshapeFunc(reshapeListener);           // Register callback handler for window re-shape
    glutKeyboardFunc(keyboardListener);         // Register callback handler for normal-key event
    glutSpecialFunc(specialKeyListener);        // Register callback handler for special-key event
    initGL();                                   // Our own OpenGL initialization
    glutMainLoop();                             // Enter the event-processing loop
    return 0;
}
