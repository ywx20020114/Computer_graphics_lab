#include<gl/glut.h>
#include<math.h>
#include<iostream>
using namespace std;
void myDisplay(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glBegin(GL_LINES);
    glVertex2f(-1.0, 0.0);
    glVertex2f(1.0, 0.0); //x轴
    glVertex2f(0.0, -1.0);
    glVertex2f(0.0, 1.0); //y轴
    glEnd();
    glBegin(GL_LINE_STRIP);     //设置折线模式
    
    for (float x = -20.0  ; x < 20.0 ; x += 0.01)
    {

        glVertex2f(x*0.05, sin(x)/x);
    }
    glEnd();
    glFlush();
}

int main(int argc, char* argv[])
{
    glutInit(&argc, argv); //对glut初始化，必须在其他的glut使用之前调用一次
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
   
    glutInitWindowPosition(100, 100);

    glutInitWindowSize(400, 400);

    glutCreateWindow("1、draw the function");

    glutDisplayFunc(&myDisplay);

    glutMainLoop();
    return 0;
}
