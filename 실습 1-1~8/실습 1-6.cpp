#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <vector>
#include <cmath>

const int g_rectNum = 7;

using namespace std;

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float> rand_color(0.0f, 1.0f);	// rand_color(mt)
std::uniform_real_distribution<float> rand_position(-0.8f, 0.8f);	// rand_color(mt)

// ������ ��ǥ�踦 OpenGL ��ǥ��� ��ȯ
void window_to_gl(float& x, float& y);
void gl_size(float size, float& w, float& h);

class DPoint {
public:
	float x, y;

	// ������
	DPoint(float x = 0, float y = 0) : x(x), y(y) {}

	// �� ���
	void print() const {
		std::cout << "(" << x << ", " << y << ")";
	}
};
class DRect : public DPoint {
public:
	float width, height;
	int direction = 0;
	bool reftButtonSelect = false;
	bool isSRexist = false;
	float color[3];
	vector <DRect> s_rt;	// �ɰ����� �簢��

	float dx = 0.0f;
	float dy = 0.0f;

	// �ɰ����� �簢��
	DRect(float x, float y, float w, float h, int key, int dir)
		:DPoint(x, y), width(w), height(h), isSRexist(true){

		// ��ġ ����
		changePose(key, dir);

		// ���� ����
		if (key == 1 && dir == 0 ||(key == 4 && dir == 0)) {	// ��
			dx = 0.0f;
			dy = 0.05f;
		}
		else if (key == 1 && dir == 1 || (key == 4 && dir == 1)) {	// ��
			dx = -0.05f;
			dy = 0.0f;
		}
		else if (key == 1 && dir == 2 || (key == 4 && dir == 2)) {	// ��
			dx = 0.0f;
			dy = -0.05f;
		}
		else if ((key == 1 && dir == 3) || key == 3 || (key == 4 && dir == 3)) {	// ��
			dx = 0.05f;
			dy = 0.0f;
		}
		else if (key == 2 && dir == 4 || (key == 4 && dir == 4)) {	// �»�
			dx = -0.05f;
			dy = 0.05f;
		}
		else if (key == 2 && dir == 5 || (key == 4 && dir == 5)) {	// ���
			dx = 0.05f;
			dy = 0.05f;
		}
		else if (key == 2 && dir == 6 || (key == 4 && dir == 6)) {	// ����
			dx = -0.05f;
			dy = -0.05f;
		}
		else if (key == 2 && dir == 7 || (key == 4 && dir == 7)) {	// ����
			dx = 0.05f;
			dy = -0.05f;
		}
	}

	// �簢�� ���� ��ġ ����, ũ�� ����, ���� ��
	DRect()
		:DPoint(rand_position(mt), rand_position(mt)) {
		float gl_w;
		float gl_h;

		reftButtonSelect = false;

		// gl ũ��� ��ȯ
		gl_size(90.0f, gl_w, gl_h);

		width = gl_w;
		height = gl_h;

		for (auto& c : color)
			c = rand_color(mt);
	}

	void changePose(int key, int dir)	// 3���� ��� 4���� , 4�� ��� 8����
	{
		if (key <= 3) {
			switch (dir) {
			case 1:
			case 5:
				x += width;
				break;
			case 2:
			case 6:
				y += height;
				break;
			case 3:
			case 7:
				x += width;
				y += height;
				break;
			}
		}
	}

	// �簢�� ���� ���
	void print() const {
		cout << "�簢�� ���� ��� : ";
		DPoint::print();
		cout << ", width : " << width << ", height" << height
			<< "color (" << color[0] << ',' << color[1] << ',' << color[2] << ')' << endl;
	}

	// �簢�� ���� �� ���
	float topRightX() const { return (x + width); }

	// �簢�� ���� �� ���
	float topRightY() const { return (y + height); }

	void move()
	{
		x += dx;
		y += dy;

		// x �������� �������
		if (x <= -1.0f || topRightX() >= 1.0f) {
			dx *= -1.0f;
		}// y �������� ������� ���� ����
		else if (y <= -1.0f || topRightY() >= 1.0f){
			dy *= -1.0f;
		}
	}

	void sizeDown()
	{
		float gl_w, gl_h;
		gl_size(0.3f, gl_w, gl_h);
		
		if (width > 0) {
			width -= gl_w;
		}
		if (height > 0) {
			height -= gl_h;
		}
		else {
			width = 0;
			height = 0;
			isSRexist = false;
		}
	}
};

// �簢���� ���� ��ġ���� Ȯ��
bool check_clash(DRect a, DRect b);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int, int, int, int);
GLvoid Motion(int x, int y);
GLvoid keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

int isKey = 0;
bool timerActive = true;  // Ÿ�̸Ӱ� Ȱ��ȭ �������� ����
bool once[2]{ false,false };

