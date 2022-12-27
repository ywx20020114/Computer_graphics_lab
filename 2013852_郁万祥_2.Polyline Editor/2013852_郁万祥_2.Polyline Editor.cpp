#include<Windows.h>
#include<gl/glut.h> 
#include<math.h>
#include<iostream>
#include<vector>


using namespace std;


const int screenWidth = 1200;
const int screenHeight = 800;
const int windowPositionX = 100;
const int windowPositionY = 100;


int signal;

class Point  //  ������
{
public:
	double x, y;
	Point(double x, double y)
	{
		this->x = x;
		this->y = y;
	}
	Point()
	{
	}
};

class Node    // �����ڵ�
{
public:
	Point p;
	Node* next;
	Node* last;
	Node(Point t) :p(t.x, t.y) {
		next = this;
		last = this;
	}
};

Node* operateNode;

class List      // �����ڵ��б�
{
public:
	Node* head;
	int length;
	List()
	{
		head = NULL;
		length = 0;
	}
	void push(Point newpoint)   // ���б��з���һ����
	{
		Node* cur = new Node(newpoint);
		cur->p = newpoint;
		if (this->length == 0)
		{
			cur->next = cur;
			cur->last = cur;
			this->head = cur;
		}
		else
		{
			cur->last = this->head;
			cur->next = this->head->next;
			this->head->next->last = cur;
			this->head->next = cur;
			this->head = cur;
		}
		this->length++;
	}
	Node* search(Point p)    // ���б���Ѱ��һ����
	{
		Node* cur = this->head;
		while (TRUE) {
			if (cur->p.x == p.x && cur->p.y == p.y) {
				break;
			}
			else {
				cur = cur->next;
			}
		}
		return cur;
	}
	void deleteNode(Node *target)    // ���б���ɾ���ڵ�
	{
		if (target->next == target->last) {	/* two Nodes or one Node*/
			if (target->next == target) {	/* only one Node*/
				delete target;
				this->head = NULL;
			}
			else {	/* two Nodes */
				target->next->last = target->next;
				target->next->next = target->next;
				this->head = target->next;
				delete target;
			}
		}
		else {	/* more nodes */
			target->last->next = target->next;
			target->next->last = target->last;
			if (target == this->head) {
				this->head = target->last;
			}
			delete target;
		}
		this->length--;
	}
};

vector<List> Allvector = vector<List>();
vector<List> BaseVector = vector<List>();

void baseInit() {     // �����ʼ�ķ��ӵ���״
	List houseList;
	List windowLList;
	List windowRList;
	List chimneyList;
	//HouseOutlinePoints
	Point pHouseTop(0, 200);
	Point pHouseLeftTop(-200, 100);
	Point pHouseLeftBottom(-200, -100);
	Point pHouseRightTop(200, 100);
	Point pHouseRightBottom(200, -100);

	//leftWindowPoints
	Point pLeftWindowLT(-130, 80);
	Point pLeftWindowLB(-130, 0);
	Point pLeftWindowRT(-50, 80);
	Point pLeftWindowRB(-50, 0);

	//rightWindowPoints
	Point pRightWindowLT(50, 80);
	Point pRightWindowLB(50, 0);
	Point pRightWindowRT(130, 80);
	Point pRightWindowRB(130, 0);

	//chimneyPoints
	Point pChimneyLT(120, 220);
	Point pChimneyLB(120, 140);
	Point pChimneyRT(160, 220);
	Point pChimneyRB(160, 120);

	houseList.push(pHouseLeftTop);
	houseList.push(pHouseTop);
	houseList.push(pHouseRightTop);
	houseList.push(pHouseRightBottom);
	houseList.push(pHouseLeftBottom);

	windowLList.push(pLeftWindowLT);
	windowLList.push(pLeftWindowRT);
	windowLList.push(pLeftWindowRB);
	windowLList.push(pLeftWindowLB);

	windowRList.push(pRightWindowLT);
	windowRList.push(pRightWindowRT);
	windowRList.push(pRightWindowRB);
	windowRList.push(pRightWindowLB);

	chimneyList.push(pChimneyLT);
	chimneyList.push(pChimneyRT);
	chimneyList.push(pChimneyRB);
	chimneyList.push(pChimneyLB);
	Allvector.clear();
	Allvector.push_back(houseList);
	Allvector.push_back(windowLList);
	Allvector.push_back(windowRList);
	Allvector.push_back(chimneyList);
	BaseVector = Allvector;
}

void reBase() {
	Allvector.clear();
	Allvector = BaseVector;
}

void drawBase() {       // ������ʼ�ķ���
	reBase();
	int i = 0;
	while (i < 4) {
		glBegin(GL_LINE_LOOP);
		List* curList = &Allvector[i];
		Node* cur = curList->head;
		int j;
		for (j = 0; j < curList->length; j++) {
			glVertex2d(cur->p.x, cur->p.y);
			cur = cur->next;
		}
		glEnd();
		i++;
	}
}

void transformation(int& x, int& y) {	// ת������
	x = x - screenWidth / 2;
	y = screenHeight / 2 - y;
}


double distanceS(Point p, int x, int y) {
	return (p.x - x) * (p.x - x) + (p.y - y) * (p.y - y);    // �������ĵ��뻭���ĵ�ľ���
}

