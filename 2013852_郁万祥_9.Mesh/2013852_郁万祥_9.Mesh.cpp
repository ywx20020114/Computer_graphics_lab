#include<GL/glut.h>
#include<iostream>
#include<vector>
using namespace std;
#include<fstream>
class Point {
public:
	float x;
	float y;
	float z;
	Point() {
		x = 0; y = 0; z = 0;
	}
	Point(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}
};
class Face {
public:
	int number;//��¼����ĸ���
	vector<int>index;//��¼��������ֵ
	vector<int>index1;//��¼����������
};
vector<Face>f;//��
vector<Point>point;//��
vector<Point>normal;//����
GLint w = 640; GLint h = 480;
void myDisplay() {
	glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(0.0, 0.0, 0.0);
	glPointSize(1);
	for (int i = 0; i < f.size(); i++) {
		glBegin(GL_LINE_LOOP);
		for (int j = 0; j < f[i].index.size(); j++) {
			int in = f[i].index1[j];    //��������
			int iv = f[i].index[j];    //��������
			glNormal3f(normal[in].x, normal[in].y, normal[in].z);
			glVertex3f(point[iv].x, point[iv].y, point[iv].z);
		}
		glEnd();
	}
	//���������ɫ
	glColor3f(0.0, 0.0, 1.0);
	glBegin(GL_LINES);
	//�������᷽��۲�
	glVertex3f(0, 0, 10);
	glVertex3f(0, 0, -10);
	glVertex3f(10, 0, 0);
	glVertex3f(-10, 0, 0);
	glVertex3f(0, 10, 0);
	glVertex3f(0, -10, 0);
	glEnd();
	glFlush();
}
void openfile() {
	int vn, nn, fn;
	ifstream in("C:\\Users\\yuwan\\OneDrive\\����\\lab9.txt");
	string line;
	if (in) {
		in >> vn >> nn >> fn;
		for (int i = 0; i < vn; i++) {//����������
			float a, b, c;
			in >> a >> b >> c;
			Point p(a, b, c);
			point.push_back(p);
		}
		for (int i = 0; i < nn; i++) {//���뷨������ֵ?
			float a, b, c;
			in >> a >> b >> c;
			Point p(a, b, c);
			normal.push_back(p);
		}
		for (int i = 0; i < fn; i++) {
			int num, p;
			in >> num;
			Face face;
			for (int j = 0; j < num; j++) {
				in >> p;
				face.index.push_back(p);
			}
			for (int j = 0; j < num; j++) {
				in >> p;
				face.index1.push_back(p);
			}
			f.push_back(face);
		}
	}
}
void init() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glOrtho(-2, 2, -2, 2, -2, 2);
	//��ת�Ƕ�
	glRotatef(30, 1, 1, 1);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
}
int main(int argc, char* argv[]) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowPosition(320, 120);
	glutInitWindowSize(w, h);
	glutCreateWindow("Display");
	init();
	openfile();
	glutDisplayFunc(myDisplay);
	glutMainLoop();
	return 0;
}