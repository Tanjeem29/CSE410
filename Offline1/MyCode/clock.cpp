#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<ctime>
#include<iostream>
#include <GL/glut.h>
using namespace std;
#define pi (2*acos(0.0))
#define g (9.81)
#include <chrono>
using namespace std::chrono;

uint64_t timeInMillis(){
	return duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
}

double hourAngle;
double minuteAngle;
double secondAngle;



double secondInc = (6.0 * pi / 180);
double minuteInc = secondInc / 60;
double hourInc = minuteInc / 12;

int secondRefreshMillis = 1000;  // Refresh period in milliseconds
int bobRefresh = 20;  // Refresh period in milliseconds
int refreshMillis = 10;  // Refresh period in milliseconds

double secondLength = 50;
double minuteLength = 50;
double hourLength = 30;

double secondWidth = 2;
double minuteWidth = 3;
double hourWidth = 4;


double centerX = 0;
double centerY =0;

// double pendLength = 109.25;
double pendLength = 100.5;

double alpha;
double omega;
double theta = -1.0 * pi / 3;
double theta2 = theta;

double thetamax = pi / 6;
double t=0;
double theta3 = thetamax;
double t_inc = 0.105;

double translation_amount = 0.3;
double pendBeginRad = 8;
double pendEndRad = 16;

double innerFaceRadius=56;
double outerFaceRadius=63;

double translate_y = 40;

void initTime(){


	time_t time_ptr;
	time_ptr = time(NULL);
	tm* tm_local = localtime(&time_ptr);

	int sec = tm_local->tm_sec;
	int min = tm_local->tm_min;
	int hours = (tm_local->tm_hour);
	hours%=12;
	printf("%d %d %d", hours, min, sec);
	secondAngle = (90 - (double)sec * 6.0) * pi /180.0 ;
	minuteAngle = (90 -  ((double)min + (double)sec/60.0) * 6.0)  * pi /180 ;
	hourAngle = (90 -  (((double)hours*5) + ((double)min /12.0) + ((double)sec/720)  )* 6.0 ) * pi /180;
}





struct point
{
	double x,y,z;
};


void drawCircle(double radius,int segments)
{
    int i;
    struct point points[100];
    glColor3f(0.7,0.7,0.7);
    //generate points
    for(i=0;i<=segments;i++)
    {
        points[i].x=radius*cos(((double)i/(double)segments)*2*pi);
        points[i].y=radius*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glVertex3f(points[i].x,points[i].y,0);
			glVertex3f(points[i+1].x,points[i+1].y,0);
        }
        glEnd();
    }
}

void drawCircleAt(double radius,int segments, double x, double y, double z)
{
	glPushMatrix();
    glTranslatef(x,y,z);
    int i;
    struct point points[100];
    //draw segments using generated points
	double tempAngle = 360 / segments;
	tempAngle = tempAngle * pi / 180;
	double currAngle = 0;
	glBegin(GL_LINE_LOOP);  // All vertices form a single loop of single pixel width
        for(i=0;i<segments;i++)
		{
				glVertex3f(radius*cos(currAngle),radius*sin(currAngle),0);
				currAngle+=tempAngle;
		}
    glEnd();
    
	glPopMatrix();
}

void drawFilledCircleAt(double radius,int segments, double x, double y, double z)
{
	glPushMatrix();
    glTranslatef(x,y,z);
    int i;
    struct point points[100];
    //draw segments using generated points
	double tempAngle = 360 / segments;
	tempAngle = tempAngle * pi / 180;
	double currAngle = 0;
	glBegin(GL_POLYGON);  // All vertices form a single loop of single pixel width
        for(i=0;i<segments;i++)
		{
				glVertex3f(radius*cos(currAngle),radius*sin(currAngle),0);
				currAngle+=tempAngle;
		}
    glEnd();
    
	glPopMatrix();
}


