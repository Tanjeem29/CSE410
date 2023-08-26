#include <windows.h> // for MS Windows
#include <GL/glut.h> // GLUT, include glu.h and gl.h
#include <cmath>

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include <fstream>
#include <vector>
#include "./bitmap_image.hpp"
using namespace std;

#define phi (70.53 * M_PI / 180)
#define PI 3.1415926535
void capture();
double windowWidth = 640, windowHeight = 640;
class sphere
{
public:
    double center_x, center_y, center_z, radius;
    double k_a, k_d, k_s, k_r;
    double r, g, b;
    double shine;
    sphere(double cx, double cy, double cz, double r, double red, double green, double blue, double ka, double kd, double ks, double kr, double shininess)
    {
        center_x = cx;
        center_y = cy;
        center_z = cz;
        radius = r;
        k_a = ka;
        k_d = kd;
        k_s = ks;
        k_r = kr;
        r = red;
        g = green;
        b = blue;
        shine = shininess;
    }
    sphere()
    {
        center_x = 0;
        center_y = 0;
        center_z = 0;
        radius = 0;
        k_a = 0;
        k_d = 0;
        k_s = 0;
        k_r = 0;
        r = 0;
        g = 0;
        b = 0;
        shine = 0;
    }
    void setCenter(double cx, double cy, double cz)
    {
        center_x = cx;
        center_y = cy;
        center_z = cz;
    }
    void setRadius(double r)
    {
        radius = r;
    }
    void setK(double ka, double kd, double ks, double kr)
    {
        k_a = ka;
        k_d = kd;
        k_s = ks;
        k_r = kr;
    }
    void setColor(double red, double green, double blue)
    {
        r = red;
        g = green;
        b = blue;
    }
    void printSphere()
    {
        printf("Sphere\n");
        printf("Center: %lf %lf %lf\n", center_x, center_y, center_z);
        printf("Radius: %lf\n", radius);
        printf("K: %lf %lf %lf %lf\n", k_a, k_d, k_s, k_r);
        printf("Color: %lf %lf %lf\n", r, g, b);
        printf("Shine: %lf\n", shine);
    }
};

sphere spheres[100];
int numSpheres = 0;

class cube
{
public:
    double blX, blY, blZ, baseLength;
    double k_a, k_d, k_s, k_r;
    double r, g, b;
    double shine;
    double points[8][3]; // bottom left back , bottom right back, bottom right front , bottom left front, top left back , top right back, top right front , top left front,
    cube()
    {
        blX = 0;
        blY = 0;
        blZ = 0;
        baseLength = 0;
        k_a = 0;
        k_d = 0;
        k_s = 0;
        k_r = 0;
        r = 0;
        g = 0;
        b = 0;
        shine = 0;
    }
    cube(double x, double y, double z, double l, double red, double green, double blue, double ka, double kd, double ks, double kr, double shininess)
    {
        blX = x;
        blY = y;
        blZ = z;
        baseLength = l;
        k_a = ka;
        k_d = kd;
        k_s = ks;
        k_r = kr;
        r = red;
        g = green;
        b = blue;
        shine = shininess;
    }

    void calcPoints()
    {
        points[0][0] = blX;
        points[0][1] = blY;
        points[0][2] = blZ;

        points[1][0] = blX + baseLength;
        points[1][1] = blY;
        points[1][2] = blZ;

        points[2][0] = blX + baseLength;
        points[2][1] = blY;
        points[2][2] = blZ + baseLength;

        points[3][0] = blX;
        points[3][1] = blY;
        points[3][2] = blZ + baseLength;

        points[4][0] = blX;
        points[4][1] = blY + baseLength;
        points[4][2] = blZ;

        points[5][0] = blX + baseLength;
        points[5][1] = blY + baseLength;
        points[5][2] = blZ;

        points[6][0] = blX + baseLength;
        points[6][1] = blY + baseLength;
        points[6][2] = blZ + baseLength;

        points[7][0] = blX;
        points[7][1] = blY + baseLength;
        points[7][2] = blZ + baseLength;
    }
    void printCube()
    {
        printf("Cube\n");
        printf("Bottom left back: %lf %lf %lf\n", points[0][0], points[0][1], points[0][2]);
        printf("Bottom right back: %lf %lf %lf\n", points[1][0], points[1][1], points[1][2]);
        printf("Bottom right front: %lf %lf %lf\n", points[2][0], points[2][1], points[2][2]);
        printf("Bottom left front: %lf %lf %lf\n", points[3][0], points[3][1], points[3][2]);
        printf("Top left back: %lf %lf %lf\n", points[4][0], points[4][1], points[4][2]);
        printf("Top right back: %lf %lf %lf\n", points[5][0], points[5][1], points[5][2]);
        printf("Top right front: %lf %lf %lf\n", points[6][0], points[6][1], points[6][2]);
        printf("Top left front: %lf %lf %lf\n", points[7][0], points[7][1], points[7][2]);

        printf("Color: %lf %lf %lf\n", r, g, b);
        printf("K: %lf %lf %lf %lf\n", k_a, k_d, k_s, k_r);
        printf("Shine: %lf\n", shine);
    }
};

cube cubes[100];
int numCubes = 0;

class pyramid
{
public:
    double x, y, z, baseLength, height;
    double k_a, k_d, k_s, k_r;
    double r, g, b;
    double shine;
    double points[5][3]; // bottom left back , bottom right back, bottom right front , bottom left front, top
    pyramid()
    {
        x = 0;
        y = 0;
        z = 0;
        baseLength = 0;
        height = 0;
        k_a = 0;
        k_d = 0;
        k_s = 0;
        k_r = 0;
        r = 0;
        g = 0;
        b = 0;
        shine = 0;
    }
    pyramid(double x, double y, double z, double l, double h, double red, double green, double blue, double ka, double kd, double ks, double kr, double shininess)
    {
        x = x;
        y = y;
        z = z;
        baseLength = l;
        height = h;
        k_a = ka;
        k_d = kd;
        k_s = ks;
        k_r = kr;
        r = red;
        g = green;
        b = blue;
        shine = shininess;
    }
    void calcPoints()
    {
        double temp = baseLength / 2;
        points[0][0] = x - temp;
        points[0][1] = y;
        points[0][2] = z - temp;

        points[1][0] = x + temp;
        points[1][1] = y;
        points[1][2] = z - temp;

        points[2][0] = x + temp;
        points[2][1] = y;
        points[2][2] = z + temp;

        points[3][0] = x - temp;
        points[3][1] = y;
        points[3][2] = z + temp;

        points[4][0] = x;
        points[4][1] = y + height;
        points[4][2] = z;
    }
    void printPyramid()
    {
        printf("Pyramid\n");
        printf("Center: %lf %lf %lf\n", x, y, z);
        printf("Base length: %lf\n", baseLength);
        printf("Height: %lf\n", height);

        printf("Bottom left back: %lf %lf %lf\n", points[0][0], points[0][1], points[0][2]);
        printf("Bottom right back: %lf %lf %lf\n", points[1][0], points[1][1], points[1][2]);
        printf("Bottom right front: %lf %lf %lf\n", points[2][0], points[2][1], points[2][2]);
        printf("Bottom left front: %lf %lf %lf\n", points[3][0], points[3][1], points[3][2]);
        printf("Top: %lf %lf %lf\n", points[4][0], points[4][1], points[4][2]);

        printf("Color: %lf %lf %lf\n", r, g, b);
        printf("K: %lf %lf %lf %lf\n", k_a, k_d, k_s, k_r);
        printf("Shine: %lf\n", shine);
    }
};

pyramid pyramids[100];
int numPyramids = 0;

double nearPlane, farPlane, fovY, aspectRatio, fovX, recLevels, numPixels, checkerCellWidth, I_checker_a, I_checker_d, I_checker_r, numObjects;

double pX_x = 1, pX_y = 0, pX_z = 0,
       pY_x = 0, pY_y = 1, pY_z = 0,
       pZ_x = 0, pZ_y = 0, pZ_z = 1;

const int subdivisions = 5;
// // #define subdivisions (5);
const int numRows = (int)pow(2, subdivisions) + 1;

double sphereVertices[3][40][40];
// int objAngle = 0;
// int num_moves = 16;
// double inc_1 = (1.0/3 - 1)/num_moves;
// double inc_2 = (1.0/3 - 0)/num_moves;
// double cylinderLength = sqrt((pX_x-pY_x)*(pX_x-pY_x) + (pX_y-pY_y)*(pX_y-pY_y) + (pX_z-pY_z)*(pX_z-pY_z));
// double cylinderRadius = pX_z / sin(phi/2);

// double sphereCenter = pX_x - pX_z;
// double sphereRadius = sqrt(3) * pX_z;

// double camMoveSmooth = 0.25;
// double noRefChange = 0.25;

// double camMoveSmooth = 0.25;
// double camRotateRate = 0.01;
double camMoveSmooth = 3.5;
double camRotateRate = 0.035;

double dir_x, dir_y, dir_z, dirNorm, left_x, left_y, left_z, updir_x, updir_y, updir_z, updirNorm;
double t1_x, t1_y, t1_z, t2_x, t2_y, t2_z, t2_dot;

