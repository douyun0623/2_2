#include <iostream>

#include "Scene.h"

void DisplayFunc(void);
void ReshapeFunc(int width, int height);
void KeyboardFunc(unsigned char, int, int);
void KeyboardUpFunc(unsigned char, int, int);
void MouseFunc(int, int, int, int);
void MotionFunc(int, int);
void TimerFunc(int);

constexpr int winWidth = 800, winHeight = 800;

Scene g_scene{ winWidth, winHeight };	

int main(int argc, char** argv)
{
	//--- ������ �����ϱ� (freeglut)
	glutInit(&argc, argv);							// glut �ʱ�ȭ
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);	// ���÷��� ��� ����
	glutInitWindowPosition(0, 0);				// �������� ��ġ ����
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

	glClearColor(0.f, 0.9f, 0.8f, 1.f);		// Ŭ���� ���� (�ƹ��͵� ������ ��)

	// �ʱ� ������
	g_scene.initialize();
	glEnable(GL_DEPTH_TEST);
	glFrontFace(GL_CCW);

	// �ݹ� �Լ���
	glutKeyboardFunc(KeyboardFunc);		// �Ϲ� Ű���� (abcde..)
	glutKeyboardUpFunc(KeyboardUpFunc);	// �Ϲ� Ű���尡 ������ ��

	glutMouseFunc(MouseFunc);			// ���콺 �Է� (��Ŭ��, ��Ŭ��, ��Ŭ��... 
	glutMotionFunc(MotionFunc);			// ȭ�� ������ �巡��

	glutDisplayFunc(DisplayFunc);		// ��� �Լ��� ����
	glutReshapeFunc(ReshapeFunc);		// ȭ�� ũ�Ⱑ ����Ǿ��� ��....
	glutTimerFunc(16, TimerFunc, 0);	// 16ms --> �ʴ� 60��

	glutMainLoop(); // �̺�Ʈ ó�� ����
}

void DisplayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// �ĸ���� �����

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
	g_scene.keyboard(key, true);
}

void KeyboardUpFunc(unsigned char key, int, int)
{
	g_scene.keyboard(key, false);
}

void MouseFunc(int button, int state, int x, int y)
{
	g_scene.mouse(button, state, x, y);
}

void MotionFunc(int x, int y)
{
	g_scene.mouseMove(x, y);
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
