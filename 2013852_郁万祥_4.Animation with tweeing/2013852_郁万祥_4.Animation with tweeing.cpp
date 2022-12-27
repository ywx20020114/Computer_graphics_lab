#include<iostream>
#include<gl\glut.h>
#include<Windows.h>
using namespace std;

const int screenWidth = 800;
const int screenHeight = 800;
const int windowPositionX = 100;
const int windowPositionY = 100;

class Point {
public:
	double x, y;
	Point(double x, double y) {
		this->x = x;
		this->y = y;
	}
};
//初始化
Point FA[] = { 
	Point(-20, 0), 
	Point(-20, 40), 
	Point(-60, 40), 
	Point(-30, 60), 
	Point(-50, 60), 
	Point(0, 100), 
	Point(50, 60), 
	Point(30, 60), 
	Point(60, 40), 
	Point(20, 40), 
	Point(20, 0) };
Point FB[] = { 
	Point(-30, 0), 
	Point(-30, 90), 
	Point(0, 90), 
	Point(100, 90), 
	Point(100, 70), 
	Point(20, 70), 
	Point(20, 50),
	Point(40, 50), 
	Point(40, 30), 
	Point(20, 30), 
	Point(20, 0) };
Point cur(0, 0);

void moveTo(double x, double y) {
	cur.x = x;
	cur.y = y;
}

void lineTo(double x, double y) {
	glBegin(GL_LINES);
	glVertex2d(cur.x, cur.y);
	glVertex2d(x, y);
	glEnd();
	glFlush();
	cur.x = x;
	cur.y = y;
}

void drawGraph(Point A[]) {
	glBegin(GL_LINES);
	moveTo(A[0].x, A[0].y);
	for (int i = 0; i < 11; i++) {
		lineTo(A[i].x, A[i].y);
	}
	lineTo(A[0].x, A[0].y);
}

Point Tween(Point A, Point B, float t) {
	double X, Y;
	X = A.x + (B.x - A.x) * t;
	Y = A.y + (B.y - A.y) * t;
	return Point(X, Y);
}

void drawTween(Point A[], Point B[], int N, float t) {
	Point sP = Tween(A[0], B[0], t);
	for (int i = 0; i < N; i++) {
		Point P = Tween(A[i], B[i], t);
		if (i == 0)	moveTo(P.x, P.y);
		else
		{
			lineTo(P.x, P.y);
		}
	}
	lineTo(sP.x, sP.y);
}

void setViewport(GLint left, GLint right, GLint bottom, GLint top) {
	glViewport(left, bottom, right - left, top - bottom);
}

void kernelDraw() {
	float t = 0;
	float delT = 0.01;
	int N = 11;	//11个点
	while (true) {
		glClear(GL_COLOR_BUFFER_BIT);
		drawTween(FA, FB, N, t);
		glutSwapBuffers();	
		t += delT;
		Sleep(10);
		if (t > 1.0 || t < 0.0) {	
			delT = -delT;
		}
	}
}

void mydisplay(void)
{

	glClear(GL_COLOR_BUFFER_BIT);  
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(3.0);
	kernelDraw();
}

void setWindow(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top) {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(left, right, bottom, top);
}

void myInit(void) {

	glClearColor(1.0, 1.0, 1.0, 0.0);      
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(3.0);
	setWindow(-screenWidth / 2, screenWidth / 2, -screenHeight / 2, screenHeight / 2);
}


void main(int argc, char** argv)
{
	glutInit(&argc, argv);   
	
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(screenWidth, screenHeight);  
	glutInitWindowPosition(windowPositionX, windowPositionX); 
	glutCreateWindow("lab 4  Animation with tweening");  
	myInit();
	glutDisplayFunc(&mydisplay); 
	glutMainLoop();  
}