class ray
{
    double start[3];
    double dir[3];

    ray(double x, double y, double z, double dx, double dy, double dz)
    {
        start[0] = x;
        start[1] = y;
        start[2] = z;
        dir[0] = dx;
        dir[1] = dy;
        dir[2] = dz;
        double length = sqrt(dx * dx + dy * dy + dz * dz);
        dir[0] /= length;
        dir[1] /= length;
        dir[2] /= length;
    }
    ray()
    {
        start[0] = 0;
        start[1] = 0;
        start[2] = 0;
        dir[0] = 0;
        dir[1] = 0;
        dir[2] = 0;
    }
};

/* Initialize OpenGL Graphics */
void initGL()
{
    // Set "clearing" or background color
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black and opaque
    glEnable(GL_DEPTH_TEST);              // Enable depth testing for z-culling
}

// Global variables
// GLfloat eyex = 4, eyey = 4, eyez = 4;
// GLfloat centerx = 0, centery = 0, centerz = 0;
// GLfloat upx = 0, upy = 1, upz = 0;
// bool isAxes = true;

// double lookdir_x=-1/sqrt(3), lookdir_y =-1/sqrt(3), lookdir_z= -1/sqrt(3);
// double lookright_x = 0.707, lookright_y = 0, lookright_z = -0.707;
// double lookup_x = -1/sqrt(6), lookup_y = 2/sqrt(6), lookup_z = -1/sqrt(6);
// double camRotateRate = 0.01;

// MyMatrix eyeat = MyMatrix(3,1,50);
// MyMatrix lookat = MyMatrix(3,1,0);
// MyMatrix up = MyMatrix(3,1,0);
// MyMatrix rightVec = MyMatrix(3,1,0);
// rightVec.setElement(0,0,1/sqrt(2));
// rightVec.setElement(1,0,0);
// rightVec.setElement(2,0,-1/sqrt(2));

// double eyeat_x = 50, eyeat_y = 1, eyeat_z = 50;
// double lookat_x = 0, lookat_y = 0, lookat_z = 0;
// double up_x = -1 / sqrt(6), up_y = 2 / sqrt(6), up_z = -1 / sqrt(6);

double eyeat_x = 100, eyeat_y = 100, eyeat_z = 100;
double lookat_x = 0, lookat_y = 0, lookat_z = 0;
// double up_x = -20 / sqrt(400 + 400 + 10000), up_y = 100 / sqrt(sqrt(400 + 400 + 10000)), up_z = -20 / sqrt(400 + 400 + 10000);
double temp_right_x = 1, temp_right_y = 0, temp_right_z = -1;
double tempdir_x = lookat_x - eyeat_x, tempdir_y = lookat_y - eyeat_y, tempdir_z = lookat_z - eyeat_z;
// double tempup_x = -eyeat_y , tempup_y = eyeat_z + eyeat_x , tempup_z = -eyeat_y;
double tempup_x = temp_right_y * tempdir_z - temp_right_z * tempdir_y, tempup_y = temp_right_z * tempdir_x - temp_right_x * tempdir_z, tempup_z = temp_right_x * tempdir_y - temp_right_y * tempdir_x;
double up_length = sqrt(tempup_x * tempup_x + tempup_y * tempup_y + tempup_z * tempup_z);
double up_x = tempup_x / up_length, up_y = tempup_y / up_length, up_z = tempup_z / up_length;
// double up_x = tempup_x / sqrt(tempup_x * tempup_x + tempup_y * tempup_y + tempup_z * tempup_z), up_y = tempup_y / sqrt(tempup_x * tempup_x + tempup_y * tempup_y + tempup_z * tempup_z), up_z = tempup_z / sqrt(tempup_x * tempup_x + tempup_y * tempup_y + tempup_z * tempup_z);

/* Draw axes: X in Red, Y in Green and Z in Blue */
void drawAxes()
{
    glLineWidth(10);
    glBegin(GL_LINES);
    glColor3f(1, 0, 0); // Red
    // X axis
    glVertex3f(0, 0, 0);
    glVertex3f(100, 0, 0);

    glColor3f(0, 1, 0); // Green
    // Y axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 100, 0);

    glColor3f(0, 0, 1); // Blue
    // Z axis
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 100);
    glEnd();
}

void drawOctahedronFace()
{
    glBegin(GL_TRIANGLES); // Begin drawing the pyramid with 4 triangles
    // Front
    glVertex3f(pX_x, pX_y, pX_z);
    glVertex3f(pY_x, pY_y, pY_z);
    glVertex3f(pZ_x, pZ_y, pZ_z);
    glEnd(); // Done drawing the pyramid
}

void drawOctahedron()
{
    glColor3f(0, 1, 1);
    drawOctahedronFace();
    glPushMatrix();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    drawOctahedronFace();
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    drawOctahedronFace();
    glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    drawOctahedronFace();
    glPopMatrix();

    // glColor3f(1, 0, 1);

    // glPushMatrix();
    // glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    // drawOctahedronFace();
    // glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    // drawOctahedronFace();
    // glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    // drawOctahedronFace();
    // glRotatef(180.0f, 0.0f, 1.0f, 0.0f);
    // drawOctahedronFace();
    // glPopMatrix();

    // printf("%lf %lf", inc_1, inc_2);
}

// void O2S(){
//     // printf("OOOO");
//     if(pX_x + inc_1 - 1.0 /3 <= 0){
//         // printf("%lf\n", pX_x);
//         pX_x = pX_y = pX_z = pY_x = pY_y = pY_z = pZ_x = pZ_y = pZ_z = 1.0/3;

//     }
//     else{
//         // printf("%lf\n", pX_x);
//         pX_x +=inc_1;
//         pY_y +=inc_1;
//         pZ_z +=inc_1;

//         pX_y+=inc_2;
//         pX_z+=inc_2;
//         pY_x+=inc_2;
//         pY_z+=inc_2;
//         pZ_x+=inc_2;
//         pZ_y+=inc_2;
//     }
//     cylinderLength = sqrt((pX_x-pY_x)*(pX_x-pY_x) + (pX_y-pY_y)*(pX_y-pY_y) + (pX_z-pY_z)*(pX_z-pY_z));
//     cylinderRadius = pX_z / sin(phi/2);
//     sphereCenter = pX_x - pX_z;
//     sphereRadius = sqrt(3) * pX_z;
//     // sphereCenter += sphereCenter_inc;
//     // sphereRadius += sphereRadius_inc;
// }

// void S2O(){
//     if(pX_x - inc_1 - 1.0  > 0){
//         // printf("Ohnoh2\n");
//         // printf("%lf\n", pX_x);
//         pX_y = pX_z = pY_x  = pY_z = pZ_x = pZ_y =  0;
//         pX_x = pY_y = pZ_z = 1;
//     }
//     else{
//         // printf("%lf\n", pX_x);
//         pX_x -=inc_1;
//         pY_y -=inc_1;
//         pZ_z -=inc_1;

//         pX_y-=inc_2;
//         pX_z-=inc_2;
//         pY_x-=inc_2;
//         pY_z-=inc_2;
//         pZ_x-=inc_2;
//         pZ_y-=inc_2;
//     }
//     cylinderLength = sqrt((pX_x-pY_x)*(pX_x-pY_x) + (pX_y-pY_y)*(pX_y-pY_y) + (pX_z-pY_z)*(pX_z-pY_z));
//     cylinderRadius = pX_z / sin(phi/2);
//     // sphereCenter -= sphereCenter_inc;
//     // sphereRadius -= sphereRadius_inc;
//     sphereCenter = pX_x - pX_z;
//     sphereRadius = sqrt(3) * pX_z;
// }

void drawCylinder(double height, double radius, int segments)
{
    double tempx = radius, tempy = 0;
    double currx, curry;
    glBegin(GL_QUADS);
    for (int i = 1; i <= segments; i++)
    {
        double theta = i * 2.0 * M_PI / segments;
        currx = radius * cos(theta);
        curry = radius * sin(theta);

        GLfloat c = (2 + cos(theta)) / 3;
        // glColor3f(c,c,c);
        glVertex3f(currx, curry, height / 2);
        glVertex3f(currx, curry, -height / 2);

        glVertex3f(tempx, tempy, -height / 2);
        glVertex3f(tempx, tempy, height / 2);

        tempx = currx;
        tempy = curry;
    }
    glEnd();
}

// void drawCylinderEdge(){
//     double YZmp_x = (pY_x + pZ_x)/2;
//     double YZmp_y = (pY_y + pZ_y)/2;
//     double YZmp_z = (pY_z + pZ_z)/2;
//     double temp_x = YZmp_x;
//     double temp_y = YZmp_y * cos(M_PI/4) - YZmp_z * sin(M_PI/4);
//     double temp_z = YZmp_y * cos(M_PI/4) + YZmp_z * sin(M_PI/4);
//     double tr_x = 0;
//     double tr_y = 0;
//     double tr_z = temp_z - cylinderRadius * cos(phi/2);

