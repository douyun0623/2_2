#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
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

// 윈도우 좌표계를 OpenGL 좌표계로 변환
void window_to_gl(float& x, float& y) {
	int windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	int windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	// X 좌표 변환 (OpenGL 좌표계에서 왼쪽이 -1, 오른쪽이 1)
	x = (float)x / (windowWidth / 2) - 1.0f;

	// Y 좌표 변환 (OpenGL 좌표계에서 아래가 -1, 위가 1)
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
bool timerActive = true;  // 타이머가 활성화 상태인지 추적

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(50, 100); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("실습 1-2"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		std::cout << "GLEW Initialized\n";

	glutDisplayFunc(drawScene); // 출력 함수의 지정
	glutReshapeFunc(Reshape); // 다시 그리기 함수 지정(화면의 크기 변경시)]
	glutMouseFunc(Mouse);
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(set_color[0], set_color[1], set_color[2], 1.0f); // 바탕색 설정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	for (int i = 0; i < 4; ++i) {
		glColor3f(rt[i].set_color[0], rt[i].set_color[1], rt[i].set_color[2]);
		glRectf(rt[i].p1.first, rt[i].p1.second, rt[i].p2.first, rt[i].p2.second);
	}

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid Mouse(int button, int state, int x, int y) {
	float gl_x = x;
	float gl_y = y;
	//window_to_gl(gl_x, gl_y);
	bool select = false;

	switch (button) {
		// 왼쪽
	case GLUT_LEFT_BUTTON: {  // 왼쪽 마우스 버튼
		window_to_gl(gl_x, gl_y);

		for (int i{}; i < 4; ++i) {
			// 사각형 안쪽을 클릭했을 경우 (OpenGL 좌표계에서 검사)
			if (rt[i].p1.first <= gl_x && gl_x <= rt[i].p2.first &&
				rt[i].p1.second <= gl_y && gl_y <= rt[i].p2.second) {
				select = true;

				// 사각형의 색상 변경
				for (auto& c : rt[i].set_color) {
					c = dist(mt);
				}
			}
		}

		// 선택되지 않았다면 배경색을 랜덤하게 바꾸기
		if (select == false) {
			for (auto& c : set_color) {
				c = dist(mt);
			}
		}

	} break;
		// 오른쪽 클릭
	case GLUT_RIGHT_BUTTON: {
		window_to_gl(gl_x, gl_y);

		for (int i{}; i < 4; ++i) {
			// 사각형 안쪽을 클릭했을 경우 (OpenGL 좌표계에서 검사)
			if (rt[i].p1.first <= gl_x && gl_x <= rt[i].p2.first &&
				rt[i].p1.second <= gl_y && gl_y <= rt[i].p2.second) {

				select = true;

				// 사각형의 크기 변경
				float size_change = 0.1f; // 변경할 크기 (너무 크게 변경하지 않도록 조정)
				float min_size = 0.1f;     // 사각형의 최소 크기

				// 가로, 세로 크기 계산
				float width = rt[i].p2.first - rt[i].p1.first;
				float height = rt[i].p2.second - rt[i].p1.second;

				// 사각형 크기가 최소 크기보다 크면 크기 축소
				if (width > min_size && height > min_size) {
					rt[i].p1.first += size_change;
					rt[i].p1.second += size_change;
					rt[i].p2.first -= size_change;
					rt[i].p2.second -= size_change;
				}

				cout << i << "사각형" << "(" << rt[i].p1.first << ", " << rt[i].p1.second << ")";
				cout << "	(" << rt[i].p2.first << ", " << rt[i].p2.second << ")" << endl;
			}
		}

		// 오른쪽 클릭을 했지만 사각형 안쪽이 선택되지 않았을때
		if (select == false) {
			for (int i = 0; i < 4; ++i) {
				// 마우스 좌표 (OpenGL 좌표계)
				float mouse_x = gl_x;
				float mouse_y = gl_y;

				// 사각형의 구역을 기준으로 최대/최소 크기 제한
				// 사각형이 속한 구역 내에서만 크기 변화
				float max_x, min_x, max_y, min_y;

				// 각 사각형의 구역 설정
				if (i == 0) { // 좌상단 구역
					min_x = -1.0f; max_x = 0.0f;
					min_y = 0.0f; max_y = 1.0f;
				}
				else if (i == 1) { // 우상단 구역
					min_x = 0.0f; max_x = 1.0f;
					min_y = 0.0f; max_y = 1.0f;
				}
				else if (i == 2) { // 좌하단 구역
					min_x = -1.0f; max_x = 0.0f;
					min_y = -1.0f; max_y = 0.0f;
				}
				else if (i == 3) { // 우하단 구역
					min_x = 0.0f; max_x = 1.0f;
					min_y = -1.0f; max_y = 0.0f;
				}

				// 마우스가 해당 구역 안에 있는지 확인
				if (mouse_x >= min_x && mouse_x <= max_x && mouse_y >= min_y && mouse_y <= max_y) {
					// 사각형의 크기 변경 (너무 크게 변경하지 않도록 조정)
					float size_change = 0.1f;

					// 사각형의 새로운 크기를 계산
					float new_p1_x = rt[i].p1.first - size_change;
					float new_p1_y = rt[i].p1.second - size_change;
					float new_p2_x = rt[i].p2.first + size_change;
					float new_p2_y = rt[i].p2.second + size_change;

					// 새로운 크기가 구역을 넘어가지 않도록 경계 제한
					if (new_p1_x >= min_x && new_p2_x <= max_x &&
						new_p1_y >= min_y && new_p2_y <= max_y) {
						// 크기 조정이 구역 내에 있는 경우에만 사각형 크기 변경
						rt[i].p1.first = new_p1_x;
						rt[i].p1.second = new_p1_y;
						rt[i].p2.first = new_p2_x;
						rt[i].p2.second = new_p2_y;
					}
					break; // 하나의 사각형만 크기를 조절하고 루프를 종료
				}
			}
		}



	} break;

		cout << "gl x : " << gl_x << "gl y : " << gl_y << endl;
		glutPostRedisplay();  // 화면을 갱신하여 색상 변화를 반영
	}
}