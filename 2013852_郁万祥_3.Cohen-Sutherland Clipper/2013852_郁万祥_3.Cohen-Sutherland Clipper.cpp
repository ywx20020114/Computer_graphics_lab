#include <GL/glut.h>

#define LEFT 1//0001
#define RIGHT 2//0010
#define BOTTOM 4//0100
#define TOP 8//1000

//int x1 = 150, y1 = 50, x2 = 50, y2 = 250;
int x1 = 350, y1 = 50, x2 = 300, y2 = 250;

//int x1_init = 150, y1_init = 50, x2_init = 50, y2_init = 250;  
int x1_init = 350, y1_init = 50, x2_init = 300, y2_init = 250;


int XL = 100, XR = 400, YB = 100, YT = 300;  // ���α߽�

void encode(int x, int y, int& c)   // ���б���
{
	c = 0;
	if (x < XL) c |= LEFT;
	else if (x > XR) c |= RIGHT;
	if (y < YB) c |= BOTTOM;
	else if (y > YT) c |= TOP;
}
/*
����:1001; ����:0001; ����:0101;
����:1000; ����:0000 ����:0100
����:1010  ����:0010  ����:0110
*/
void CS_LineClip()  // �ü��㷨
{
	int x, y;
	int code1, code2, code;
	encode(x1, y1, code1);//����
	encode(x2, y2, code2);
	while (code1 != 0 || code2 != 0)//ֻ�������˵�ͬʱ���ھ���������Ų�ʹ�ú���
	{
		if (code1 & code2)//������Ϊ1�ų���������������ͬһ�������� ֱ�Ӳ����д���
			return;
		if (code1 != 0)//���
			code = code1;
		else
			code = code2;

		if (LEFT & code)//�߶�����߽��ཻ
		{
			x = XL;//�������߽�Ľ����xֵ
			y = y1 + (y2 - y1) * (XL - x1) / (x2 - x1);//�����߽����yֵ
		}
		else if (RIGHT & code)//�߶����ұ߽��ཻ
		{
			x = XR;//����ұ߽�Ľ����xֵ
			y = y1 + (y2 - y1) * (XR - x1) / (x2 - x1);//����б�ʼ�����ұ߽�Ľ����yֵ
		}
		else if (BOTTOM & code)//�߶����±߽��ཻ
		{
			y = YB;//����±߽�Ľ����yֵ
			x = x1 + (x2 - x1) * (YB - y1) / (y2 - y1);//����б������±߽�Ľ����xֵ
		}
		else if (TOP & code)//�߶����ϱ߽��ཻ
		{
			y = YT;//����ϱ߽�Ľ����yֵ
			x = x1 + (x2 - x1) * (YT - y1) / (y2 - y1);//����б������ϱ߽�Ľ����xֵ
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
	glVertex2i(XL, YT);//���ƾ���
	glVertex2i(XL, YB);
	glVertex2i(XR, YB);
	glVertex2i(XR, YT);
	glEnd();
	glFlush();
	//����δ�ü�ǰ���߶�
	glBegin(GL_LINES);//����ֱ��
	glVertex2i(x1_init, y1_init);
	glVertex2i(x2_init, y2_init);
	glEnd();
	glFlush();
	//�ú�ɫ���Ʋü�����߶�
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