vector <DRect> rt;

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(50, 100); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("�ǽ� 1-6"); // ������ ����(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
		for (int i = 0; i < g_rectNum; ++i) {
			rt.emplace_back();
		}
		std::cout << "GLEW Initialized\n";
	}

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����(ȭ���� ũ�� �����)]
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(keyboard);
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // ������ ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	for (const auto& r : rt) {

		if (r.reftButtonSelect == false) {	// Ŭ���� ���� ���� ��� (���� �簢�� ���
			glColor3f(r.color[0], r.color[1], r.color[2]);
			glRectf(r.x, r.y, r.topRightX(), r.topRightY());
		}

		else {		// Ŭ���� ���, �ɰ��� �簢�� ���
			for (const auto& s_t : r.s_rt) {
				glColor3f(r.color[0], r.color[1], r.color[2]);
				glRectf(s_t.x, s_t.y, s_t.topRightX(), s_t.topRightY());
			}
		}

	}

	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLvoid keyboard(unsigned char key, int x = 0, int y = 0)	//--- Ű���� �Է� ó��
{
	switch (key)
	{
	case '1':
		isKey = 1;
		cout << "key�� 1�� �����Ǿ����ϴ�" << endl;
		break;
	case '2':
		isKey = 2;
		cout << "key�� 2�� �����Ǿ����ϴ�" << endl;
		break;
	case '3':
		isKey = 3;
		cout << "key�� 3�� �����Ǿ����ϴ�" << endl;
		break;
	case '4':
		isKey = 4;
		cout << "key�� 4�� �����Ǿ����ϴ�" << endl;
		break;

	case 'r':

		rt.clear();
		rt.shrink_to_fit();
		for (int i = 0; i < g_rectNum; ++i) {
			rt.emplace_back();
		}
		cout << "�簢���� �缳���Ǿ����ϴ�" << endl;

		break;
	}

	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y) {
	// gl ��ǥ�� ����
	float gl_x = x;
	float gl_y = y;
	window_to_gl(gl_x, gl_y);

	for (auto& r : rt) {
		// �簢�� ������ Ŭ������ ���
		if (r.x <= gl_x && gl_x <= r.topRightX() &&
			r.y <= gl_y && gl_y <= r.topRightY() && r.reftButtonSelect == false) {

			r.reftButtonSelect = true;
			cout << "���õǾ���." << endl;

			// ũ�� ���ϱ�
			float gl_w = r.width / 2;
			float gl_h = r.height / 2;
			float gl_w8 = r.width / 3;
			float gl_h8 = r.height / 3;

			// size ���� ũ��� ����
			switch (isKey) {

			case 1:
				
				// ���� �簢�� �߰�
				for (int i = 0; i < 4; ++i) {
					r.s_rt.emplace_back(r.x, r.y, gl_w, gl_h, isKey, i);
				}

				if (once[0] == false) {
					once[0] = true;
					glutTimerFunc(16, TimerFunction, 1);
				}

				break;

			case 2:

				// ���� �簢�� �߰�
				for (int i = 0; i < 4; ++i) {
					r.s_rt.emplace_back(r.x, r.y, gl_w, gl_h, isKey, i + 4);
				}

				if (once[0] == false) {
					once[0] = true;
					glutTimerFunc(16, TimerFunction, 1);
				}

				break;


			case 3:

				// ���� �簢�� �߰�
				for (int i = 0; i < 4; ++i) {
					r.s_rt.emplace_back(r.x, r.y, gl_w, gl_h, isKey, i);	// ��� ���������� �̵�
				}

				if (once[0] == false) {
					once[0] = true;
					glutTimerFunc(16, TimerFunction, 1);
				}

				break;

			case 4:

				// ���� �簢�� �߰�
				for (int i = 0; i < 8; ++i) {
					r.s_rt.emplace_back(r.x, r.y, gl_w8, gl_h8, isKey, i);
				}

				if (once[0] == false) {
					once[0] = true;
					glutTimerFunc(16, TimerFunction, 1);
				}
				break;
			}
		}
	}

	glutPostRedisplay();  // ȭ���� �����Ͽ� ���� ��ȭ�� �ݿ�
}

GLvoid Motion(int x, int y) {
	glutPostRedisplay();  // ��ǥ ���� �� �ٽ� �׸���
}

// ������ ��ǥ�踦 OpenGL ��ǥ��� ��ȯ
void window_to_gl(float& x, float& y) {
	float windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	float windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	// X ��ǥ ��ȯ (OpenGL ��ǥ�迡�� ������ -1, �������� 1)
	x = (float)x / (windowWidth / 2) - 1.0f;

	// Y ��ǥ ��ȯ (OpenGL ��ǥ�迡�� �Ʒ��� -1, ���� 1)
	y = 1.0f - (float)y / (windowHeight / 2);
}

void TimerFunction(int value) {

	switch (value)
	{
	case 1:
		for (auto& r : rt) {
			if (r.reftButtonSelect) {
				for (auto& s : r.s_rt) {

					if (s.isSRexist) {	// ����� Ÿ�̸� ����
						s.sizeDown();	// false�� Ÿ�̸� ��
						s.move();
					}

				}
			}
		}

		glutPostRedisplay();
		glutTimerFunc(50, TimerFunction, 1);
		break;
	}
}

// ������ ũ�⿡�� gl ������� ��ȯ
void gl_size(float size, float& w, float& h)
{
	float windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	float windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	w = size / windowWidth * 2.0f;
	h = size / windowHeight * 2.0f;
}

// �簢���� ���� ��ġ���� Ȯ��
bool check_clash(DRect a, DRect b) {
	if (a.x < b.topRightX() && a.topRightX() > b.x &&
		a.y < b.topRightY() && a.topRightY() > b.y) {
		return true;
	}
	else {
		return false;
	}
}