//     glPushMatrix();
//         glRotatef(-45.0f, 1.0f, 0.0f, 0.0f);
//         glTranslatef(tr_x,tr_y, tr_z);
//         // glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
//         glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);

//         drawCylinder(cylinderLength, cylinderRadius, 90);
//         // glRotatef(45.0f, 1.0f, 0.0f, 0.0f);
//     glPopMatrix();

// }

// void drawAllEdges(){
//     glPushMatrix();
//         drawCylinderEdge();
//         glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
//         drawCylinderEdge();
//         glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
//         drawCylinderEdge();
//         glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
//         drawCylinderEdge();
//     glPopMatrix();
//     glPushMatrix();
//         glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
//         drawCylinderEdge();
//         glRotatef(90.0f, 1.0f, 0.0f, 0.0f); //axes also shifted, -x -> y, y -> x
//         drawCylinderEdge();
//         glRotatef(90.0f, 1.0f, 0.0f, 0.0f); //axes also shifted, -x -> y, y -> x
//         drawCylinderEdge();
//         glRotatef(90.0f, 1.0f, 0.0f, 0.0f); //axes also shifted, -x -> y, y -> x
//         drawCylinderEdge();
//     glPopMatrix();
//     glPushMatrix();
//         glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
//         drawCylinderEdge();
//         glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
//         drawCylinderEdge();
//         glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
//         drawCylinderEdge();
//         glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
//         drawCylinderEdge();
//     glPopMatrix();
// }

void calcVertices()
{
    double norm1[3];
    double norm2[3];
    double tempVertex[3];
    double latAng;
    double longAng;

    for (int i = 0; i < numRows; i++)
    {
        longAng = (M_PI / 180.0) * (45.0 - 90.0 * i / (numRows - 1));
        norm2[0] = -sin(longAng);
        norm2[1] = cos(longAng);
        norm2[2] = 0;
        for (int j = 0; j < numRows; j++)
        {
            latAng = (M_PI / 180.0) * (-45.0 + 90.0 * j / (numRows - 1));
            norm1[0] = -sin(latAng);
            norm1[1] = 0;
            norm1[2] = -cos(latAng);

            tempVertex[0] = norm1[1] * norm2[2] - norm1[2] * norm2[1];
            tempVertex[1] = norm1[2] * norm2[0] - norm1[0] * norm2[2];
            tempVertex[2] = norm1[0] * norm2[1] - norm1[1] * norm2[0];

            double scale = 1 / sqrt(tempVertex[0] * tempVertex[0] + tempVertex[1] * tempVertex[1] + tempVertex[2] * tempVertex[2]);
            tempVertex[0] *= scale;
            tempVertex[1] *= scale;
            tempVertex[2] *= scale;

            sphereVertices[0][i][j] = tempVertex[0];
            sphereVertices[1][i][j] = tempVertex[1];
            sphereVertices[2][i][j] = tempVertex[2];
        }
    }
}

// #USE THIS
void drawSpherePart(double sphereCenterX, double sphereCenterY, double sphereCenterZ, double sphereRadius)
{
    // glPushMatrix();
    // glTranslatef(sphereCenterX, sphereCenterY, sphereCenterZ);
    glBegin(GL_QUADS); // Begin drawing the pyramid with 4 triangles
    for (int i = 0; i < numRows - 1; i++)
    {
        for (int j = 0; j < numRows - 1; j++)
        {

            glVertex3f(sphereRadius * sphereVertices[0][i][j], sphereRadius * sphereVertices[1][i][j], sphereRadius * sphereVertices[2][i][j]);
            glVertex3f(sphereRadius * sphereVertices[0][i + 1][j], sphereRadius * sphereVertices[1][i + 1][j], sphereRadius * sphereVertices[2][i + 1][j]);
            glVertex3f(sphereRadius * sphereVertices[0][i + 1][j + 1], sphereRadius * sphereVertices[1][i + 1][j + 1], sphereRadius * sphereVertices[2][i + 1][j + 1]);
            glVertex3f(sphereRadius * sphereVertices[0][i][j + 1], sphereRadius * sphereVertices[1][i][j + 1], sphereRadius * sphereVertices[2][i][j + 1]);
        }
    }
    glEnd();
    // glPopMatrix();
}

void drawSphere(sphere s)
{
    glColor3f(s.r, s.g, s.b);
    glPushMatrix();
    glTranslatef(s.center_x, s.center_y, s.center_z);
    drawSpherePart(s.center_x, s.center_y, s.center_z, s.radius);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawSpherePart(s.center_x, s.center_y, s.center_z, s.radius);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawSpherePart(s.center_x, s.center_y, s.center_z, s.radius);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    drawSpherePart(s.center_x, s.center_y, s.center_z, s.radius);
    glRotatef(90.0f, 0.0f, 1.0f, 0.0f);
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
    drawSpherePart(s.center_x, s.center_y, s.center_z, s.radius);
    glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
    drawSpherePart(s.center_x, s.center_y, s.center_z, s.radius);
    glPopMatrix();
    // drawSpherePart();
    // glPushMatrix();
    //     glRotatef(180,0,1.0,0);
    //     drawSpherePart();
    // glPopMatrix();

    // glColor3f(0,1,0);
    // glPushMatrix();
    //     glRotatef(90,0,1.0,0);
    //     drawSpherePart();
    //     glRotatef(180,0,1.0,0);
    //     drawSpherePart();
    // glPopMatrix();

    // glColor3f(0,0,1);
    // glPushMatrix();
    //     glRotatef(90,0,0,1.0);
    //     drawSpherePart();
    //     glRotatef(180,0,0,1.0);
    //     drawSpherePart();
    // glPopMatrix();
}

/*  Handler for window-repaint event. Call back when the window first appears and
    whenever the window needs to be re-painted. */
// void display() {
//     // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
//     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//     glMatrixMode(GL_MODELVIEW);             // To operate on Model-View matrix
//     glLoadIdentity();                       // Reset the model-view matrix

//     // default arguments of gluLookAt
//     // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

//     // gluLookAt(eyeat_x,eyeat_y,eyeat_z,
//     //           eyeat_x + lookdir_x,eyeat_y + lookdir_y,eyeat_z + lookdir_z,
//     //           lookup_x,lookup_y,lookup_z);

//     gluLookAt(eyeat_x,eyeat_y,eyeat_z,
//               lookat_x,lookat_y,lookat_z,
//               up_x,up_y,up_z);
//     // draw
//     // if (isAxes) drawAxes();

//     glRotatef(objAngle, 0,1,0);
//     drawOctahedron();
//     glColor3f(1,1,0);
//     drawAllEdges();
//     glColor3f(1,0,0);
//     drawCorners();
//     // drawSpherePart();
//     glutSwapBuffers();  // Render now
// }

void drawcheckerboard()
{
    drawAxes();
    // glLineWidth(3);
    // glColor3f(1, 0, 0);
    // // cout<<checkerCellWidth<<endl;
    // int lim = 50;
    // printf("%lf\n", checkerCellWidth);
    // for (int i = -lim; i < lim; i++)
    // {
    //     glBegin(GL_LINES);
    //     glVertex3f(-lim * checkerCellWidth, 0, i * checkerCellWidth);
    //     glVertex3f(lim * checkerCellWidth, 0, i * checkerCellWidth);
    //     glEnd();
    //     glBegin(GL_LINES);
    //     glVertex3f(i * checkerCellWidth, 0, -lim * checkerCellWidth);
    //     glVertex3f(i * checkerCellWidth, 0, lim * checkerCellWidth);
    //     glEnd();
    // }
    glColor3f(1, 1, 1);
    // cout<<checkerCellWidth<<endl;
    int lim = 200;
    int color = 1;
    // printf("%lf\n", checkerCellWidth);
    for (int i = -lim; i < lim; i++)
    {
        color = 1 - color;
        glColor3f(color, color, color);
        for (int j = -lim; j < lim; j++)
        {
            glBegin(GL_QUADS);
            glVertex3f(i * checkerCellWidth, 0, j * checkerCellWidth);
            glVertex3f((i + 1) * checkerCellWidth, 0, j * checkerCellWidth);
            glVertex3f((i + 1) * checkerCellWidth, 0, (j + 1) * checkerCellWidth);
            glVertex3f((i)*checkerCellWidth, 0, (j + 1) * checkerCellWidth);
            glEnd();
            color = 1 - color;
            glColor3f(color, color, color);
        }
    }
    // glBegin(GL_QUADS);
    // glVertex3f(checkerCellWidth, 0, checkerCellWidth);
    // glVertex3f(-checkerCellWidth, 0, checkerCellWidth);
    // glVertex3f(-checkerCellWidth, 0, -checkerCellWidth);
    // glVertex3f(checkerCellWidth, 0, -checkerCellWidth);
    // glEnd();
}
void drawSpheres()
{
    for (int i = 0; i < numSpheres; i++)
    {
        // spheres[i].printSphere();
        drawSphere(spheres[i]);
    }
}

