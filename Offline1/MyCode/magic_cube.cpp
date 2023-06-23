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

double sphereCenter = pX_x - pX_z;
double sphereRadius = sqrt(3) * pX_z;


double camMoveSmooth = 0.25;
double noRefChange = 0.25;
double dir_x, dir_y, dir_z, dirNorm, left_x, left_y, left_z, updir_x, updir_y, updir_z, updirNorm;
double t1_x,t1_y,t1_z, t2_x, t2_y, t2_z, t2_dot;


/* Initialize OpenGL Graphics */
void initGL() {
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);   // Black and opaque
    glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
}

// Global variables
// GLfloat eyex = 4, eyey = 4, eyez = 4;
// GLfloat centerx = 0, centery = 0, centerz = 0;
// GLfloat upx = 0, upy = 1, upz = 0;
// bool isAxes = true;


double eyeat_x = 2.5, eyeat_y = 2.5, eyeat_z = 2.5;
double lookdir_x=-1/sqrt(3), lookdir_y =-1/sqrt(3), lookdir_z= -1/sqrt(3);
double lookright_x = 0.707, lookright_y = 0, lookright_z = -0.707;
double lookup_x = -1/sqrt(6), lookup_y = 2/sqrt(6), lookup_z = -1/sqrt(6);
double camRotateRate = 0.01;


double lookat_x =0, lookat_y = 0, lookat_z = 0;
double up_y = 1, up_x =0, up_z = 0;


/* Draw axes: X in Red, Y in Green and Z in Blue */
// void drawAxes() {
//     glLineWidth(3);
//     glBegin(GL_LINES);
//         glColor3f(1,0,0);   // Red
//         // X axis
//         glVertex3f(0,0,0);
//         glVertex3f(1,0,0);

//         glColor3f(0,1,0);   // Green
//         // Y axis
//         glVertex3f(0,0,0);
//         glVertex3f(0,1,0);

//         glColor3f(0,0,1);   // Blue
//         // Z axis
//         glVertex3f(0,0,0);
//         glVertex3f(0,0,1);
//     glEnd();
// }


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


    
    glPushMatrix();
        glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);
        glTranslatef(tr_x,tr_y, tr_z);
        // glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
        glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
        
        
        drawCylinder(cylinderLength, cylinderRadius, 90);
        // glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
    glPopMatrix();

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
    double norm1[3];
    double norm2[3];
    double tempVertex[3];
    double latAng;
    double longAng;

    for(int i =0; i<numRows; i++){
        longAng = (M_PI /180.0)*(45.0 - 90.0 * i/(numRows-1));
        norm2[0] = -sin(longAng);
        norm2[1] = cos(longAng);
        norm2[2] = 0;
        for(int j = 0; j<numRows;j++){
            latAng = (M_PI /180.0)*(-45.0 + 90.0 * j/(numRows-1));
            norm1[0] = -sin(latAng);
            norm1[1] = 0;
            norm1[2] = -cos(latAng);

            tempVertex[0] = norm1[1] * norm2[2] - norm1[2] * norm2[1];
            tempVertex[1] = norm1[2] * norm2[0] - norm1[0] * norm2[2];
            tempVertex[2] = norm1[0] * norm2[1] - norm1[1] * norm2[0];

            double scale = 1 / sqrt(tempVertex[0]*tempVertex[0] + tempVertex[1]*tempVertex[1] + tempVertex[2]*tempVertex[2]);
            tempVertex[0] *= scale;
            tempVertex[1] *= scale;
            tempVertex[2] *= scale;

            sphereVertices[0][i][j] = tempVertex[0];
            sphereVertices[1][i][j] = tempVertex[1];
            sphereVertices[2][i][j] = tempVertex[2];
            
        }
    }
}