void drawClockFace()
{

	
	
	int i;
	int segments = 60;
    struct point endPoints[100];
	struct point bigClockPoints[100];
	struct point midClockPoints[100];
	struct point smolClockPoints[100];
    glColor3f(0,0,0);
    //generate points
    for(i=0;i<=segments;i++)
    {
        endPoints[i].x=innerFaceRadius*cos(((double)i/(double)segments)*2*pi);
        endPoints[i].y=innerFaceRadius*sin(((double)i/(double)segments)*2*pi);
		bigClockPoints[i].x=(innerFaceRadius-10)*cos(((double)i/(double)segments)*2*pi);
        bigClockPoints[i].y=(innerFaceRadius-10)*sin(((double)i/(double)segments)*2*pi);
		midClockPoints[i].x=(innerFaceRadius-5)*cos(((double)i/(double)segments)*2*pi);
        midClockPoints[i].y=(innerFaceRadius-5)*sin(((double)i/(double)segments)*2*pi);
		smolClockPoints[i].x=(innerFaceRadius-2)*cos(((double)i/(double)segments)*2*pi);
        smolClockPoints[i].y=(innerFaceRadius-2)*sin(((double)i/(double)segments)*2*pi);
    }
    //draw segments using generated points
	glLineWidth(2.0f);
    for(i=0;i<segments;i++)
    {
        glBegin(GL_LINES);
        {
			glColor3f(0,0,0);
			glVertex3f(endPoints[i].x,endPoints[i].y,0);
			glColor3f(0.9,0.78,0);
			if(!(i%15)){
				
				glVertex3f(bigClockPoints[i].x,bigClockPoints[i].y,0);
			}
			else if(!(i%5)){
				// glColor3f(0,0,0);
				glVertex3f(midClockPoints[i].x,midClockPoints[i].y,0);
			}
			else{
				// glColor3f(0,0,0);
				glVertex3f(smolClockPoints[i].x,smolClockPoints[i].y,0);
			}
				
        }
        glEnd();
    }
	glColor3f(0.9,0.9,0.9);
	drawFilledCircleAt(innerFaceRadius, 90,0,0,0);
	glColor3f(1,0.78,0);
	drawFilledCircleAt(innerFaceRadius+3, 90,0,0,0);
	glColor3f(1,0.9,0);
	drawFilledCircleAt(outerFaceRadius, 180, 0,0,0);

	
}

void updateHands(int value) {
	secondAngle-=secondInc;
	minuteAngle-=minuteInc;
	hourAngle-=hourInc;
	glutTimerFunc(secondRefreshMillis, updateHands, 0); // Call next 'updateHands' secondRefreshMillis later
}


void updateBob2(int value) {
	// theta3 = thetamax*cos(sqrt(g/pendLength)*t);
	double period = 2000;
	theta3 = thetamax*cos(2 * M_PI / period * (timeInMillis()%2000));
	// theta3 = thetamax*cos(2 * M_PI / period * timeInMillis());
	glutTimerFunc(bobRefresh, updateBob2, 0); // Call next 'updateBob' secondRefreshMillis later
}


void refresh(int value) {
    glutPostRedisplay();    // Post re-paint request to activate display()
	t+=t_inc;
	// if(theta3 == thetamax) t = 0.0001 ;
	glutTimerFunc(refreshMillis, refresh, 0); // Call next 'refresh' milliseconds later

}


