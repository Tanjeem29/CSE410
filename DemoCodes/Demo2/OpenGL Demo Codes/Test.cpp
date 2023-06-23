#include<iostream>

#include <GL/glut.h>
// Global variables
GLfloat eyex = 4, eyey = 4, eyez = 4;
GLfloat centerx = 0, centery = 0, centerz = 0;
GLfloat upx = 0, upy = 1, upz = 0;
bool isAxes = true, isCube = false, isPyramid = false;

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
void display(){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glBegin(GL_QUADS); // of the color cube
 
   // Top-face
   glColor3f(0.0f, 0.5f, 0.0f); // green
   glVertex3f(0.5f, 0.5f, -0.5f);
   glVertex3f(-0.5f, 0.5f, -0.5f);
   glVertex3f(-0.5f, 0.5f, 0.5f);
   glVertex3f(0.5f, 0.5f, 0.5f);
 
   // Bottom-face
   glColor3f(0.5f, 0.5f, 0.0f); // orange
   glVertex3f(0.5f, -0.5f, 0.5f);
   glVertex3f(-0.5f, -0.5f, 0.5f);
   glVertex3f(-0.5f, -0.5f, -0.5f);
   glVertex3f(0.5f, -0.5f, -0.5f);
 
   // Front-face
   glColor3f(0.5f, 0.0f, 0.0f); // red
   glVertex3f(0.5f, 0.5f, 0.5f);
   glVertex3f(-0.5f, 0.5f, 0.5f);
   glVertex3f(-0.5f, -0.5f, 0.5f);
   glVertex3f(0.5f, -0.5f, 0.5f);
 
   // Back-face
   glColor3f(0.5f, 0.5f, 0.0f); // yellow
   glVertex3f(0.5f, -0.5f, -0.5f);
   glVertex3f(-0.5f, -0.5f, -0.5f);
   glVertex3f(-0.5f, 0.5f, -0.5f);
   glVertex3f(0.5f, 0.5f, -0.5f);
 
   // Left-face
   glColor3f(0.0f, 0.0f, 0.5f); // blue
   glVertex3f(-0.5f, 0.5f, 0.5f);
   glVertex3f(-0.5f, 0.5f, -0.5f);
   glVertex3f(-0.5f, -0.5f, -0.5f);
   glVertex3f(-0.5f, -0.5f, 0.5f);
 
   // Right-face
   glColor3f(0.5f, 0.0f, 0.5f); // magenta
   glVertex3f(0.5f, 0.5f, -0.5f);
   glVertex3f(0.5f, 0.5f, 0.5f);
   glVertex3f(0.5f, -0.5f, 0.5f);
   glVertex3f(0.5f, -0.5f, -0.5f);
 
	glEnd(); // of the color cube
	glFlush();
}

int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	// glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");
	

	glutDisplayFunc(display);	//display callback function
	
	
	glutMainLoop();		//The main loop of OpenGL

	return 0;
}