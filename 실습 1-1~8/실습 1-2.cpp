#include <iostream>
#include <gl/glew.h> //--- �ʿ��� ������� include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <cmath>

using namespace std;

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float> dist(0.0f, 1.0f);	// dist(mt)

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int, int, int, int);

// ������ ��ǥ�踦 OpenGL ��ǥ��� ��ȯ
void window_to_gl(float& x, float& y) {
	int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	// X ��ǥ ��ȯ (OpenGL ��ǥ�迡�� ������ -1, �������� 1)
	x = (float)x / (windowWidth / 2) - 1.0f;

	// Y ��ǥ ��ȯ (OpenGL ��ǥ�迡�� �Ʒ��� -1, ���� 1)
	y = 1.0f - (float)y / (windowHeight / 2);
}

typedef pair<float, float> point;

struct rect {
	point p1;
	point p2;

	float set_color[3] = { dist(mt), dist(mt),dist(mt) };
};

rect rt[4] = {
	{{-1.0f, 0.0f}, {0.0f, 1.0f}},  // rt[0]
	{{ 0.0f, 0.0f}, {1.0f, 1.0f}},  // rt[1]
	{{-1.0f, -1.0f}, {0.0f, 0.0f}}, // rt[2]
	{{ 0.0f, -1.0f}, {1.0f, 0.0f}}  // rt[3]
};

float set_color[3] = { 1.0f, 1.0f,1.0f };
bool timerActive = true;  // Ÿ�̸Ӱ� Ȱ��ȭ �������� ����

void main(int argc, char** argv) //--- ������ ����ϰ� �ݹ��Լ� ���� 
{ //--- ������ �����ϱ�
	glutInit(&argc, argv); // glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
	glutInitWindowPosition(50, 100); // �������� ��ġ ����
	glutInitWindowSize(800, 600); // �������� ũ�� ����
	glutCreateWindow("�ǽ� 1-2"); // ������ ����(������ �̸�)

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
	glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() //--- �ݹ� �Լ�: �׸��� �ݹ� �Լ� 
{
	glClearColor(set_color[0], set_color[1], set_color[2], 1.0f); // ������ ����
	glClear(GL_COLOR_BUFFER_BIT); // ������ ������ ��ü�� ĥ�ϱ�

	for (int i = 0; i < 4; ++i) {
		glColor3f(rt[i].set_color[0], rt[i].set_color[1], rt[i].set_color[2]);
		glRectf(rt[i].p1.first, rt[i].p1.second, rt[i].p2.first, rt[i].p2.second);
	}

	// �׸��� �κ� ����: �׸��� ���� �κ��� ���⿡ ���Եȴ�.
	glutSwapBuffers(); // ȭ�鿡 ����ϱ�
}

GLvoid Reshape(int w, int h) //--- �ݹ� �Լ�: �ٽ� �׸��� �ݹ� �Լ�
{
	glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y) {
	float gl_x = x;
	float gl_y = y;
	//window_to_gl(gl_x, gl_y);
	bool select = false;

	switch (button) {
		// ����
	case GLUT_LEFT_BUTTON: {  // ���� ���콺 ��ư
		window_to_gl(gl_x, gl_y);

		for (int i{}; i < 4; ++i) {
			// �簢�� ������ Ŭ������ ��� (OpenGL ��ǥ�迡�� �˻�)
			if (rt[i].p1.first <= gl_x && gl_x <= rt[i].p2.first &&
				rt[i].p1.second <= gl_y && gl_y <= rt[i].p2.second) {
				select = true;

				// �簢���� ���� ����
				for (auto& c : rt[i].set_color) {
					c = dist(mt);
				}
			}
		}

		// ���õ��� �ʾҴٸ� ������ �����ϰ� �ٲٱ�
		if (select == false) {
			for (auto& c : set_color) {
				c = dist(mt);
			}
		}

	} break;
		// ������ Ŭ��
	case GLUT_RIGHT_BUTTON: {
		window_to_gl(gl_x, gl_y);

		for (int i{}; i < 4; ++i) {
			// �簢�� ������ Ŭ������ ��� (OpenGL ��ǥ�迡�� �˻�)
			if (rt[i].p1.first <= gl_x && gl_x <= rt[i].p2.first &&
				rt[i].p1.second <= gl_y && gl_y <= rt[i].p2.second) {

				select = true;

				// �簢���� ũ�� ����
				float size_change = 0.1f; // ������ ũ�� (�ʹ� ũ�� �������� �ʵ��� ����)
				float min_size = 0.1f;     // �簢���� �ּ� ũ��

				// ����, ���� ũ�� ���
				float width = rt[i].p2.first - rt[i].p1.first;
				float height = rt[i].p2.second - rt[i].p1.second;

				// �簢�� ũ�Ⱑ �ּ� ũ�⺸�� ũ�� ũ�� ���
				if (width > min_size && height > min_size) {
					rt[i].p1.first += size_change;
					rt[i].p1.second += size_change;
					rt[i].p2.first -= size_change;
					rt[i].p2.second -= size_change;
				}

				cout << i << "�簢��" << "(" << rt[i].p1.first << ", " << rt[i].p1.second << ")";
				cout << "	(" << rt[i].p2.first << ", " << rt[i].p2.second << ")" << endl;
			}
		}

		// ������ Ŭ���� ������ �簢�� ������ ���õ��� �ʾ�����
		if (select == false) {
			for (int i = 0; i < 4; ++i) {
				// ���콺 ��ǥ (OpenGL ��ǥ��)
				float mouse_x = gl_x;
				float mouse_y = gl_y;

				// �簢���� ������ �������� �ִ�/�ּ� ũ�� ����
				// �簢���� ���� ���� �������� ũ�� ��ȭ
				float max_x, min_x, max_y, min_y;

				// �� �簢���� ���� ����
				if (i == 0) { // �»�� ����
					min_x = -1.0f; max_x = 0.0f;
					min_y = 0.0f; max_y = 1.0f;
				}
				else if (i == 1) { // ���� ����
					min_x = 0.0f; max_x = 1.0f;
					min_y = 0.0f; max_y = 1.0f;
				}
				else if (i == 2) { // ���ϴ� ����
					min_x = -1.0f; max_x = 0.0f;
					min_y = -1.0f; max_y = 0.0f;
				}
				else if (i == 3) { // ���ϴ� ����
					min_x = 0.0f; max_x = 1.0f;
					min_y = -1.0f; max_y = 0.0f;
				}

				// ���콺�� �ش� ���� �ȿ� �ִ��� Ȯ��
				if (mouse_x >= min_x && mouse_x <= max_x && mouse_y >= min_y && mouse_y <= max_y) {
					// �簢���� ũ�� ���� (�ʹ� ũ�� �������� �ʵ��� ����)
					float size_change = 0.1f;

					// �簢���� ���ο� ũ�⸦ ���
					float new_p1_x = rt[i].p1.first - size_change;
					float new_p1_y = rt[i].p1.second - size_change;
					float new_p2_x = rt[i].p2.first + size_change;
					float new_p2_y = rt[i].p2.second + size_change;

					// ���ο� ũ�Ⱑ ������ �Ѿ�� �ʵ��� ��� ����
					if (new_p1_x >= min_x && new_p2_x <= max_x &&
						new_p1_y >= min_y && new_p2_y <= max_y) {
						// ũ�� ������ ���� ���� �ִ� ��쿡�� �簢�� ũ�� ����
						rt[i].p1.first = new_p1_x;
						rt[i].p1.second = new_p1_y;
						rt[i].p2.first = new_p2_x;
						rt[i].p2.second = new_p2_y;
					}
					break; // �ϳ��� �簢���� ũ�⸦ �����ϰ� ������ ����
				}
			}
		}



	} break;

		cout << "gl x : " << gl_x << "gl y : " << gl_y << endl;
		glutPostRedisplay();  // ȭ���� �����Ͽ� ���� ��ȭ�� �ݿ�
	}
}