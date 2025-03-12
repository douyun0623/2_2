#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <vector>
#include <cmath>

using namespace std;

#define CHANGE_COLOR 1
#define CHANGE_SIZE 2
#define MOVE_DIAGONAL 3
#define MOVE_ZIGZAG 4

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float> rand_color(0.0f, 1.0f);	// rand_color(mt)
std::uniform_real_distribution<float> rand_position(-0.8f, 0.8f);	// rand_color(mt)
std::uniform_real_distribution<float> rand_size(-4.5f, 4.5f);	// rand_size(mt)
std::uniform_int_distribution<int> rand_distance(0, 3);	// ����

// ������ ��ǥ�踦 OpenGL ��ǥ��� ��ȯ
void window_to_gl(float& x, float& y);
void gl_size(float size, float& w, float& h);
float gl_size_h(float size);
float gl_size_w(float size);

float dis[4][2] = { {0.01f, 0.01f}, {-0.01f, 0.01f},{0.01f, -0.01f},{-0.01f, -0.01f} };

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
	float draw_x, draw_y;
	float draw_w, draw_h;
	float color[3];
	int count_z = 0;

	// ����, �ӵ�
	float dx, dy;

	// ������ : �߾���, ���簢��, ó���׸� ��ġ ����
	DRect(float d_x, float d_y, float size)
	{
		float gl_w = 0;
		float gl_h = 0;

		gl_size(size, gl_w, gl_h);

		float gl_x = d_x - (gl_w / 2);
		float gl_y = d_y - (gl_h / 2);

		this->x = gl_x;
		this->y = gl_y;
		this->width = gl_w;
		this->height = gl_h;
		this->draw_x = gl_x;
		this->draw_y = gl_y;
		this->draw_w = gl_w;
		this->draw_h = gl_h;

		for (auto& c : color)
			c = rand_color(mt);

		int rand_dis = rand_distance(mt);

		this->dx = dis[rand_dis][0];
		this->dy = dis[rand_dis][1];
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

		if (x <= -1.0f || topRightX() >= 1.0f) {
			dx *= -1.0f;
		}
		if (y <= -1.0f || topRightY() >= 1.0f) {
			dy *= -1.0f;
		}
	}

	void move_z() {
		++count_z;
		x += dx;
		y += dy;

		if (count_z > 7) {
			count_z = 0;
			dy *= -1.0f;
		}
		if (x <= -1.0f || topRightX() >= 1.0f) {
			dx *= -1.0f;
		}
		if (y <= -1.0f || topRightY() >= 1.0f) {
			dy *= -1.0f;
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

vector <DRect> rt;
int count_a = 0;
bool timerActive = true;  // Ÿ�̸Ӱ� Ȱ��ȭ �������� ����

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(50, 100); // �������� ��ġ ����
	glutInitWindowSize(800, 800); // �������� ũ�� ����
	glutCreateWindow("�ǽ� "); // ������ ����(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene); // ��� �Լ��� ����
	glutReshapeFunc(Reshape); // �ٽ� �׸��� �Լ� ����(ȭ���� ũ�� �����)]
	glutMouseFunc(Mouse);
	glutMotionFunc(Motion);
	glutKeyboardFunc(keyboard);
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // ������ ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	for (const auto& r : rt) {
		glColor3f(r.color[0], r.color[1], r.color[2]);
		glRectf(r.x, r.y, r.topRightX(), r.topRightY());
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
	timerActive = true;

	switch (key) {
		
	case '1':	// �밢 �ൿ
		glutTimerFunc(30, TimerFunction, MOVE_DIAGONAL);
		break;
	case '2':	//Zigzag
		glutTimerFunc(30, TimerFunction, MOVE_ZIGZAG);
		break;
	case '3':
		glutTimerFunc(100, TimerFunction, CHANGE_SIZE);
		break;
	case '4':	// �� ����
		glutTimerFunc(100, TimerFunction, CHANGE_COLOR);
		break;
	case 's':
		timerActive = false;
		break;
	case 'm':
		timerActive = false;
		for (auto& r : rt) {
			r.x = r.draw_x;
			r.y = r.draw_y;
			r.width = r.draw_w;
			r.height = r.draw_h;
		}
		break;
	case 'r':		// ��� ���� ����,  count = 0;
		count_a = 0;
		timerActive = false;
		rt.clear();
		rt.shrink_to_fit();
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if(timerActive){
		if (value == CHANGE_COLOR) {
			for (auto& r : rt) {
				for (auto& c : r.color) {
					c = rand_color(mt);
				}
			}

			glutPostRedisplay();
			glutTimerFunc(100, TimerFunction, CHANGE_COLOR);
		}

		if (value == CHANGE_SIZE) {
			for (auto& r : rt) {
				float size_w = gl_size_w(10.0f);
				float size_h = gl_size_h(10.0f);

				r.width = max(size_w, r.width + gl_size_w(rand_size(mt)));
				r.height = max(size_h, r.height + gl_size_h(rand_size(mt)));
			}

			glutPostRedisplay();
			glutTimerFunc(100, TimerFunction, CHANGE_SIZE);
		}

		if (value == MOVE_DIAGONAL) {
			for (auto& r : rt) {
				r.move();
			}

			glutPostRedisplay();
			glutTimerFunc(30, TimerFunction, MOVE_DIAGONAL);
		}

		if (value == MOVE_ZIGZAG) {
			for (auto& r : rt) {
				r.move_z();
			}

			glutPostRedisplay();
			glutTimerFunc(30, TimerFunction, MOVE_ZIGZAG);
		}
	}
}

GLvoid Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (count_a < 5) {
			// gl ��ǥ�� ����
			float gl_x = x;
			float gl_y = y;
			window_to_gl(gl_x, gl_y);
			rt.emplace_back(gl_x, gl_y, 50.0f);

			++count_a;
		}

		glutPostRedisplay();  // ȭ���� �����Ͽ� ���� ��ȭ�� �ݿ�
	}
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

// ������ ũ�⿡�� gl ������� ��ȯ
void gl_size(float size, float& w, float& h)
{
	float windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	float windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	w = size / windowWidth * 2.0f;
	h = size / windowHeight * 2.0f;
}

float gl_size_w(float size) {
	float windowWidth = glutGet(GLUT_WINDOW_WIDTH);

	return size / windowWidth * 2.0f;
}

float gl_size_h(float size) {
	float windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	return size / windowHeight * 2.0f;
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