void drawSpherePart(){
    glPushMatrix();
    glTranslatef(sphereCenter, 0, 0);
    glBegin(GL_QUADS);           // Begin drawing the pyramid with 4 triangles
    for(int i=0;i<numRows-1;i++){
        for(int j=0;j<numRows-1;j++){
                
                glVertex3f( sphereRadius* sphereVertices[0][i][j],sphereRadius* sphereVertices[1][i][j],sphereRadius* sphereVertices[2][i][j]);
                glVertex3f( sphereRadius*sphereVertices[0][i+1][j], sphereRadius*sphereVertices[1][i+1][j], sphereRadius*sphereVertices[2][i+1][j]);
                glVertex3f( sphereRadius*sphereVertices[0][i+1][j+1], sphereRadius*sphereVertices[1][i+1][j+1], sphereRadius*sphereVertices[2][i+1][j+1]);
                glVertex3f( sphereRadius*sphereVertices[0][i][j+1], sphereRadius*sphereVertices[1][i][j+1], sphereRadius*sphereVertices[2][i][j+1]);



        }
    }
    glEnd();   
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

    // gluLookAt(eyeat_x,eyeat_y,eyeat_z,
    //           eyeat_x + lookdir_x,eyeat_y + lookdir_y,eyeat_z + lookdir_z,
    //           lookup_x,lookup_y,lookup_z);


    gluLookAt(eyeat_x,eyeat_y,eyeat_z,
              lookat_x,lookat_y,lookat_z,
              up_x,up_y,up_z);
    // draw
    // if (isAxes) drawAxes();



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

    gluPerspective(45.0f, aspect, 0.1f, 100.0f);
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y) {
    float v = 0.1;
    switch (key) {
    // Control eye (location of the eye)
    // look left/right, so change only lookright and lookdir vectors
    case '1':


        lookat_x -= eyeat_x;
        lookat_y -= eyeat_y;
        lookat_z -= eyeat_z;


        t1_x = up_y*lookat_z - up_z*lookat_y;
        t1_y = up_z*lookat_x - up_x*lookat_z;
        t1_z = up_x*lookat_y - up_y*lookat_x;
        t1_x*=sin(camRotateRate);
        t1_y*=sin(camRotateRate);
        t1_z*=sin(camRotateRate);

        t2_dot = up_x * lookat_x + up_y * lookat_y + up_z * lookat_z;
        t2_x = up_x * t2_dot * (1-cos(camRotateRate));
        t2_y = up_y * t2_dot * (1-cos(camRotateRate));
        t2_z = up_z * t2_dot * (1-cos(camRotateRate)); 


        lookat_x = lookat_x*(cos(camRotateRate)) + t1_x + t2_x;
        lookat_y = lookat_y*(cos(camRotateRate)) + t1_y + t2_y;
        lookat_z = lookat_z*(cos(camRotateRate)) + t1_z + t2_z;


        lookat_x += eyeat_x;
        lookat_y += eyeat_y;
        lookat_z += eyeat_z;


        break;
    case '2':


        lookat_x -= eyeat_x;
        lookat_y -= eyeat_y;
        lookat_z -= eyeat_z;


        t1_x = up_y*lookat_z - up_z*lookat_y;
        t1_y = up_z*lookat_x - up_x*lookat_z;
        t1_z = up_x*lookat_y - up_y*lookat_x;
        t1_x*=sin(-camRotateRate);
        t1_y*=sin(-camRotateRate);
        t1_z*=sin(-camRotateRate);

        t2_dot = up_x * lookat_x + up_y * lookat_y + up_z * lookat_z;
        t2_x = up_x * t2_dot * (1-cos(-camRotateRate));
        t2_y = up_y * t2_dot * (1-cos(-camRotateRate));
        t2_z = up_z * t2_dot * (1-cos(-camRotateRate)); 


        lookat_x = lookat_x*(cos(-camRotateRate)) + t1_x + t2_x;
        lookat_y = lookat_y*(cos(-camRotateRate)) + t1_y + t2_y;
        lookat_z = lookat_z*(cos(-camRotateRate)) + t1_z + t2_z;


        lookat_x += eyeat_x;
        lookat_y += eyeat_y;
        lookat_z += eyeat_z;

        break;
    // Look up/down, so change only lookdir and lookup vectors
    case '3':



        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1/sqrt(dir_x*dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x*=dirNorm, dir_y*=dirNorm, dir_z*=dirNorm;

        left_x = up_y * dir_z - up_z*dir_y;
        left_y = up_z * dir_x - up_x*dir_z;
        left_z = up_x * dir_y - up_y*dir_x;


        lookat_x -= eyeat_x;
        lookat_y -= eyeat_y;
        lookat_z -= eyeat_z;


        t1_x = left_y*lookat_z - left_z*lookat_y;
        t1_y = left_z*lookat_x - left_x*lookat_z;
        t1_z = left_x*lookat_y - left_y*lookat_x;
        t1_x*=sin(-camRotateRate);
        t1_y*=sin(-camRotateRate);
        t1_z*=sin(-camRotateRate);

        t2_dot = left_x * lookat_x + left_y * lookat_y + left_z * lookat_z;
        t2_x = left_x * t2_dot * (1-cos(-camRotateRate));
        t2_y = left_y * t2_dot * (1-cos(-camRotateRate));
        t2_z = left_z * t2_dot * (1-cos(-camRotateRate)); 


        lookat_x = lookat_x*(cos(-camRotateRate)) + t1_x + t2_x;
        lookat_y = lookat_y*(cos(-camRotateRate)) + t1_y + t2_y;
        lookat_z = lookat_z*(cos(-camRotateRate)) + t1_z + t2_z;


        lookat_x += eyeat_x;
        lookat_y += eyeat_y;
        lookat_z += eyeat_z;
        break;
    case '4':


        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1/sqrt(dir_x*dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x*=dirNorm, dir_y*=dirNorm, dir_z*=dirNorm;

        left_x = up_y * dir_z - up_z*dir_y;
        left_y = up_z * dir_x - up_x*dir_z;
        left_z = up_x * dir_y - up_y*dir_x;


        lookat_x -= eyeat_x;
        lookat_y -= eyeat_y;
        lookat_z -= eyeat_z;


        t1_x = left_y*lookat_z - left_z*lookat_y;
        t1_y = left_z*lookat_x - left_x*lookat_z;
        t1_z = left_x*lookat_y - left_y*lookat_x;
        t1_x*=sin(camRotateRate);
        t1_y*=sin(camRotateRate);
        t1_z*=sin(camRotateRate);

        t2_dot = left_x * lookat_x + left_y * lookat_y + left_z * lookat_z;
        t2_x = left_x * t2_dot * (1-cos(camRotateRate));
        t2_y = left_y * t2_dot * (1-cos(camRotateRate));
        t2_z = left_z * t2_dot * (1-cos(camRotateRate)); 


        lookat_x = lookat_x*(cos(camRotateRate)) + t1_x + t2_x;
        lookat_y = lookat_y*(cos(camRotateRate)) + t1_y + t2_y;
        lookat_z = lookat_z*(cos(camRotateRate)) + t1_z + t2_z;


        lookat_x += eyeat_x;
        lookat_y += eyeat_y;
        lookat_z += eyeat_z;


        break;
    // tilt anticlock/clock, so change only lookup and lookright vectors
    case '5':



        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1/sqrt(dir_x*dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x*=dirNorm, dir_y*=dirNorm, dir_z*=dirNorm;



        t1_x = dir_y*up_z - dir_z*up_y;
        t1_y = dir_z*up_x - dir_x*up_z;
        t1_z = dir_x*up_y - dir_y*up_x;
        t1_x*=sin(camRotateRate);
        t1_y*=sin(camRotateRate);
        t1_z*=sin(camRotateRate);

        t2_dot = dir_x * up_x + dir_y * up_y + dir_z * up_z;
        t2_x = dir_x * t2_dot * (1-cos(camRotateRate));
        t2_y = dir_y * t2_dot * (1-cos(camRotateRate));
        t2_z = dir_z * t2_dot * (1-cos(camRotateRate)); 


        up_x = up_x*(cos(camRotateRate)) + t1_x + t2_x;
        up_y = up_y*(cos(camRotateRate)) + t1_y + t2_y;
        up_z = up_z*(cos(camRotateRate)) + t1_z + t2_z;


        break;
    case '6':


        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1/sqrt(dir_x*dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x*=dirNorm, dir_y*=dirNorm, dir_z*=dirNorm;



        t1_x = dir_y*up_z - dir_z*up_y;
        t1_y = dir_z*up_x - dir_x*up_z;
        t1_z = dir_x*up_y - dir_y*up_x;
        t1_x*=sin(-camRotateRate);
        t1_y*=sin(-camRotateRate);
        t1_z*=sin(-camRotateRate);

        t2_dot = dir_x * up_x + dir_y * up_y + dir_z * up_z;
        t2_x = dir_x * t2_dot * (1-cos(-camRotateRate));
        t2_y = dir_y * t2_dot * (1-cos(-camRotateRate));
        t2_z = dir_z * t2_dot * (1-cos(-camRotateRate)); 


        up_x = up_x*(cos(-camRotateRate)) + t1_x + t2_x;
        up_y = up_y*(cos(-camRotateRate)) + t1_y + t2_y;
        up_z = up_z*(cos(-camRotateRate)) + t1_z + t2_z;


        break;
    

    case 'a':
        objAngle-=10;   // show/hide Axes if 'a' is pressed
        objAngle%=360;
        break;
    case 'd':
        objAngle+=10;   // show/hide Cube if 'c' is pressed
        objAngle%=360;
        break;
    case ',':
        O2S();
        break;
    case '.':
        S2O();
        break;
    case 's':

        eyeat_y-=camMoveSmooth;
        break;
    case 'w':

        eyeat_y+=camMoveSmooth;

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

    switch (key) {
    
    case GLUT_KEY_LEFT:


        
        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1/sqrt(dir_x*dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x*=dirNorm, dir_y*=dirNorm, dir_z*=dirNorm;

        left_x = up_y * dir_z - up_z*dir_y;
        left_y = up_z * dir_x - up_x*dir_z;
        left_z = up_x * dir_y - up_y*dir_x;

        eyeat_x+=left_x * camMoveSmooth;
        eyeat_y+=left_y * camMoveSmooth;
        eyeat_z+=left_z * camMoveSmooth;

        lookat_x+=left_x * camMoveSmooth;
        lookat_y+=left_y * camMoveSmooth;
        lookat_z+=left_z * camMoveSmooth;

        break;

    case GLUT_KEY_RIGHT:



        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1/sqrt(dir_x*dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x*=dirNorm, dir_y*=dirNorm, dir_z*=dirNorm;

        left_x = up_y * dir_z - up_z*dir_y;
        left_y = up_z * dir_x - up_x*dir_z;
        left_z = up_x * dir_y - up_y*dir_x;

        eyeat_x-=left_x * camMoveSmooth;
        eyeat_y-=left_y * camMoveSmooth;
        eyeat_z-=left_z * camMoveSmooth;

        lookat_x-=left_x * camMoveSmooth;
        lookat_y-=left_y * camMoveSmooth;
        lookat_z-=left_z * camMoveSmooth;
        break;
    case GLUT_KEY_UP:

        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1/sqrt(dir_x*dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x*=dirNorm, dir_y*=dirNorm, dir_z*=dirNorm;

        eyeat_x+=dir_x * camMoveSmooth;
        eyeat_y+=dir_y * camMoveSmooth;
        eyeat_z+=dir_z * camMoveSmooth;

        lookat_x+=dir_x * camMoveSmooth;
        lookat_y+=dir_y * camMoveSmooth;
        lookat_z+=dir_z * camMoveSmooth;


        break;
    case GLUT_KEY_DOWN:


        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1/sqrt(dir_x*dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x*=dirNorm, dir_y*=dirNorm, dir_z*=dirNorm;

        eyeat_x-=dir_x * camMoveSmooth;
        eyeat_y-=dir_y * camMoveSmooth;
        eyeat_z-=dir_z * camMoveSmooth;

        lookat_x-=dir_x * camMoveSmooth;
        lookat_y-=dir_y * camMoveSmooth;
        lookat_z-=dir_z * camMoveSmooth;

        break;
    case GLUT_KEY_PAGE_DOWN:

        updir_x = up_x ;
        updir_y = up_y ;
        updir_z = up_z ;
        updirNorm = 1/sqrt(updir_x * updir_x + updir_y * updir_y + updir_z * updir_z);
        updir_x*=updirNorm, updir_y*=updirNorm, updir_z*=updirNorm;

        eyeat_x-=updir_x * camMoveSmooth;
        eyeat_y-=updir_y * camMoveSmooth;
        eyeat_z-=updir_z * camMoveSmooth;

        lookat_x-=updir_x * camMoveSmooth;
        lookat_y-=updir_y * camMoveSmooth;
        lookat_z-=updir_z * camMoveSmooth;




        break;
    case GLUT_KEY_PAGE_UP:


        updir_x = up_x ;
        updir_y = up_y ;
        updir_z = up_z ;
        updirNorm = 1/sqrt(updir_x * updir_x + updir_y * updir_y + updir_z * updir_z);
        updir_x*=updirNorm, updir_y*=updirNorm, updir_z*=updirNorm;

        eyeat_x+=updir_x * camMoveSmooth;
        eyeat_y+=updir_y * camMoveSmooth;
        eyeat_z+=updir_z * camMoveSmooth;

        lookat_x+=updir_x * camMoveSmooth;
        lookat_y+=updir_y * camMoveSmooth;
        lookat_z+=updir_z * camMoveSmooth;

        break;

    
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