void drawCube(cube c)
{
    glColor3f(c.r, c.g, c.b);
    glBegin(GL_QUADS);
    glVertex3f(c.points[0][0], c.points[0][1], c.points[0][2]);
    glVertex3f(c.points[1][0], c.points[1][1], c.points[1][2]);
    glVertex3f(c.points[2][0], c.points[2][1], c.points[2][2]);
    glVertex3f(c.points[3][0], c.points[3][1], c.points[3][2]);

    glVertex3f(c.points[0][0], c.points[0][1], c.points[0][2]);
    glVertex3f(c.points[4][0], c.points[4][1], c.points[4][2]);
    glVertex3f(c.points[5][0], c.points[5][1], c.points[5][2]);
    glVertex3f(c.points[1][0], c.points[1][1], c.points[1][2]);

    glVertex3f(c.points[0][0], c.points[0][1], c.points[0][2]);
    glVertex3f(c.points[4][0], c.points[4][1], c.points[4][2]);
    glVertex3f(c.points[7][0], c.points[7][1], c.points[7][2]);
    glVertex3f(c.points[3][0], c.points[3][1], c.points[3][2]);

    glVertex3f(c.points[3][0], c.points[3][1], c.points[3][2]);
    glVertex3f(c.points[2][0], c.points[2][1], c.points[2][2]);
    glVertex3f(c.points[6][0], c.points[6][1], c.points[6][2]);
    glVertex3f(c.points[7][0], c.points[7][1], c.points[7][2]);

    glVertex3f(c.points[4][0], c.points[4][1], c.points[4][2]);
    glVertex3f(c.points[5][0], c.points[5][1], c.points[5][2]);
    glVertex3f(c.points[6][0], c.points[6][1], c.points[6][2]);
    glVertex3f(c.points[7][0], c.points[7][1], c.points[7][2]);

    glVertex3f(c.points[1][0], c.points[1][1], c.points[1][2]);
    glVertex3f(c.points[5][0], c.points[5][1], c.points[5][2]);
    glVertex3f(c.points[6][0], c.points[6][1], c.points[6][2]);
    glVertex3f(c.points[2][0], c.points[2][1], c.points[2][2]);

    glEnd();
}

void drawCubes()
{
    for (int i = 0; i < numCubes; i++)
    {
        // cubes[i].printCube();
        drawCube(cubes[i]);
    }
}

void drawPyramid(pyramid p)
{
    glColor3f(p.r, p.g, p.b);
    glBegin(GL_QUADS);
    glVertex3f(p.points[0][0], p.points[0][1], p.points[0][2]);
    glVertex3f(p.points[1][0], p.points[1][1], p.points[1][2]);
    glVertex3f(p.points[2][0], p.points[2][1], p.points[2][2]);
    glVertex3f(p.points[3][0], p.points[3][1], p.points[3][2]);
    glEnd();

    glBegin(GL_TRIANGLES);
    glVertex3f(p.points[0][0], p.points[0][1], p.points[0][2]);
    glVertex3f(p.points[1][0], p.points[1][1], p.points[1][2]);
    glVertex3f(p.points[4][0], p.points[4][1], p.points[4][2]);

    glVertex3f(p.points[1][0], p.points[1][1], p.points[1][2]);
    glVertex3f(p.points[2][0], p.points[2][1], p.points[2][2]);
    glVertex3f(p.points[4][0], p.points[4][1], p.points[4][2]);

    glVertex3f(p.points[2][0], p.points[2][1], p.points[2][2]);
    glVertex3f(p.points[3][0], p.points[3][1], p.points[3][2]);
    glVertex3f(p.points[4][0], p.points[4][1], p.points[4][2]);

    glVertex3f(p.points[3][0], p.points[3][1], p.points[3][2]);
    glVertex3f(p.points[0][0], p.points[0][1], p.points[0][2]);
    glVertex3f(p.points[4][0], p.points[4][1], p.points[4][2]);
    glEnd();
}

void drawPyramids()
{
    for (int i = 0; i < numPyramids; i++)
    {
        // pyramids[i].printPyramid();
        drawPyramid(pyramids[i]);
    }
}

void display()
{
    // glClear(GL_COLOR_BUFFER_BIT);            // Clear the color buffer (background)
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW); // To operate on Model-View matrix
    glLoadIdentity();           // Reset the model-view matrix

    // default arguments of gluLookAt
    // gluLookAt(0,0,0, 0,0,-100, 0,1,0);

    // gluLookAt(eyeat_x,eyeat_y,eyeat_z,
    //           eyeat_x + lookdir_x,eyeat_y + lookdir_y,eyeat_z + lookdir_z,
    //           lookup_x,lookup_y,lookup_z);

    gluLookAt(eyeat_x, eyeat_y, eyeat_z,
              lookat_x, lookat_y, lookat_z,
              up_x, up_y, up_z);
    // draw
    // if (isAxes) drawAxes();

    drawcheckerboard();
    // printf("numSpheres: %d\n", numSpheres);
    drawSpheres();
    drawCubes();
    drawPyramids();
    // glRotatef(objAngle, 0,1,0);
    // drawOctahedron();
    // glColor3f(1,1,0);
    // drawAllEdges();
    // glColor3f(1,0,0);
    // drawCorners();
    // drawSpherePart();

    glutSwapBuffers(); // Render now
}

/* Handler for window re-size event. Called back when the window first appears and
   whenever the window is re-sized with its new width and height */
void reshapeListener(GLsizei width, GLsizei height)
{ // GLsizei for non-negative integer
    // Compute aspect ratio of the new window
    if (height == 0)
        height = 1; // To prevent divide by 0
    GLfloat aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping area to match the viewport
    glMatrixMode(GL_PROJECTION); // To operate on the Projection matrix
    glLoadIdentity();            // Reset the projection matrix

    // gluPerspective(45.0f, aspect, 0.1f, 100.0f);
    // fovY = 45;
    gluPerspective(fovY, aspectRatio, nearPlane, farPlane);
}

