#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <vector>

using namespace std;

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float> rand_color(0.0f, 1.0f);	// rand_color(mt)
std::uniform_real_distribution<float> rand_position(-0.8f, 0.8f);	// rand_color(mt)

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
	float color[3];
	bool exist = true;

	// �簢�� ���� ��ġ ����, ũ�� ����, ���� ��
	DRect(float size)
		:DPoint(rand_position(mt), rand_position(mt)) {
		float gl_w;
		float gl_h;

		// gl ũ��� ��ȯ
		gl_size(size, gl_w, gl_h);

		width = gl_w;
		height = gl_h;

		for (auto& c : color)
			c = rand_color(mt);
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
};

// ������ ��ǥ�踦 OpenGL ��ǥ��� ��ȯ
void window_to_gl(float& x, float& y);
float gl_size_h(float size);
float gl_size_w(float size);

// �簢���� ���� ��ġ���� Ȯ��
bool check_clash(DRect a, DRect b);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int, int, int, int);
GLvoid Motion(int x, int y);
GLvoid keyboard(unsigned char key, int x, int y);

vector <DRect> rt;

bool left_button = false;
bool timerActive = true;  // Ÿ�̸Ӱ� Ȱ��ȭ �������� ����


void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(50, 100); // �������� ��ġ ����
	glutInitWindowSize(800, 800); // �������� ũ�� ����
	glutCreateWindow("�ǽ� 1 - 5"); // ������ ����(������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew �ʱ�ȭ
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
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
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // ������ ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	for (int i = 0; i < rt.size(); ++i) {
		if (rt[i].exist) {
			glColor3f(rt[i].color[0], rt[i].color[1], rt[i].color[2]);
			glRectf(rt[i].x, rt[i].y, rt[i].topRightX(), rt[i].topRightY());
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
	timerActive = true;

	switch (key) {
	case 'r':		// ��� ���� ����,  count = 0;
		rt.clear();
		rt.shrink_to_fit();
		for (int i = 0; i < 20; ++i) {
			rt.emplace_back(30);
		}

		// ���찳
		rt.emplace_back(60);
		rt[rt.size() - 1].exist = false;

		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
}

GLvoid Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		float gl_x = x;
		float gl_y = y;
		window_to_gl(gl_x, gl_y);

		if (!rt.empty()) {
			float gl_w, gl_h;
			gl_size(60, gl_w, gl_h);

			// ������ ���ҿ� ��ǥ �Ҵ�
			rt[rt.size() - 1].x = gl_x - gl_w / 2;
			rt[rt.size() - 1].y = gl_y - gl_h / 2;
			rt[rt.size() - 1].width = gl_w;
			rt[rt.size() - 1].height = gl_h;
			for (auto& r : rt[rt.size() - 1].color) {
				r = 0.0f;
			}

			rt[rt.size() - 1].exist = true;
		}
		else {
			// rt�� ��������� ��ǥ�� �������� ����
			std::cerr << "rt ���Ͱ� ��� �ֽ��ϴ�." << std::endl;
		}

		left_button = true;

		glutPostRedisplay();  // ȭ���� �����Ͽ� ���� ��ȭ�� �ݿ�
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = false;
		rt[rt.size() - 1].exist = false;

		glutPostRedisplay();  // ȭ���� �����Ͽ� ���� ��ȭ�� �ݿ�
	}
}

GLvoid Motion(int x, int y) {
	if (left_button) {
		float gl_x = x;
		float gl_y = y;
		window_to_gl(gl_x, gl_y);

		if (!rt.empty()) {
			float gl_w, gl_h;

			gl_size(60, gl_w, gl_h);

			// ������ ���ҿ� ��ǥ �Ҵ�
			rt[rt.size() - 1].x = gl_x - gl_w / 2;
			rt[rt.size() - 1].y = gl_y - gl_h / 2;

			// ���� ������ ��ġ�� �簢���� �ִٸ�!
			for (int i = 0; i < rt.size() - 1; ++i) {
				if (check_clash(rt[i], rt[rt.size() - 1]) && rt[i].exist) {

					gl_size(10, gl_w, gl_h);

					// ���찳 ũ�� up
					rt[rt.size() - 1].width += gl_w;
					rt[rt.size() - 1].height += gl_h;

					// ���찳 �� ���� ������ ����
					rt[rt.size() - 1].color[0] = rt[i].color[0];
					rt[rt.size() - 1].color[1] = rt[i].color[1];
					rt[rt.size() - 1].color[2] = rt[i].color[2];

					// ���� �簢�� ����
					rt[i].exist = false;

					glutPostRedisplay();
					break;
				}
			}

		}
		else {
			// rt�� ��������� ��ǥ�� �������� ����
			std::cerr << "rt ���Ͱ� ��� �ֽ��ϴ�." << std::endl;
		}

		glutPostRedisplay();  // ��ǥ ���� �� �ٽ� �׸���
	}
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