Node* catchNode(int x, int y) {  // ��ȡָ�������Node
	int i = 0;
	while (i < Allvector.size()) {
		List curList = Allvector[i];
		Node* cur = curList.head;
		int j = 0;
		while (j < curList.length) {
			if (distanceS(cur->p, x, y) <= 25) {   // �������С��25����ô���ǾͿ�������ѡ���������
				return cur;
			}
			else {
				cur = cur->next;
				j++;
			}
		}
		i++;
	}
	return NULL;
}


List* catchList(Node* targetNode) {
	int i = 0;
	while (i < Allvector.size()) {
		List* curList = &Allvector[i];
		Node* cur = curList->head;
		int j = 0;
		while (j < curList->length) {
			if (cur->p.x == targetNode->p.x && cur->p.y == targetNode->p.y) {
				return curList;
			}
			else {
				cur = cur->next;
				j++;
			}
		}
		i++;
	}
}


void refreshAll() {      // ���³�ʼ��
	operateNode = NULL;
}

void newList() {       // ����һ���µ�line
	List newList;
	Allvector.push_back(newList);
}


/*��һ������button GLUT_LEFT_BUTTON  | GLUT_MIDDLE_BUTTON  | GLUT_RIGHT_BUTTON
*
* �ڶ�������  state  GLUT_DOWN  | GLUT_UP
*
*/
void myMouse(int button, int state, int x, int y) {

	transformation(x, y);
	Point newP = Point(x, y);
	if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && signal == 0) {
		if (Allvector.back().length == 0) {   //����ʸ�������һ��Ԫ�ص�length
			glBegin(GL_POINTS);  // ���û��line  ��һ��point
			glVertex2d(newP.x, newP.y);
			glEnd();
			glFlush();
		}
		else {
			Point lastP = Allvector.back().head->p;   // �����һ��line  �������һ���㣬���»���
			glBegin(GL_LINES);
			glVertex2d(lastP.x, lastP.y);
			glVertex2d(newP.x, newP.y);
			glEnd();
			glFlush();
		}
		Allvector.back().push(newP);
	}
	else if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && signal == 1) {
		if (catchNode(x, y) != NULL) {
			operateNode = catchNode(x, y);      // ת��������Node;
		}
	}
	else if (state == GLUT_UP && button == GLUT_LEFT_BUTTON && signal == 1) {
		refreshAll();
	}
	else if (state == GLUT_DOWN && button == GLUT_LEFT_BUTTON && signal == 2) {
		if (catchNode(x, y) != NULL) {
			operateNode = catchNode(x, y);
			List* curList = catchList(operateNode);
			curList->deleteNode(operateNode);
			refreshAll();
			glutPostRedisplay();
		}
	}
}


//  �������̵���
void myKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'b':
		// creat a new ployline
		signal = 0;
		newList();
		break;
	case 'm':
		// drag the point pointed to to a new location
		signal = 1;
		break;
	case 'd':
		// delete the next point pointed to
		signal = 2;
		break;
	case 'r':
		//  erase the screen and redraw all the polylines
		glClear(GL_COLOR_BUFFER_BIT);
		baseInit();
		drawBase();
		glFlush();
		break;
	case 'q':
		//exit from the program
		exit(0);
		break;
	}
}

void drawAll() {    //  �������е������еĵ�
	int i = 0;
	while (i < Allvector.size()) {
		glBegin(GL_LINE_LOOP);
		int j;
		List* curList = &Allvector[i];
		Node* cur = curList->head;
		if (cur == NULL) {
			cout << "������ͷָ��Ϊ��" << endl;
		}
		for (j = 0; j < curList->length; j++) {
			glVertex2d(cur->p.x, cur->p.y);
			cur = cur->next;
		}
		glEnd();
		i++;
	}
}


// display
void mydisplay(void)
{
	glClear(GL_COLOR_BUFFER_BIT);  //clear the screen   ÿ�ζ�����clear Ȼ����drawAll
	glColor3f(0.0f, 0.0f, 0.0f);
	glLineWidth(3.0);
	drawAll();
	glFlush();
}


void myMovedMouse(int x, int y) {
	transformation(x, y);
	if (signal == 1) {
		if (operateNode != NULL) {
			operateNode->p.x = x;
			operateNode->p.y = y;
			glutPostRedisplay();
		}
		//reset the new graph.
	}
}


void myInit(void) {
	//initialize things
	glClearColor(1.0, 1.0, 1.0, 0.0);   //set white background color,r��g��b     
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(3.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(-screenWidth / 2, screenWidth / 2, -screenHeight / 2, screenHeight / 2);
	baseInit();
	drawBase();
	//mylist.clear();
}


int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	glutInitWindowSize(screenWidth, screenHeight);  //set the window size
	glutInitWindowPosition(windowPositionX, windowPositionX); //set the window position
	glutCreateWindow("2��Ployline Editor");
	glutDisplayFunc(&mydisplay);
	glutMouseFunc(&myMouse);
	glutKeyboardFunc(&myKeyboard);
	glutMotionFunc(&myMovedMouse);
	myInit();
	glutMainLoop();

}