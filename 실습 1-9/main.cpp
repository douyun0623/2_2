#include <iostream>

#include "Scene.h"

// 각종 콜백 함수 미리 선언
void DisplayFunc(void);
void ReshapeFunc(int width, int height);
void KeyboardFunc(unsigned char, int, int);
void KeyboardUpFunc(unsigned char, int, int);
void SpecialFunc(int, int, int);
void SpecialUpFunc(int, int, int);
void MouseFunc(int, int, int, int);
void MotionFunc(int, int);
void TimerFunc(int);

// 전역 변수
constexpr int winWidth = 500, winHeight = 500;

// Scene 클래스... 나중에는 게임에 카메라...플레이어... -> GameFramework
Scene g_scene{ winWidth, winHeight };		// initialize_list --> 추후 들어볼 일 있을것...

int main(int argc, char** argv)
{
	//--- 윈도우 생성하기 (freeglut)
	glutInit(&argc, argv);							// glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);	// 디스플레이 모드 설정
	glutInitWindowPosition(100, 100);				// 윈도우의 위치 지정
	glutInitWindowSize(winWidth, winHeight);			// 윈도우의 크기 지정
	glutCreateWindow("Triangle Program!");				// 윈도우 생성 (윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {					// glew 초기화
		std::cerr << "Unable to initialize GLEW\n";
		exit(EXIT_FAILURE);
	}
	else {
		std::cout << "GLEW Initialized\n";
	}

	glClearColor(1.f, 1.f, 1.f, 1.f);		// 클리어 색상 (아무것도 안했을 때)

	// 초기 설정들
	g_scene.initialize();

	//glEnable(GL_CULL_FACE);			// 컬링 활성화

	// 콜백 함수들
	glutKeyboardFunc(KeyboardFunc);		// 일반 키보드 (abcde..)
	glutKeyboardUpFunc(KeyboardUpFunc);	// 일반 키보드가 떼졌을 때
	glutSpecialFunc(SpecialFunc);		// 특수 키보드 (F1 ~ F12, HOME, CTRL, ALT 등등..)
	glutSpecialUpFunc(SpecialUpFunc);	// 특수 키보드가 떼졌을 때

	glutMouseFunc(MouseFunc);			// 마우스 입력 (좌클릭, 우클릭, 휠클릭... 
	glutMotionFunc(MotionFunc);			// 화면 누르고 드래그

	glutDisplayFunc(DisplayFunc);		// 출력 함수의 지정
	glutReshapeFunc(ReshapeFunc);		// 화면 크기가 변경되었을 때....
	glutTimerFunc(16, TimerFunc, 0);	// 16ms --> 초당 60번

	glutMainLoop(); // 이벤트 처리 시작
}

// 전역변수를 최소화!

// 클래스 사용!, 분할 컴파일!
// .cpp당 하나의 obj가 나오도록 컴파일 - 라이브러리와 obj들을 함께 링크하여 실행파일...

void DisplayFunc(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	// 후면버퍼 지우기

	// 씬을 그린다
	g_scene.draw();

	// 후면버퍼를 전면 버퍼와 바꿔준다!
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
	case 0:			// 업데이트
		g_scene.update();
		glutPostRedisplay();	// 씬을 업데이트 했으니, 화면에 그려줘야 한다..

		glutTimerFunc(16, TimerFunc, 0);
		break;
	}
}