/* Callback handler for normal-key event */
void keyboardListener(unsigned char key, int x, int y)
{
    float v = 0.1;
    switch (key)
    {
    // Control eye (location of the eye)
    // look left/right, so change only lookright and lookdir vectors
    case '1':

        lookat_x -= eyeat_x;
        lookat_y -= eyeat_y;
        lookat_z -= eyeat_z;

        t1_x = up_y * lookat_z - up_z * lookat_y;
        t1_y = up_z * lookat_x - up_x * lookat_z;
        t1_z = up_x * lookat_y - up_y * lookat_x;
        t1_x *= sin(camRotateRate);
        t1_y *= sin(camRotateRate);
        t1_z *= sin(camRotateRate);

        t2_dot = up_x * lookat_x + up_y * lookat_y + up_z * lookat_z;
        t2_x = up_x * t2_dot * (1 - cos(camRotateRate));
        t2_y = up_y * t2_dot * (1 - cos(camRotateRate));
        t2_z = up_z * t2_dot * (1 - cos(camRotateRate));

        lookat_x = lookat_x * (cos(camRotateRate)) + t1_x + t2_x;
        lookat_y = lookat_y * (cos(camRotateRate)) + t1_y + t2_y;
        lookat_z = lookat_z * (cos(camRotateRate)) + t1_z + t2_z;

        lookat_x += eyeat_x;
        lookat_y += eyeat_y;
        lookat_z += eyeat_z;

        break;
    case '2':

        lookat_x -= eyeat_x;
        lookat_y -= eyeat_y;
        lookat_z -= eyeat_z;

        t1_x = up_y * lookat_z - up_z * lookat_y;
        t1_y = up_z * lookat_x - up_x * lookat_z;
        t1_z = up_x * lookat_y - up_y * lookat_x;
        t1_x *= sin(-camRotateRate);
        t1_y *= sin(-camRotateRate);
        t1_z *= sin(-camRotateRate);

        t2_dot = up_x * lookat_x + up_y * lookat_y + up_z * lookat_z;
        t2_x = up_x * t2_dot * (1 - cos(-camRotateRate));
        t2_y = up_y * t2_dot * (1 - cos(-camRotateRate));
        t2_z = up_z * t2_dot * (1 - cos(-camRotateRate));

        lookat_x = lookat_x * (cos(-camRotateRate)) + t1_x + t2_x;
        lookat_y = lookat_y * (cos(-camRotateRate)) + t1_y + t2_y;
        lookat_z = lookat_z * (cos(-camRotateRate)) + t1_z + t2_z;

        lookat_x += eyeat_x;
        lookat_y += eyeat_y;
        lookat_z += eyeat_z;

        break;
    // Look up/down, so change only lookdir and lookup vectors
    case '3':

        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        left_x = up_y * dir_z - up_z * dir_y;
        left_y = up_z * dir_x - up_x * dir_z;
        left_z = up_x * dir_y - up_y * dir_x;

        lookat_x -= eyeat_x;
        lookat_y -= eyeat_y;
        lookat_z -= eyeat_z;

        t1_x = left_y * lookat_z - left_z * lookat_y;
        t1_y = left_z * lookat_x - left_x * lookat_z;
        t1_z = left_x * lookat_y - left_y * lookat_x;
        t1_x *= sin(-camRotateRate);
        t1_y *= sin(-camRotateRate);
        t1_z *= sin(-camRotateRate);

        t2_dot = left_x * lookat_x + left_y * lookat_y + left_z * lookat_z;
        t2_x = left_x * t2_dot * (1 - cos(-camRotateRate));
        t2_y = left_y * t2_dot * (1 - cos(-camRotateRate));
        t2_z = left_z * t2_dot * (1 - cos(-camRotateRate));

        lookat_x = lookat_x * (cos(-camRotateRate)) + t1_x + t2_x;
        lookat_y = lookat_y * (cos(-camRotateRate)) + t1_y + t2_y;
        lookat_z = lookat_z * (cos(-camRotateRate)) + t1_z + t2_z;


        
        

        lookat_x += eyeat_x;
        lookat_y += eyeat_y;
        lookat_z += eyeat_z;

        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;
        
        up_x = dir_y * left_z - dir_z * left_y;
        up_y = dir_z * left_x - dir_x * left_z;
        up_z = dir_x * left_y - dir_y * left_x;

        printf("dir lenght: %lf\n", sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z));
        printf("left lenght: %lf\n", sqrt(left_x * left_x + left_y * left_y + left_z * left_z));
        printf("up lenght: %lf\n", sqrt(up_x * up_x + up_y * up_y + up_z * up_z));
        // double upNorm = 1 / sqrt(up_x * up_x + up_y * up_y + up_z * up_z);
        // up_x *= upNorm, up_y *= upNorm, up_z *= upNorm;
        break;
    case '4':

        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        left_x = up_y * dir_z - up_z * dir_y;
        left_y = up_z * dir_x - up_x * dir_z;
        left_z = up_x * dir_y - up_y * dir_x;

        lookat_x -= eyeat_x;
        lookat_y -= eyeat_y;
        lookat_z -= eyeat_z;

        t1_x = left_y * lookat_z - left_z * lookat_y;
        t1_y = left_z * lookat_x - left_x * lookat_z;
        t1_z = left_x * lookat_y - left_y * lookat_x;
        t1_x *= sin(camRotateRate);
        t1_y *= sin(camRotateRate);
        t1_z *= sin(camRotateRate);

        t2_dot = left_x * lookat_x + left_y * lookat_y + left_z * lookat_z;
        t2_x = left_x * t2_dot * (1 - cos(camRotateRate));
        t2_y = left_y * t2_dot * (1 - cos(camRotateRate));
        t2_z = left_z * t2_dot * (1 - cos(camRotateRate));

        lookat_x = lookat_x * (cos(camRotateRate)) + t1_x + t2_x;
        lookat_y = lookat_y * (cos(camRotateRate)) + t1_y + t2_y;
        lookat_z = lookat_z * (cos(camRotateRate)) + t1_z + t2_z;
        
        up_x = dir_y * left_z - dir_z * left_y;
        up_y = dir_z * left_x - dir_x * left_z;
        up_z = dir_x * left_y - dir_y * left_x;

        lookat_x += eyeat_x;
        lookat_y += eyeat_y;
        lookat_z += eyeat_z;

        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;
        
        up_x = dir_y * left_z - dir_z * left_y;
        up_y = dir_z * left_x - dir_x * left_z;
        up_z = dir_x * left_y - dir_y * left_x;

        printf("dir lenght: %lf\n", sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z));
        printf("left lenght: %lf\n", sqrt(left_x * left_x + left_y * left_y + left_z * left_z));
        printf("up lenght: %lf\n", sqrt(up_x * up_x + up_y * up_y + up_z * up_z));

        break;
    // tilt anticlock/clock, so change only lookup and lookright vectors
    case '5':

        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        t1_x = dir_y * up_z - dir_z * up_y;
        t1_y = dir_z * up_x - dir_x * up_z;
        t1_z = dir_x * up_y - dir_y * up_x;
        t1_x *= sin(camRotateRate);
        t1_y *= sin(camRotateRate);
        t1_z *= sin(camRotateRate);

        t2_dot = dir_x * up_x + dir_y * up_y + dir_z * up_z;
        t2_x = dir_x * t2_dot * (1 - cos(camRotateRate));
        t2_y = dir_y * t2_dot * (1 - cos(camRotateRate));
        t2_z = dir_z * t2_dot * (1 - cos(camRotateRate));

        up_x = up_x * (cos(camRotateRate)) + t1_x + t2_x;
        up_y = up_y * (cos(camRotateRate)) + t1_y + t2_y;
        up_z = up_z * (cos(camRotateRate)) + t1_z + t2_z;

        break;
    case '6':

        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        t1_x = dir_y * up_z - dir_z * up_y;
        t1_y = dir_z * up_x - dir_x * up_z;
        t1_z = dir_x * up_y - dir_y * up_x;
        t1_x *= sin(-camRotateRate);
        t1_y *= sin(-camRotateRate);
        t1_z *= sin(-camRotateRate);

        t2_dot = dir_x * up_x + dir_y * up_y + dir_z * up_z;
        t2_x = dir_x * t2_dot * (1 - cos(-camRotateRate));
        t2_y = dir_y * t2_dot * (1 - cos(-camRotateRate));
        t2_z = dir_z * t2_dot * (1 - cos(-camRotateRate));

        up_x = up_x * (cos(-camRotateRate)) + t1_x + t2_x;
        up_y = up_y * (cos(-camRotateRate)) + t1_y + t2_y;
        up_z = up_z * (cos(-camRotateRate)) + t1_z + t2_z;

        break;

     case '0':

        capture();

        break;

    case 'a':
        // objAngle-=10;   // show/hide Axes if 'a' is pressed
        // objAngle%=360;
        break;
    case 'd':
        // objAngle+=10;   // show/hide Cube if 'c' is pressed
        // objAngle%=360;
        break;
    case ',':
        // O2S();
        break;
    case '.':
        // S2O();
        break;
        // case 's':
        //     dir_x = lookat_x - eyeat_x;
        //     dir_y = lookat_y - eyeat_y;
        //     dir_z = lookat_z - eyeat_z;
        //     dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        //     dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        //     left_x = up_y * dir_z - up_z * dir_y;
        //     left_y = up_z * dir_x - up_x * dir_z;
        //     left_z = up_x * dir_y - up_y * dir_x;

        //     eyeat_y -= camMoveSmooth;

        //     dir_x = lookat_x - eyeat_x;
        //     dir_y = lookat_y - eyeat_y;
        //     dir_z = lookat_z - eyeat_z;
        //     dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        //     dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        //     up_x = dir_y * left_z - dir_z * left_y;
        //     up_y = dir_z * left_x - dir_x * left_z;
        //     up_z = dir_x * left_y - dir_y * left_x;

        //     break;
        // case 'w':
        //     dir_x = lookat_x - eyeat_x;
        //     dir_y = lookat_y - eyeat_y;
        //     dir_z = lookat_z - eyeat_z;
        //     dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        //     dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        //     left_x = up_y * dir_z - up_z * dir_y;
        //     left_y = up_z * dir_x - up_x * dir_z;
        //     left_z = up_x * dir_y - up_y * dir_x;

        //     eyeat_y += camMoveSmooth;

        //     dir_x = lookat_x - eyeat_x;
        //     dir_y = lookat_y - eyeat_y;
        //     dir_z = lookat_z - eyeat_z;
        //     dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        //     dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        //     up_x = dir_y * left_z - dir_z * left_y;
        //     up_y = dir_z * left_x - dir_x * left_z;
        //     up_z = dir_x * left_y - dir_y * left_x;

        //     break;

    case 's':

        // eyeat_y -= camMoveSmooth;
        // lookat_y -= camMoveSmooth;

        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        left_x = up_y * dir_z - up_z * dir_y;
        left_y = up_z * dir_x - up_x * dir_z;
        left_z = up_x * dir_y - up_y * dir_x;

        eyeat_x -= up_x * camMoveSmooth;
        eyeat_y -= up_y * camMoveSmooth;
        eyeat_z -= up_z * camMoveSmooth;

        lookat_x -= up_x * camMoveSmooth;
        lookat_y -= up_y * camMoveSmooth;
        lookat_z -= up_z * camMoveSmooth;

        // dir_x = lookat_x - eyeat_x;
        // dir_y = lookat_y - eyeat_y;
        // dir_z = lookat_z - eyeat_z;
        // dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        // dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        // up_x = dir_y * left_z - dir_z * left_y;
        // up_y = dir_z * left_x - dir_x * left_z;
        // up_z = dir_x * left_y - dir_y * left_x;

        break;
    case 'w':

        // eyeat_y += camMoveSmooth;
        // lookat_y += camMoveSmooth;

        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        left_x = up_y * dir_z - up_z * dir_y;
        left_y = up_z * dir_x - up_x * dir_z;
        left_z = up_x * dir_y - up_y * dir_x;

        eyeat_x += up_x * camMoveSmooth;
        eyeat_y += up_y * camMoveSmooth;
        eyeat_z += up_z * camMoveSmooth;

        lookat_x += up_x * camMoveSmooth;
        lookat_y += up_y * camMoveSmooth;
        lookat_z += up_z * camMoveSmooth;

        // dir_x = lookat_x - eyeat_x;
        // dir_y = lookat_y - eyeat_y;
        // dir_z = lookat_z - eyeat_z;
        // dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        // dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        // up_x = dir_y * left_z - dir_z * left_y;
        // up_y = dir_z * left_x - dir_x * left_z;
        // up_z = dir_x * left_y - dir_y * left_x;

        break;

    // Control exit
    case 27:     // ESC key
        exit(0); // Exit window
        break;
    }
    glutPostRedisplay(); // Post a paint request to activate display()
}