void drawSecondHand(){
	
	struct point handPoints[4];
	double x = secondLength -10;
	double y = secondWidth;

	handPoints[0].x = 0;
	handPoints[0].y = 0;

	handPoints[1].x = x*cos(secondAngle) + y*sin(secondAngle);
	handPoints[1].y = x*sin(secondAngle) - y*cos(secondAngle);
	// glColor3f(1.0,.78,0);
	handPoints[2].x = secondLength * cos(secondAngle);
	handPoints[2].y = secondLength * sin(secondAngle);

	handPoints[3].x = x*cos(secondAngle) - y*sin(secondAngle);
	handPoints[3].y = x*sin(secondAngle) + y*cos(secondAngle);


	glBegin(GL_POLYGON);{
		glColor3f(1.0,.78,0);
		glVertex3d(handPoints[0].x, handPoints[0].y, 0);
		glColor3f(1.0,.9,0);
		glVertex3d(handPoints[1].x, handPoints[1].y, 0);
		glColor3f(.8,.78,0);
		glVertex3d(handPoints[2].x, handPoints[2].y, 0);
		glVertex3d(handPoints[3].x, handPoints[3].y, 0);

	}glEnd();

}
void drawMinuteHand(){

	struct point handPoints[4];
	double x = minuteLength -10;
	double y = minuteWidth;

	handPoints[0].x = 0;
	handPoints[0].y = 0;

	handPoints[1].x = x*cos(minuteAngle) + y*sin(minuteAngle);
	handPoints[1].y = x*sin(minuteAngle) - y*cos(minuteAngle);
	// glColor3f(1.0,.78,0);
	handPoints[2].x = minuteLength * cos(minuteAngle);
	handPoints[2].y = minuteLength * sin(minuteAngle);

	handPoints[3].x = x*cos(minuteAngle) - y*sin(minuteAngle);
	handPoints[3].y = x*sin(minuteAngle) + y*cos(minuteAngle);



	glBegin(GL_POLYGON);{
		glColor3f(0,0,0);
		glVertex3d(handPoints[0].x, handPoints[0].y, 0);
		glColor3f(1.0,.78,0);
		glVertex3d(handPoints[1].x, handPoints[1].y, 0);
		glColor3f(0,0,0);
		glVertex3d(handPoints[2].x, handPoints[2].y, 0);
		glVertex3d(handPoints[3].x, handPoints[3].y, 0);

	}glEnd();
}
void drawHourHand(){

	struct point handPoints[4];
	double x = hourLength -10;
	double y = hourWidth;

	handPoints[0].x = 0;
	handPoints[0].y = 0;

	handPoints[1].x = x*cos(hourAngle) + y*sin(hourAngle);
	handPoints[1].y = x*sin(hourAngle) - y*cos(hourAngle);
	// glColor3f(1.0,.78,0);
	handPoints[2].x = hourLength * cos(hourAngle);
	handPoints[2].y = hourLength * sin(hourAngle);

	handPoints[3].x = x*cos(hourAngle) - y*sin(hourAngle);
	handPoints[3].y = x*sin(hourAngle) + y*cos(hourAngle);


	glBegin(GL_POLYGON);{
		glColor3f(0,0,0);
		// glVertex3d(0, -1*outerFaceRadius, 0);
		// glVertex3d(pendLength * cos(tempTheta), -1 * outerFaceRadius + pendLength * sin(tempTheta), 0);
		glVertex3d(handPoints[0].x, handPoints[0].y, 0);
		glColor3f(1.0,.78,0);
		glVertex3d(handPoints[1].x, handPoints[1].y, 0);
		glColor3f(0,0,0);
		glVertex3d(handPoints[2].x, handPoints[2].y, 0);
		glVertex3d(handPoints[3].x, handPoints[3].y, 0);
		// glVertex3d(0,0, 0);
		// glVertex3d(60,60, 0);
	}glEnd();

}

void drawPendulum2(){
	double tempTheta = -1.0 * pi/2 + theta3;
	double PendBeginX, PendBeginY, PendEndX, PendEndY; 
	PendBeginX = 0;
	PendBeginY = -1*outerFaceRadius;
	PendEndX = pendLength * cos(tempTheta);
	PendEndY = -1 * outerFaceRadius + pendLength * sin(tempTheta);
	struct point handlePoints[4];
	handlePoints[0].x = 0 + pendBeginRad * sin(theta3) + (pendBeginRad/2)*cos(theta3);
	handlePoints[0].y = -1*outerFaceRadius - pendBeginRad * cos(theta3) + (pendBeginRad/2)*sin(theta3);
	handlePoints[1].x = 0 + pendBeginRad * sin(theta3) - (pendBeginRad/2)*cos(theta3);
	handlePoints[1].y = -1*outerFaceRadius - pendBeginRad * cos(theta3) - (pendBeginRad/2)*sin(theta3);
	
	handlePoints[3].x = 0 + (-pendEndRad/2 + pendLength) * sin(theta3) + (pendEndRad/2)*cos(theta3);
	handlePoints[3].y = -1*outerFaceRadius - (-pendEndRad/2 + pendLength) * cos(theta3) + (pendEndRad/2)*sin(theta3);
	handlePoints[2].x = 0 + (-pendEndRad/2 + pendLength) * sin(theta3) - (pendEndRad/2)*cos(theta3);
	handlePoints[2].y = -1*outerFaceRadius - (-pendEndRad/2 + pendLength) * cos(theta3) - (pendEndRad/2)*sin(theta3);


	glColor3f(1,0.9,0);
	drawFilledCircleAt(pendBeginRad,60,PendBeginX + pendBeginRad * cos(tempTheta), PendBeginY + pendBeginRad * sin(tempTheta), 0.0f);
	drawFilledCircleAt(pendEndRad,60, PendEndX - (pendEndRad/2) * cos(tempTheta), PendEndY - (pendEndRad/2) * sin(tempTheta), 0.0f);
	glColor3f(1,0.8,0);
	drawCircleAt(pendBeginRad,60,PendBeginX + pendBeginRad * cos(tempTheta), PendBeginY + pendBeginRad * sin(tempTheta), 0.0f);
	drawCircleAt(pendEndRad,60, PendEndX - (pendEndRad/2) * cos(tempTheta), PendEndY - (pendEndRad/2) * sin(tempTheta), 0.0f);

	glBegin(GL_POLYGON);{
		glColor3f(1,0.78,0);
		glVertex3d(handlePoints[0].x, handlePoints[0].y, 0);
		glVertex3d(handlePoints[1].x, handlePoints[1].y, 0);
		glVertex3d(handlePoints[2].x, handlePoints[2].y, 0);
		glVertex3d(handlePoints[3].x, handlePoints[3].y, 0);
	}glEnd();

	glBegin(GL_LINE_LOOP);{
		glColor3f(1,0.5,0);
		glVertex3d(handlePoints[0].x, handlePoints[0].y, 0);
		glVertex3d(handlePoints[1].x, handlePoints[1].y, 0);
		glVertex3d(handlePoints[2].x, handlePoints[2].y, 0);
		glVertex3d(handlePoints[3].x, handlePoints[3].y, 0);

	}glEnd();
	

	// glColor3f(0,0.4,0);
	
}

