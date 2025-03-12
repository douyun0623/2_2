#include <iostream>

#include "Scene.h"

// ���� �ݹ� �Լ� �̸� ����
void DisplayFunc(void);
void ReshapeFunc(int width, int height);
void KeyboardFunc(unsigned char, int, int);
void KeyboardUpFunc(unsigned char, int, int);
void SpecialFunc(int, int, int);
void SpecialUpFunc(int, int, int);
void MouseFunc(int, int, int, int);
void MotionFunc(int, int);
void TimerFunc(int);

// ���� ����
constexpr int winWidth = 500, winHeight = 500;

// Scene Ŭ����... ���߿��� ���ӿ� ī�޶�...�÷��̾�... -> GameFramework
Scene g_scene{ winWidth, winHeight };		// initialize_list --> ���� �� �� ������...

int main(int argc, char** argv)
{
	//--- ������ �����ϱ� (freeglut)
	glutInit(&argc, argv);							// glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	// ���÷��� ��� ����
	glutInitWindowPosition(100, 100);				// �������� ��ġ ����
	glutInitWindowSize(winWidth, winHeight);			// �������� ũ�� ����
	glutCreateWindow("Triangle Program!");				// ������ ���� (������ �̸�)

	//--- GLEW �ʱ�ȭ�ϱ�
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {					// glew �ʱ�ȭ
		std::cerr << "Unable to initialize GLEW\n";
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "GLEW Initialized\n";
	}

	glClearColor(1.f, 1.f, 1.f, 1.f);		// Ŭ���� ���� (�ƹ��͵� ������ ��)

	// �ʱ� ������
	g_scene.initialize();

	//glEnable(GL_CULL_FACE);			// �ø� Ȱ��ȭ

	// �ݹ� �Լ���
	glutKeyboardFunc(KeyboardFunc);		// �Ϲ� Ű���� (abcde..)
	glutKeyboardUpFunc(KeyboardUpFunc);	// �Ϲ� Ű���尡 ������ ��
	glutSpecialFunc(SpecialFunc);		// Ư�� Ű���� (F1 ~ F12, HOME, CTRL, ALT ���..)
	glutSpecialUpFunc(SpecialUpFunc);	// Ư�� Ű���尡 ������ ��

	glutMouseFunc(MouseFunc);			// ���콺 �Է� (��Ŭ��, ��Ŭ��, ��Ŭ��... 
	glutMotionFunc(MotionFunc);			// ȭ�� ������ �巡��

	glutDisplayFunc(DisplayFunc);		// ��� �Լ��� ����
	glutReshapeFunc(ReshapeFunc);		// ȭ�� ũ�Ⱑ ����Ǿ��� ��....
	glutTimerFunc(16, TimerFunc, 0);	// 16ms --> �ʴ� 60��

	glutMainLoop(); // �̺�Ʈ ó�� ����
}

// ���������� �ּ�ȭ!

// Ŭ���� ���!, ���� ������!
// .cpp�� �ϳ��� obj�� �������� ������ - ���̺귯���� obj���� �Բ� ��ũ�Ͽ� ��������...

void DisplayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	// �ĸ���� �����

	// ���� �׸���
	g_scene.draw();

	// �ĸ���۸� ���� ���ۿ� �ٲ��ش�!
	glutSwapBuffers();
}

void ReshapeFunc(int width, int height)
{
	g_scene.setWindowSize(width, height);

	glutPostRedisplay();

	glViewport(0, 0, width, height);
}

void KeyboardFunc(unsigned char key, int x, int y)
{
	g_scene.keyboard(key);
}

void KeyboardUpFunc(unsigned char key, int, int)
{
	g_scene.keyboardUp(key);
}

void SpecialFunc(int key, int x, int y)
{
	g_scene.specialKeyboard(key);
}

void SpecialUpFunc(int key, int, int)
{
	g_scene.specialKeyboardUp(key);
}

void MouseFunc(int button, int state, int x, int y)
{
	g_scene.mouse(button, state, x, y);
}

void MotionFunc(int x, int y)
{
}

void TimerFunc(int id)
{
	switch (id) {
	case 0:			// ������Ʈ
		g_scene.update();
		glutPostRedisplay();	// ���� ������Ʈ ������, ȭ�鿡 �׷���� �Ѵ�..

		glutTimerFunc(16, TimerFunc, 0);
		break;
	}
}