/* Callback handler for special-key event */
void specialKeyListener(int key, int x, int y)
{

    switch (key)
    {

    case GLUT_KEY_LEFT:

        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        left_x = up_y * dir_z - up_z * dir_y;
        left_y = up_z * dir_x - up_x * dir_z;
        left_z = up_x * dir_y - up_y * dir_x;

        eyeat_x += left_x * camMoveSmooth;
        eyeat_y += left_y * camMoveSmooth;
        eyeat_z += left_z * camMoveSmooth;

        lookat_x += left_x * camMoveSmooth;
        lookat_y += left_y * camMoveSmooth;
        lookat_z += left_z * camMoveSmooth;

        break;

    case GLUT_KEY_RIGHT:

        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        left_x = up_y * dir_z - up_z * dir_y;
        left_y = up_z * dir_x - up_x * dir_z;
        left_z = up_x * dir_y - up_y * dir_x;

        eyeat_x -= left_x * camMoveSmooth;
        eyeat_y -= left_y * camMoveSmooth;
        eyeat_z -= left_z * camMoveSmooth;

        lookat_x -= left_x * camMoveSmooth;
        lookat_y -= left_y * camMoveSmooth;
        lookat_z -= left_z * camMoveSmooth;
        break;
    case GLUT_KEY_UP:

        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        eyeat_x += dir_x * camMoveSmooth;
        eyeat_y += dir_y * camMoveSmooth;
        eyeat_z += dir_z * camMoveSmooth;

        lookat_x += dir_x * camMoveSmooth;
        lookat_y += dir_y * camMoveSmooth;
        lookat_z += dir_z * camMoveSmooth;

        break;
    case GLUT_KEY_DOWN:

        dir_x = lookat_x - eyeat_x;
        dir_y = lookat_y - eyeat_y;
        dir_z = lookat_z - eyeat_z;
        dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);
        dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

        eyeat_x -= dir_x * camMoveSmooth;
        eyeat_y -= dir_y * camMoveSmooth;
        eyeat_z -= dir_z * camMoveSmooth;

        lookat_x -= dir_x * camMoveSmooth;
        lookat_y -= dir_y * camMoveSmooth;
        lookat_z -= dir_z * camMoveSmooth;

        break;
    case GLUT_KEY_PAGE_DOWN:

        updir_x = up_x;
        updir_y = up_y;
        updir_z = up_z;
        updirNorm = 1 / sqrt(updir_x * updir_x + updir_y * updir_y + updir_z * updir_z);
        updir_x *= updirNorm, updir_y *= updirNorm, updir_z *= updirNorm;

        eyeat_x -= updir_x * camMoveSmooth;
        eyeat_y -= updir_y * camMoveSmooth;
        eyeat_z -= updir_z * camMoveSmooth;

        lookat_x -= updir_x * camMoveSmooth;
        lookat_y -= updir_y * camMoveSmooth;
        lookat_z -= updir_z * camMoveSmooth;

        break;
    case GLUT_KEY_PAGE_UP:

        updir_x = up_x;
        updir_y = up_y;
        updir_z = up_z;
        updirNorm = 1 / sqrt(updir_x * updir_x + updir_y * updir_y + updir_z * updir_z);
        updir_x *= updirNorm, updir_y *= updirNorm, updir_z *= updirNorm;

        eyeat_x += updir_x * camMoveSmooth;
        eyeat_y += updir_y * camMoveSmooth;
        eyeat_z += updir_z * camMoveSmooth;

        lookat_x += updir_x * camMoveSmooth;
        lookat_y += updir_y * camMoveSmooth;
        lookat_z += updir_z * camMoveSmooth;

        break;

    default:
        return;
    }
    glutPostRedisplay(); // Post a paint request to activate display()
}

void takeinputs()
{
    string filename1 = "./description.txt";
    ifstream file(filename1);
    if (!file.is_open())
    {
        cout << "Error opening file" << endl;
        return;
    }
    file >> nearPlane >> farPlane >> fovY >> aspectRatio;
    file >> recLevels >> numPixels;
    file >> checkerCellWidth >> I_checker_a >> I_checker_d >> I_checker_r;
    file >> numObjects;

    fovX = fovY * aspectRatio;
    // printf("testing\n");
    // cout<<"in takeinputs"<<endl;
    // printf("%f %f %f %f %d %d %f %f %f %f %d\n", nearPlane, farPlane, fovY, aspectRatio, recLevels, numPixels, checkerCellWidth, I_checker_a, I_checker_d, I_checker_r, numObjects);
    // cout<<nearPlane<<" "<<farPlane<<" "<<fovY<<" "<<aspectRatio<<" "<<recLevels<<" "<<numPixels<<" "<<checkerCellWidth<<" "<<I_checker_a<<" "<<I_checker_d<<" "<<I_checker_r<<" "<<numObjects<<endl;
    string type;
    // spheres.clear();

    for (int i = 0; i < numObjects; i++)
    {
        file >> type;
        // printf("%s\n", type.c_str());
        if (type == "sphere")
        {
            double x1, y1, z1, r1;
            file >> x1 >> y1 >> z1;
            file >> r1;
            double cr1, cg1, cb1, ka1, kd1, ks1, kr1, shine1;
            file >> cr1 >> cg1 >> cb1 >> ka1 >> kd1 >> ks1 >> kr1 >> shine1;
            sphere s = sphere(x1, y1, z1, r1, cr1, cg1, cb1, ka1, kd1, ks1, kr1, shine1);
            s.center_x = x1;
            s.center_y = y1;
            s.center_z = z1;
            s.radius = r1;

            s.r = cr1;
            s.g = cg1;
            s.b = cb1;

            s.k_a = ka1;
            s.k_d = kd1;
            s.k_s = ks1;
            s.k_r = kr1;
            s.shine = shine1;

            // printf("=============================\n");

            // printf("Sphere number: %d\n", numSpheres + 1);
            // printf("Center: %f %f %f\n", x1, y1, z1);
            // printf("Radius: %f\n", r1);
            // printf("Color: %f %f %f\n", cr1, cg1, cb1);
            // printf("ka1: %f kd1: %f ks1: %f kr: %f shine1: %f\n", ka1, kd1, ks1, kr1, shine1);

            // printf("-----------------------------\n");

            // s.printSphere();

            // spheres.push_back(s);
            spheres[numSpheres++] = s;
        }
        else if (type == "cube")
        {
            double x2, y2, z2, length2;
            file >> x2 >> y2 >> z2 >> length2;
            double cr2, cg2, cb2, ka2, kd2, ks2, kr2, shine2;
            file >> cr2 >> cg2 >> cb2 >> ka2 >> kd2 >> ks2 >> kr2 >> shine2;

            cube c = cube(x2, y2, z2, length2, cr2, cg2, cb2, ka2, kd2, ks2, kr2, shine2);

            c.baseLength = length2;
            c.blX = x2;
            c.blY = y2;
            c.blZ = z2;

            c.r = cr2;
            c.g = cg2;
            c.b = cb2;

            c.k_a = ka2;
            c.k_d = kd2;
            c.k_s = ks2;
            c.k_r = kr2;

            c.shine = shine2;

            c.calcPoints();

            // printf("=============================\n");

            // printf("Cube number: %d\n", numCubes + 1);
            // printf("Center: %f %f %f\n", x2, y2, z2);
            // printf("Length2: %f\n", length2);
            // printf("Color: %f %f %f\n", cr2, cg2, cb2);
            // printf("ka2: %f kd2: %f ks2: %f kr2: %f shine2: %f\n", ka2, kd2, ks2, kr2, shine2);

            // printf("-----------------------------\n");

            // c.printCube();

            cubes[numCubes++] = c;
        }
        else if (type == "pyramid")
        {
            double x, y, z, width, height;
            file >> x >> y >> z >> width >> height;
            double cr, cg, cb, ka, kd, ks, kr, shine;

            file >> cr >> cg >> cb >> ka >> kd >> ks >> kr >> shine;
            pyramid p = pyramid(x, y, z, width, height, cr, cg, cb, ka, kd, ks, kr, shine);

            p.baseLength = width;
            p.height = height;
            p.x = x;
            p.y = y;
            p.z = z;

            p.k_a = ka;
            p.k_d = kd;
            p.k_s = ks;
            p.k_r = kr;

            p.shine = shine;

            p.calcPoints();

            // printf("=============================\n");

            // printf("Pyramid number: %d\n", numPyramids + 1);
            // printf("Center: %f %f %f\n", x, y, z);
            // printf("Width: %f Height: %f\n", width, height);
            // printf("Color: %f %f %f\n", cr, cg, cb);
            // printf("ka: %f kd: %f ks: %f kr: %f shine: %f\n", ka, kd, ks, kr, shine);

            // printf("-----------------------------\n");

            // p.printPyramid();

            pyramids[numPyramids++] = p;
        }
    }
}

