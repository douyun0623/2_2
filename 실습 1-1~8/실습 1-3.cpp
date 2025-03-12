#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <vector>
#include <cmath>

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
	bool rectExists;
	bool reftButtonSelect;
	float color[3];

	// ������ : ���ϴ� ���� �ʺ�, ����
	DRect(float x, float y, float w, float h)
		:DPoint(x, y), width(w), height(h) {
		for (auto& c : color)
			c = rand_color(mt);
	}

	// �簢�� ���� ��ġ ����, ũ�� ����, ���� ��
	DRect()
		:DPoint(rand_position(mt), rand_position(mt)) {
		float gl_w;
		float gl_h;

		rectExists = true;
		reftButtonSelect = false;

		// gl ũ��� ��ȯ
		gl_size(70.0f, gl_w, gl_h);

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

// �簢���� ���� ��ġ���� Ȯ��
bool check_clash(DRect a, DRect b);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int, int, int, int);
GLvoid Motion(int x, int y);
GLvoid keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

std::vector<DRect> rt;
bool left_button = false;	// �簢���� ������ ���
int s_idx = 0;	// ���õ� �簢�� �ε���
int count_a = 0;
bool timerActive = true;  // Ÿ�̸Ӱ� Ȱ��ȭ �������� ����

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(50, 100); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("�ǽ� 1-1"); // ������ ����(������ �̸�)

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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // ������ ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	for (const auto& d : rt) {
		if(d.rectExists){
			glColor3f(d.color[0], d.color[1], d.color[2]);
			glRectf(d.x, d.y, d.topRightX(), d.topRightY());
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
		// ������ ��ġ�� �ٸ� ������ �簢�� ����
	case 'a':
		if (count_a < 10) {	// 10�� ���� �簢�� ����
			++count_a;
			rt.emplace_back();
		}
		break;
	case 't':
		for (const auto& t : rt) {
			t.print();
		}
		break;
	case 'q':	// ���α׷� ����
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
}

GLvoid Mouse(int button, int state, int x, int y) {
	// gl ��ǥ�� ����
	float gl_x = x;
	float gl_y = y;
	window_to_gl(gl_x, gl_y);

	// ���� ���콺
	if (button == GLUT_LEFT_BUTTON) {
		// Ŭ����
		if (state == GLUT_DOWN) {

			for (int i = 0; i < rt.size(); ++i) {
				if (rt[i].x <= gl_x && gl_x <= rt[i].topRightX() &&
					rt[i].y <= gl_y && gl_y <= rt[i].topRightY() &&
					rt[i].rectExists && rt[i].reftButtonSelect == false) {

					rt[i].reftButtonSelect = true;	// ��Ŭ�� ���õ� �ε���
					s_idx = i;	// ���õ� �簢�� �ε���
					left_button = true;		// �簢�� ���ÿ� ����
					cout << "���õǾ���." << endl;
					break;
				}
			}
		}

		// �� ���: �簢���� ���� �κ�
		if (state == GLUT_UP) {
			if (left_button) {
				for (auto& r : rt) {
					if (r.reftButtonSelect == false && r.rectExists &&
						check_clash(rt[s_idx], r)) {

						// ��ġ�� �簢�� ó��
						r.rectExists = false;  // ��ġ�� �簢�� ����
						rt[s_idx].reftButtonSelect = false;

						// ���ϴ� ��ǥ ���
						float bottomLeftX = min(rt[s_idx].x, r.x);
						float bottomLeftY = min(rt[s_idx].y, r.y);

						// ���� ��ǥ ���
						float topRightX = max(rt[s_idx].topRightX(), r.topRightX());
						float topRightY = max(rt[s_idx].topRightY(), r.topRightY());

						// �簢���� ���ο� ũ�� ���
						rt[s_idx].x = bottomLeftX;
						rt[s_idx].y = bottomLeftY;
						rt[s_idx].width = topRightX - bottomLeftX;
						rt[s_idx].height = topRightY - bottomLeftY;

						// ���� ����
						for (auto& c : rt[s_idx].color) {
							c = rand_color(mt);
						}

						cout << "ũ�Ⱑ �ٲ� �簢�� : ";
						rt[s_idx].print();
						break;
					}
				}

				for (auto& r : rt) {
					r.reftButtonSelect = false;
				}

				left_button = false;
				cout << "�����Ǿ���" << endl;
			}
		}
	}

	glutPostRedisplay();  // ȭ���� �����Ͽ� ���� ��ȭ�� �ݿ�
}

GLvoid Motion(int x, int y) {
	if (left_button) {
		float gl_x = x;
		float gl_y = y;
		window_to_gl(gl_x, gl_y);

		// ���õ� �簢���� ��ǥ ����
		rt[s_idx].x = gl_x - rt[s_idx].width / 2;
		rt[s_idx].y = gl_y - rt[s_idx].height / 2;

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