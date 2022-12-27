#include<gl/glut.h>
#include<iostream>
#include<vector>
#include<cmath>
#include<gl\glut.h>
#include<Windows.h>
using namespace std;
struct Point2D {
    float _x, _y;
    Point2D() {
        _x = 0.0f;
        _y = 0.0f;
    }
    Point2D(const Point2D& p) {
        _x = p._x;
        _y = p._y;
    }
    Point2D(float xx, float yy) {
        _x = xx;
        _y = yy;
    }
    Point2D& operator=(const Point2D& p) {
        _x = p._x;
        _y = p._y;
        return *this;
    }
    Point2D& operator+(const Point2D& p) {
        Point2D temp;
        temp._x = _x + p._x;
        temp._y = _y + p._y;
        return temp;
    }
    Point2D& operator-(const Point2D& p) {
        Point2D temp(_x - p._x, _y - p._y);
        return temp;
    }
    float operator*(const Point2D& p) {
        return _x * p._x + _y * p._y;
    }
    Point2D operator*(const float k) {
        return Point2D(_x * k, _y * k);
    }
    float length() {
        return sqrtf(_x * _x + _y * _y);
    }
    void InverseDir() {
        _x = -_x;
        _y = -_y;
    }
};
struct Line2D {
    Point2D _start;
    Point2D _end;
    float _length;
    Line2D() : _start(), _end() {
        _length = 0.0f;
    }
    Line2D(const Point2D& start, const Point2D& end) : _start(start), _end(end) {}
    Line2D(const Line2D& line) : _start(line._start), _end(line._end) {}
    float length() {
        _length = (_end - _start).length();
    }
    void operator = (const Line2D& line) {
        _start = line._start;
        _end = line._end;
    }
    Point2D GetVector() {
        return _end - _start;
    }
};
struct Rect {
    float _left;
    float _right;
    float _up;
    float _down;
    float width() {
        return _right - _left;
    }
    float height() {
        return _down - _up;
    }
};
struct Polygon {
    int _num;
    Point2D* points;
    Point2D* norms;
    Polygon() {
        _num = 0;
    }
    Polygon(vector<Point2D> p) {
        Set(p);
    }
    ~Polygon() {
        delete[] points;
    }
    void Clear() {
        delete[] points;
    }
    void Set(vector<Point2D> p) {
        _num = p.size();
        points = new Point2D[_num];
        for (int i = 0; i < _num; ++i)
            points[i] = p[i];
        norms = new Point2D[_num];
        ComputeNormals();
    }
    Line2D GetLine(int index) {
        Line2D temp;
        if (index >= 0 && index < _num - 1) {
            temp._start = points[index];
            temp._end = points[index + 1];
        }
        else if (index == _num - 1) {
            temp._start = points[index];
            temp._end = points[0];
        }
        return temp;
    }
    Point2D GetNormal(int index) {
        Point2D temp;
        if (index >= 0 && index < _num) temp = norms[index];

        return temp;
    }
    void ComputeNormals() {
        for (int i = 0; i < _num; ++i) {
            Line2D now = GetLine(i);
            Line2D next;
            if (i == _num - 1)next = GetLine(0);
            else next = GetLine(i + 1);
            Point2D v = now.GetVector();
            Point2D vn = next.GetVector();
            Point2D norm;
            if (v._x != 0) {
                norm._y = 1;
                norm._x = (-v._y) / v._x;
            }
            else {
                norm._x = 1;
                norm._y = (-v._x) / v._y;
            }
            if (norm * vn > 0)norm.InverseDir();
            norms[i] = norm;
        }
    }
    const Point2D& GetPoint(int index) {
        if (index >= 0 && index <= _num)
            return points[index];
        return Point2D();
    }
};
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
Point2D g_Start;
Point2D g_End;
Line2D src;
Line2D dest;
bool acc;
bool buildpoly = true;
struct Polygon g_Poly;
int g_Count;
std::vector<Point2D> g_V;
int Cyrus_Beck(Line2D& src, Line2D& dest, struct Polygon& poly) {
    float tin = 0.0f, tout = 1.0f;
    struct Point2D vec = src.GetVector();
    for (int i = 0; i < poly._num; ++i) {
        struct Line2D line = poly.GetLine(i);
        struct Point2D norm = poly.GetNormal(i);
        float nc = vec * norm;
        if (nc == 0) continue;
        else {
            float hit = (line._start - src._start) * norm / nc;
            if (nc > 0)tout = min(tout, hit);
            else tin = max(tin, hit);
        }
    }
    if (tin <= tout) {
        dest._start = src._start + vec * tin;
        dest._end = src._start + vec * tout;
    }
    return tin > tout;
}
void myInit() {
    std::vector<Point2D> v;
    v.emplace_back();
    g_Count = 0;
    acc = false;
    glClearColor(1.0,1.0,1.0,1.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glPointSize(1.0);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, (GLdouble)SCREEN_WIDTH, (GLdouble)SCREEN_HEIGHT, 0.0);
    glViewport(0.0, SCREEN_WIDTH, 0.0, SCREEN_HEIGHT);
}
void myMouse(int button, int state, int x, int y) {
    if (buildpoly) {
        if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
            g_Poly.Set(g_V);
            g_V.clear();
            buildpoly = false;
            glutPostRedisplay();
        }
        if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
            g_V.emplace_back(x, y);
            glutPostRedisplay();
        }
        return;
    }
    if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
        buildpoly = true;
        g_Count = 0;
        glutPostRedisplay();
        return;
    }
    if (button != GLUT_LEFT_BUTTON || state != GLUT_DOWN)return;
}
void myDisplay() {
    glClear(GL_COLOR_BUFFER_BIT);
    Point2D temp;
    if (buildpoly) {
        glColor3f(0.0f, 0.0f, 0.0f);//Poly
        glPointSize(3.0);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < g_V.size(); ++i) glVertex2d(g_V[i]._x, g_V[i]._y);
        glEnd();
    }
    else {
        glColor3f(0.0f, 0.0f, 0.0f);//Poly
        glPointSize(3.0);
        glBegin(GL_LINE_STRIP);
        for (int i = 0; i < g_Poly._num; ++i) {
            temp = g_Poly.GetPoint(i);
            glVertex2d(temp._x, temp._y);
        }
        temp = g_Poly.GetPoint(0);
        glVertex2d(temp._x, temp._y);
        glEnd();
        for (int i = 0; i < 20; i++) {
            g_Start._x = rand() / 60;
            g_Start._y = rand() / 30;
            g_End._x = rand() / 20;
            g_End._y = rand() / 40;
            src._start = g_Start;
            src._end = g_End;
            acc = !Cyrus_Beck(src, dest, g_Poly);
            glColor3f(0.0f, 0.0f, 1.0f);
            glPointSize(2.0);
            glBegin(GL_LINES);
            glVertex2d(src._start._x, src._start._y);
            glVertex2d(src._end._x, src._end._y);
            if (acc) {
                glColor3f(0.0f, 1.0f, 0.0f);
                glPointSize(3.0);
                glVertex2d(dest._start._x, dest._start._y);
                glVertex2d(dest._end._x, dest._end._y);
            }
            glEnd();
        }
    }
    glFlush();
}

int main(int argc, char* argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(0, 0);
    glutCreateWindow("lab_05");
    glutDisplayFunc(myDisplay);
    glutMouseFunc(myMouse);
    myInit();
    glutMainLoop();
    return 0;
}