double intersectSphere(double rayBegin[], double rayDir[], sphere s ){
    double t = 0;

    return t;
}

double intersectCube(double rayBegin[], double rayDir[], cube c ){
    double t = 0;

    return t;
}



double determinantThree(double mat[][3]){
    double t = mat[0][0]*(mat[1][1]*mat[2][2] - mat[1][2]*mat[2][1]) - mat[0][1]*(mat[1][0]*mat[2][2] - mat[1][2]*mat[2][0]) + mat[0][2]*(mat[1][0]*mat[2][1] - mat[1][1]*mat[2][0]);
    return t;
}

double intersectTriangle(double rayBegin[], double rayDir[], double v1[], double v2[], double v3[]){
    // double t = 1;
    // MyMatrix A = MyMatrix(3, 3,0);
    // A.setElement(0, 0, v1[0] - v2[0]);
    // A.setElement(0, 1, v1[0] - v3[0]);
    // A.setElement(0, 2, rayDir[0]);

    // A.setElement(1, 0, v1[1] - v2[1]);
    // A.setElement(1, 1, v1[1] - v3[1]);
    // A.setElement(1, 2, rayDir[1]);

    // A.setElement(2, 0, v1[2] - v2[2]);
    // A.setElement(2, 1, v1[2] - v3[2]);
    // A.setElement(2, 2, rayDir[2]);

    // double detA = A.getDeterminant();

    double A[3][3];
    A[0][0] = v1[0] - v2[0];
    A[0][1] = v1[0] - v3[0];
    A[0][2] = rayDir[0];

    A[1][0] = v1[1] - v2[1];
    A[1][1] = v1[1] - v3[1];
    A[1][2] = rayDir[1];

    A[2][0] = v1[2] - v2[2];
    A[2][1] = v1[2] - v3[2];
    A[2][2] = rayDir[2];

    double detA = determinantThree(A);
    // double detA = A[0][0]*(A[1][1]*A[2][2] - A[1][2]*A[2][1]) - A[0][1]*(A[1][0]*A[2][2] - A[1][2]*A[2][0]) + A[0][2]*(A[1][0]*A[2][1] - A[1][1]*A[2][0]);

    

    // MyMatrix betaMat = MyMatrix(3, 3,0);
    // betaMat.setElement(0, 0, v1[0] - rayBegin[0]);
    // betaMat.setElement(0, 1, v1[0] - v3[0]);
    // betaMat.setElement(0, 2, rayDir[0]);

    // betaMat.setElement(1, 0, v1[1] - rayBegin[1]);
    // betaMat.setElement(1, 1, v1[1] - v3[1]);
    // betaMat.setElement(1, 2, rayDir[1]);

    // betaMat.setElement(2, 0, v1[2] - rayBegin[2]);
    // betaMat.setElement(2, 1, v1[2] - v3[2]);
    // betaMat.setElement(2, 2, rayDir[2]);


    // double beta = betaMat.getDeterminant() / detA;

    double betaMat[3][3];
    betaMat[0][0] = v1[0] - rayBegin[0];
    betaMat[0][1] = v1[0] - v3[0];
    betaMat[0][2] = rayDir[0];

    betaMat[1][0] = v1[1] - rayBegin[1];
    betaMat[1][1] = v1[1] - v3[1];
    betaMat[1][2] = rayDir[1];

    betaMat[2][0] = v1[2] - rayBegin[2];
    betaMat[2][1] = v1[2] - v3[2];
    betaMat[2][2] = rayDir[2];

    double beta = determinantThree(betaMat) / detA;

    

    // MyMatrix gammaMat = MyMatrix(3, 3,0);
    // gammaMat.setElement(0, 0, v1[0] - v2[0]);
    // gammaMat.setElement(0, 1, v1[0] - rayBegin[0]);
    // gammaMat.setElement(0, 2, rayDir[0]);

    // gammaMat.setElement(1, 0, v1[1] - v2[1]);
    // gammaMat.setElement(1, 1, v1[1] - rayBegin[1]);
    // gammaMat.setElement(1, 2, rayDir[1]);

    // gammaMat.setElement(2, 0, v1[2] - v2[2]);
    // gammaMat.setElement(2, 1, v1[2] - rayBegin[2]);
    // gammaMat.setElement(2, 2, rayDir[2]);

    // double gamma = gammaMat.getDeterminant() / detA;

    double gammaMat[3][3];
    gammaMat[0][0] = v1[0] - v2[0];
    gammaMat[0][1] = v1[0] - rayBegin[0];
    gammaMat[0][2] = rayDir[0];

    gammaMat[1][0] = v1[1] - v2[1];
    gammaMat[1][1] = v1[1] - rayBegin[1];
    gammaMat[1][2] = rayDir[1];

    gammaMat[2][0] = v1[2] - v2[2];
    gammaMat[2][1] = v1[2] - rayBegin[2];
    gammaMat[2][2] = rayDir[2];

    double gamma = determinantThree(gammaMat) / detA;


    // MyMatrix tMat = MyMatrix(3, 3,0);
    // tMat.setElement(0, 0, v1[0] - v2[0]);
    // tMat.setElement(0, 1, v1[0] - v3[0]);
    // tMat.setElement(0, 2, v1[0] - rayBegin[0]);

    // tMat.setElement(1, 0, v1[1] - v2[1]);
    // tMat.setElement(1, 1, v1[1] - v3[1]);
    // tMat.setElement(1, 2, v1[1] - rayBegin[1]);

    // tMat.setElement(2, 0, v1[2] - v2[2]);
    // tMat.setElement(2, 1, v1[2] - v3[2]);
    // tMat.setElement(2, 2, v1[2] - rayBegin[2]);

    // double t = tMat.getDeterminant() / detA;

    double tMat[3][3];
    tMat[0][0] = v1[0] - v2[0];
    tMat[0][1] = v1[0] - v3[0];
    tMat[0][2] = v1[0] - rayBegin[0];

    tMat[1][0] = v1[1] - v2[1];
    tMat[1][1] = v1[1] - v3[1];
    tMat[1][2] = v1[1] - rayBegin[1];

    tMat[2][0] = v1[2] - v2[2];
    tMat[2][1] = v1[2] - v3[2];
    tMat[2][2] = v1[2] - rayBegin[2];

    double t = determinantThree(tMat) / detA;
    
    if(beta+gamma>1 || beta<0 || gamma<0 || t<0){
        t = -1;
    }
    return t;
}

double intersectQuadrilateral(double rayBegin[], double rayDir[], double v1[], double v2[], double v3[], double v4[]){
    double t = farPlane;
    double temp = intersectTriangle(rayBegin, rayDir, v1, v2, v3);
    if(temp > nearPlane && temp < t){
        t = temp;
    }
    temp = intersectTriangle(rayBegin, rayDir, v1, v3, v4);
    if(temp > nearPlane && temp < t){
        t = temp;
    }
    return t;
}

double intersectPyramid(double rayBegin[], double rayDir[], pyramid p ){
    double t = farPlane;
    double temp = intersectTriangle(rayBegin, rayDir, p.points[0], p.points[1], p.points[4]);
    
    // sides
    if(temp > nearPlane && temp < t){
        t = temp;
    }
    temp = intersectTriangle(rayBegin, rayDir, p.points[1], p.points[2], p.points[4]);
    if(temp > nearPlane && temp < t){
        t = temp;
    }
    temp = intersectTriangle(rayBegin, rayDir, p.points[2], p.points[3], p.points[4]);
    if(temp > nearPlane && temp < t){
        t = temp;
    }
    temp = intersectTriangle(rayBegin, rayDir, p.points[3], p.points[0], p.points[4]);
    if(temp > nearPlane && temp < t){
        t = temp;
    }

    //base
    temp = intersectTriangle(rayBegin, rayDir, p.points[0], p.points[1], p.points[2]);
    if(temp > nearPlane && temp < t){
        t = temp;
    }
    temp = intersectTriangle(rayBegin, rayDir, p.points[0], p.points[2], p.points[3]);
    if(temp > nearPlane && temp < t){
        t = temp;
    }

    return t;
}

double ***pointBuffer;

void initPointBuffer(int screen_height = numPixels, int screen_width = numPixels, int zm = farPlane, int r =0, int g = 0, int b = 0)
{
    pointBuffer = new double **[screen_height];
    for (int i = 0; i < screen_height; ++i)
    {
        pointBuffer[i] = new double *[screen_width];
        for (int j = 0; j < screen_width; ++j)
        {
            pointBuffer[i][j] = new double[4];
            pointBuffer[i][j][3] = zm;
            pointBuffer[i][j][0] = r;
            pointBuffer[i][j][1] = g;
            pointBuffer[i][j][2] = b;
        }
    }
}

void freePointBuffer(int screen_height = numPixels, int screen_width = numPixels)
{
    for (int i = 0; i < screen_height; ++i)
    {
        for (int j = 0; j < screen_width; ++j)
        {
            delete[] pointBuffer[i][j];
        }
        delete[] pointBuffer[i];
    }
    delete[] pointBuffer;
}