void drawMiddleKnob(){
	glColor3f(.8,0.7,0);
	drawFilledCircleAt(1,60,0,0,0);
	glColor3f(0,0,0);
	drawFilledCircleAt(2,60,0,0,0);
	
}

void drawBody(){
	double x = outerFaceRadius + 10;
	double y1 = 100;
	double y2= 60;

	//botommid
	double x2 = innerFaceRadius - 20;
	double y3 = 200;
	double y4= 180;
	double y5= 80;

	//sidebars
	double x5 = x -10, x6 = x5-10, y6_5 = ((y3-y4)/x2)*x5 -y3, y6_6 = ((y3-y4)/x2)*x6 -y3;

	//top
	double x7, y7,y8,y9,diff = 20;
	x7 = x+diff;
	y7 = ((y2-y1)/x) * x7+ y1;
	y8 = y7+diff; 
	y9= y1+ diff;

	// bottomroof
	double bottx = 90;

	// glColor3f(0.388,0.27,0.176);
	// // drawFilledCircleAt(x-5,90,0,0,0);
	// glColor3f(0.69,0.48,0.313);
	// glColor3f(0.73,0.56,0.29);
	// glColor3f(0.44,0.31,0.2);

	double x10, y10,y11,y12, diff2 = 15;
	x10 = x7-10;
	y10 = ((y2-y1)/x) * x10+ y1;
	y11 = y10+diff2; 
	y12= y1+ diff2;

	glBegin(GL_POLYGON);{

		glColor3f(0.44,0.31,0.2);
		glColor3f(0.69,0.48,0.313);
		glColor3f(0.388,0.27,0.176);
		glVertex3d(0, -(y1+bottx), 0);
		glVertex3d(-x10, -(y10+bottx), 0);
		glVertex3d(-x10, -(y11+bottx), 0);
		glColor3f(0.388,0.27,0.176);
		
		glVertex3d(0, -(y12+bottx), 0);
		glColor3f(0.69,0.48,0.313);
		// glColor3f(0.388,0.27,0.176);
		glVertex3d(x10, -(y11+bottx), 0);
		glVertex3d(x10, -(y10+bottx), 0);
		

	}glEnd();
	
	// Middle
	glBegin(GL_POLYGON);{
		glColor3f(0.69,0.48,0.313);
		// glColor3f(0.73,0.56,0.29);
		glVertex3d(0, -y1, 0);
		glVertex3d(x, -y2, 0);
		glVertex3d(x, y2, 0);
		glColor3f(0.388,0.27,0.176);
		// glColor3f(0.69,0.48,0.313);
		glColor3f(0.44,0.31,0.2);
		glVertex3d(0, y1, 0);
		// glColor3f(0.44,0.31,0.2);
		glColor3f(0.388,0.27,0.176);
		glVertex3d(-x, y2, 0);
		glVertex3d(-x, -y2, 0);

	}glEnd();


	glColor3f(0.44,0.31,0.2);
	// glLineWidth(10.0f);
	glBegin(GL_LINE_LOOP);{
	
		glVertex3d(0, -y1, 0);
		glVertex3d(x, -y2, 0);
		glVertex3d(x, y2, 0);
		glVertex3d(0, y1, 0);
		glVertex3d(-x, y2, 0);
		glVertex3d(-x, -y2, 0);

	}glEnd();


	

	// Bottom
	glBegin(GL_POLYGON);{

		glColor3f(0.69,0.48,0.313);
		// glColor3f(0.73,0.56,0.29);
		glVertex3d(0, -y3, 0);
		glVertex3d(x2, -y4, 0);
		glVertex3d(x2, -y5, 0);
		
		glColor3f(0.388,0.27,0.176);
		// glColor3f(0.69,0.48,0.313);
		glVertex3d(-x2, -y5, 0);
		glVertex3d(-x2, -y4, 0);

	}glEnd();

	//SideBars
	
	glBegin(GL_POLYGON);{

		glColor3f(0.69,0.48,0.313);
		// glColor3f(0.73,0.56,0.29);
		// glVertex3d(0, -y3, 0);
		glVertex3d(x5, y6_5, 0);
		glVertex3d(x5, (y5-15), 0);
		
		glColor3f(0.388,0.27,0.176);
		// glColor3f(0.69,0.48,0.313);
		glVertex3d(x6, (y5-15), 0);
		glVertex3d(x6, y6_6, 0);

	}glEnd();

	glBegin(GL_POLYGON);{

		
		glColor3f(0.388,0.27,0.176);
		// glColor3f(0.73,0.56,0.29);
		// glVertex3d(0, -y3, 0);
		glVertex3d(-x5, y6_5, 0);
		glVertex3d(-x5, (y5-15), 0);
		glVertex3d(-x6, (y5-15), 0);
		
		
		// glColor3f(0.69,0.48,0.313);
		glColor3f(0.69,0.48,0.313);
		
		glVertex3d(-x6, y6_6, 0);

	}glEnd();
	

	

	glColor3f(0.73,0.56,0.29);

	//Draw Roof
	glBegin(GL_POLYGON);{

		glColor3f(0.44,0.31,0.2);
		glColor3f(0.69,0.48,0.313);
		glVertex3d(0, y1, 0);
		glVertex3d(x7, y7, 0);
		glVertex3d(x7, y8, 0);
		glColor3f(0.388,0.27,0.176);
		glVertex3d(0, y9, 0);
		glColor3f(0.388,0.27,0.176);
		glVertex3d(-x7, y8, 0);
		glVertex3d(-x7, y7, 0);
		

	}glEnd();
	
	
	
}


