#include<gl/glut.h>
#include<math.h>
#include<iostream>
using namespace std;
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
    glVertex2f(-1.0, 0.0);
    glVertex2f(1.0, 0.0); //x��
    glVertex2f(0.0, -1.0);
    glVertex2f(0.0, 1.0); //y��
    glEnd();
    glBegin(GL_LINE_STRIP);     //��������ģʽ
    
    for (float x = -20.0  ; x < 20.0 ; x += 0.01)
    {

        glVertex2f(x*0.05, sin(x)/x);
    }
    glEnd();
    glFlush();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv); //��glut��ʼ����������������glutʹ��֮ǰ����һ��
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   
    glutInitWindowPosition(100, 100);

    glutInitWindowSize(400, 400);

    glutCreateWindow("1��draw the function");

    glutDisplayFunc(&myDisplay);

    glutMainLoop();
    return 0;
}