void capture()
{
    initPointBuffer();
    dir_x = lookat_x - eyeat_x;
    dir_y = lookat_y - eyeat_y;
    dir_z = lookat_z - eyeat_z;

    dirNorm = 1 / sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z);

    dir_x *= dirNorm, dir_y *= dirNorm, dir_z *= dirNorm;

    double right_x = -up_y * dir_z + up_z * dir_y;
    double right_y = -up_z * dir_x + up_x * dir_z;
    double right_z = -up_x * dir_y + up_y * dir_x;

    // double planeDistance = (windowHeight / 2.0) / tan(fovY / 2.0);
    double planeDistance = nearPlane;
    double windowHeight = 2 * planeDistance * tan(fovY / 2.0 * PI / 180);
    // double windowHeight = 4 * planeDistance * tan(fovY / 2.0 * PI / 180); //works but shifted down
    double windowWidth = 2 * planeDistance * tan(fovX / 2.0 * PI / 180);
    printf("windowHeight: %f windowWidth: %f\n", windowHeight, windowWidth);
    
    printf("lenght of dir: %f\n", sqrt(dir_x * dir_x + dir_y * dir_y + dir_z * dir_z));
    printf("lenght of right: %f\n", sqrt(right_x * right_x + right_y * right_y + right_z * right_z));
    printf("lenght of up: %f\n", sqrt(up_x * up_x + up_y * up_y + up_z * up_z));

    double topLeft_x = eyeat_x + dir_x * planeDistance - right_x * (windowWidth / 2.0) + up_x * (windowHeight / 2.0);
    double topLeft_y = eyeat_y + dir_y * planeDistance - right_y * (windowWidth / 2.0) + up_y * (windowHeight / 2.0);
    double topLeft_z = eyeat_z + dir_z * planeDistance - right_z * (windowWidth / 2.0) + up_z * (windowHeight / 2.0);

    double du = (windowWidth) / numPixels;
    double dv = (windowHeight) / numPixels;

    topLeft_x = topLeft_x + right_x * (du / 2.0) - up_x * (dv / 2.0);
    topLeft_y = topLeft_y + right_y * (du / 2.0) - up_y * (dv / 2.0);
    topLeft_z = topLeft_z + right_z * (du / 2.0) - up_z * (dv / 2.0);

    // int closest;
    double closest_t = farPlane, t;

    for (int i = 0; i < numPixels; i++)
    { // along width, so columns, so y
        for (int j = 0; j < numPixels; j++)
        { // along height, so rows so x
            // double currPixel_x = topLeft_x + j * right_x * (du) - i * up_x * (dv / 2.0);
            // double currPixel_y = topLeft_y + j * right_y * (du) - i * up_y * (dv / 2.0);
            // double currPixel_z = topLeft_z + j * right_z * (du) - i * up_z * (dv / 2.0);

            // double currRay_x = currPixel_x - eyeat_x;
            // double currRay_y = currPixel_y - eyeat_y;
            // double currRay_z = currPixel_z - eyeat_z;

            // double currRayNorm = 1 / sqrt(currRay_x * currRay_x + currRay_y * currRay_y + currRay_z * currRay_z);

            // currRay_x *= currRayNorm, currRay_y *= currRayNorm, currRay_z *= currRayNorm;

            closest_t = farPlane;
            double color_r = 0, color_g = 0, color_b = 0;

            double currPixel[3];
            currPixel[0] = topLeft_x + j * right_x * (du)-i * up_x * (dv);
            currPixel[1] = topLeft_y + j * right_y * (du)-i * up_y * (dv);
            currPixel[2] = topLeft_z + j * right_z * (du)-i * up_z * (dv);

            double currRay[3];
            currRay[0] = currPixel[0] - eyeat_x;
            currRay[1] = currPixel[1] - eyeat_y;
            currRay[2] = currPixel[2] - eyeat_z;

            double currRayNorm = 1 / sqrt(currRay[0] * currRay[0] + currRay[1] * currRay[1] + currRay[2] * currRay[2]);

            currRay[0] *= currRayNorm, currRay[1] *= currRayNorm, currRay[2] *= currRayNorm;

            double eyeat[3];
            eyeat[0] = eyeat_x;
            eyeat[1] = eyeat_y;
            eyeat[2] = eyeat_z;

            int objtype = 0; // 0 --> no intersection, 1--> sphere, 2--> cube, 3--> pyramid
            sphere closestSphere;
            cube closestCube;
            pyramid closestPyramid;
            for(int i =0; i< numSpheres; i++){
                // t = intersectSphere(eyeat, currRay, spheres[i]);
                t = intersectSphere(currPixel, currRay, spheres[i]);
                if(t>0 && t<closest_t){
                    closest_t = t;
                    // closest = i;
                    objtype = 1;
                    closestSphere = spheres[i];
                }
            }
            for(int i=0;i<numCubes;i++){
                // t = intersectCube(eyeat, currRay, cubes[i]);
                t = intersectCube(currPixel, currRay, cubes[i]);
                if(t>0 && t<closest_t){
                    closest_t = t;
                    // closest = i;
                    objtype = 2;
                    closestCube = cubes[i];
                }
            }

            for(int i=0;i<numPyramids;i++){
                // t = intersectPyramid(eyeat, currRay, pyramids[i]);
                t = intersectPyramid(currPixel, currRay, pyramids[i]);
                if(t>0 && t<closest_t){
                    closest_t = t;
                    // closest = i;
                    objtype = 3;
                    closestPyramid = pyramids[i];
                }
            }

            if(t<farPlane){
                if(objtype == 3){
                    color_r = closestPyramid.r;
                    color_g = closestPyramid.g;
                    color_b = closestPyramid.b;
                }
                else if(objtype == 2){
                    color_r = closestCube.r;
                    color_g = closestCube.g;
                    color_b = closestCube.b;
                }
                else if(objtype == 1){
                    color_r = closestSphere.r;
                    color_g = closestSphere.g;
                    color_b = closestSphere.b;
                }
                else{
                    color_r = 0;
                    color_g = 0;
                    color_b = 0;
                }
                pointBuffer[i][j][0] = color_r;
                pointBuffer[i][j][1] = color_g;
                pointBuffer[i][j][2] = color_b;
                pointBuffer[i][j][3] = closest_t;
            }





        }
    }

    double screen_height = numPixels;
    double screen_width = numPixels;
    bitmap_image image(screen_width, screen_height);
    // start from topleft
    for (int i = 0; i < screen_height; i++)
    {
        for (int j = 0; j < screen_width; j++)
        {
            image.set_pixel(j, i, pointBuffer[i][j][0] *255, pointBuffer[i][j][1] *255, pointBuffer[i][j][2] *255);
        }
    }
    image.save_image("out.bmp");
    // cout << "test.bmp saved" << endl;
    freePointBuffer(screen_height, screen_width);
    image.clear();
}

void testTriangleIntersection(){
    // double rayBegin[3] = {1, 0, 0};
    // double rayDir[3] = {-.2, 0, 1};
    // double vertex1[3] = {-2, 2, 6};
    // double vertex2[3] = {2, 2, 6};
    // double vertex3[3] = {0,-4,6};
    
    // double rayBegin[3] = {0, 0, 0};
    // double rayDir[3] = {0, 0, 1};
    // double vertex1[3] = {-2, 2, 1};
    // double vertex2[3] = {2, 2, 1};
    // double vertex3[3] = {0,-4,1};

    double rayBegin[3] = {0, 0, 0};
    double rayDir[3] = {0.68, -1.14, 1.82};
    double vertex1[3] = {-10, -2.3, 0};
    double vertex2[3] = {4.4,20.3,9.5};
    double vertex3[3] = {9.8,-10,0};

    double t = intersectTriangle(rayBegin, rayDir, vertex1, vertex2, vertex3);
    printf("t: %f\n", t);
    printf("point of intersections: %f %f %f\n", rayBegin[0] + t*rayDir[0], rayBegin[1] + t*rayDir[1], rayBegin[2] + t*rayDir[2]);
    
}

/* Main function: GLUT runs as a console application starting at main()  */
int main(int argc, char **argv)
{
    glutInit(&argc, argv);                                    // Initialize GLUT
    glutInitWindowSize(windowWidth, windowHeight);            // Set the window's initial width & height
    glutInitWindowPosition(50, 50);                           // Position the window's initial top-left corner
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB); // Depth, Double buffer, RGB color
    glutCreateWindow("OpenGL 3D Drawing");                    // Create a window with the given title
    // testTriangleIntersection();
    calcVertices();
    takeinputs();
    glutDisplayFunc(display);            // Register display callback handler for window re-paint
    capture();
    glutReshapeFunc(reshapeListener);    // Register callback handler for window re-shape
    glutKeyboardFunc(keyboardListener);  // Register callback handler for normal-key event
    glutSpecialFunc(specialKeyListener); // Register callback handler for special-key event
    initGL();                            // Our own OpenGL initialization
    glutMainLoop();                      // Enter the event-processing loop
    return 0;
}
