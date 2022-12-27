#include <GL/glut.h>

#define LEFT 1//0001
#define RIGHT 2//0010
#define BOTTOM 4//0100
#define TOP 8//1000

//int x1 = 150, y1 = 50, x2 = 50, y2 = 250;
int x1 = 350, y1 = 50, x2 = 300, y2 = 250;

//int x1_init = 150, y1_init = 50, x2_init = 50, y2_init = 250;  
int x1_init = 350, y1_init = 50, x2_init = 300, y2_init = 250;


int XL = 100, XR = 400, YB = 100, YT = 300;  // 矩形边界

void encode(int x, int y, int& c)   // 进行编码
{
	c = 0;
	if (x < XL) c |= LEFT;
	else if (x > XR) c |= RIGHT;
	if (y < YB) c |= BOTTOM;
	else if (y > YT) c |= TOP;
}
/*
上左:1001; 中左:0001; 下左:0101;
上中:1000; 中中:0000 下中:0100
上右:1010  中右:0010  下右:0110
*/
void CS_LineClip()  // 裁剪算法
{
	int x, y;
	int code1, code2, code;
	encode(x1, y1, code1);//编码
	encode(x2, y2, code2);
	while (code1 != 0 || code2 != 0)//只有两个端点同时都在矩形区域里才不使用函数
	{
		if (code1 & code2)//两个都为1才成立，这两个都在同一个区域中 直接不进行处理
			return;
		if (code1 != 0)//如果
			code = code1;
		else
			code = code2;

		if (LEFT & code)//线段与左边界相交
		{
			x = XL;//求出的左边界的交点的x值
			y = y1 + (y2 - y1) * (XL - x1) / (x2 - x1);//求出左边交点的y值
		}
		else if (RIGHT & code)//线段与右边界相交
		{
			x = XR;//求出右边界的交点的x值
			y = y1 + (y2 - y1) * (XR - x1) / (x2 - x1);//根据斜率计算出右边界的交点的y值
		}
		else if (BOTTOM & code)//线段与下边界相交
		{
			y = YB;//求出下边界的交点的y值
			x = x1 + (x2 - x1) * (YB - y1) / (y2 - y1);//根据斜率求出下边界的交点的x值
		}
		else if (TOP & code)//线段与上边界相交
		{
			y = YT;//求出上边界的交点的y值
			x = x1 + (x2 - x1) * (YT - y1) / (y2 - y1);//根据斜率求出上边界的交点的x值
		}
		if (code == code1)
		{
			x1 = x; y1 = y;  encode(x1, y1, code1);
		}
		else
		{
			x2 = x; y2 = y; encode(x2, y2, code2);
		}
	}
}
void mydisplay()  
{
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(2);
	glBegin(GL_LINE_LOOP);
	glVertex2i(XL, YT);//绘制矩形
	glVertex2i(XL, YB);
	glVertex2i(XR, YB);
	glVertex2i(XR, YT);
	glEnd();
	glFlush();
	//绘制未裁剪前的线段
	glBegin(GL_LINES);//绘制直线
	glVertex2i(x1_init, y1_init);
	glVertex2i(x2_init, y2_init);
	glEnd();
	glFlush();
	//用红色绘制裁剪后的线段
	glColor3f(1.0, 0.0, 0.0);
	glBegin(GL_LINES);
	glVertex2i(x1, y1);
	glVertex2i(x2, y2);
	glEnd();
	glFlush();
	glEnd();
	glFlush();
}
int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(500, 100);
	glutInitWindowSize(500, 400);
	glutCreateWindow("lab 3  Cohen-Sutherland Clipper");
	glClearColor(1.0, 1.0, 1.0, 0.0);  
	glMatrixMode(GL_PROJECTION);       
	gluOrtho2D(0.0, 600.0, 0.0, 400.0); 
	CS_LineClip();  
	glutDisplayFunc(&mydisplay);
	glutMainLoop();
	return 0;
}