void display(){
	
	//clear the display
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0,0,0,0);	//color black
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/********************
	/ set-up camera here
	********************/
	//load the correct matrix -- MODEL-VIEW matrix
	glMatrixMode(GL_MODELVIEW);

	//initialize the matrix
	glLoadIdentity();

    // Apply translation along the y-axis to move face up
    glTranslatef(0.0f, translate_y, 0.0f);


	gluLookAt(0,0,200,	0,0,0,	0,1,0);


	//again select MODEL-VIEW
	glMatrixMode(GL_MODELVIEW);


	/****************************
	/ Add your objects from here
	****************************/
	//add objects

	drawMiddleKnob();
	drawSecondHand();
	drawMinuteHand();
	drawHourHand();
	drawPendulum2();
	drawClockFace();
	drawBody();

	//ADD this line in the end --- if you use double buffer (i.e. GL_DOUBLE)
	glutSwapBuffers();
}


// void animate(){
// 	angle+=0.05;
// 	//codes for any changes in Models, Camera
// 	glutPostRedisplay();
// }

void init(){
	//codes for initialization
	// drawgrid=0;
	// drawaxes=1;
	// cameraHeight=150.0;
	// cameraAngle=1.0;
	// angle=0;

	//clear the screen
	glClearColor(0,0,0,0);

	/************************
	/ set-up projection here
	************************/
	//load the PROJECTION matrix
	glMatrixMode(GL_PROJECTION);

	//initialize the matrix
	glLoadIdentity();

	//give PERSPECTIVE parameters
	gluPerspective(80,	1,	1,	1000.0);
	//field of view in the Y (vertically)
	//aspect ratio that determines the field of view in the X direction (horizontally)
	//near distance
	//far distance
}



int main(int argc, char **argv){
	glutInit(&argc,argv);
	glutInitWindowSize(500, 500);
	glutInitWindowPosition(0, 0);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB);	//Depth, Double buffer, RGB color

	glutCreateWindow("My OpenGL Program");

	init();
	initTime();
	
	glEnable(GL_DEPTH_TEST);	//enable Depth Testing



	glutDisplayFunc(display);	//display callback function

	glutTimerFunc(0, updateHands, 0);             // First timer call immediately
	glutTimerFunc(0, refresh, 0);             // First timer call immediately
	glutTimerFunc(0, updateBob2, 0);


	glutMainLoop();		//The main loop of OpenGL

	return 0;
}
