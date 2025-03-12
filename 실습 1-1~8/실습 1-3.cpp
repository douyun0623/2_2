#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
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

// 윈도우 좌표계를 OpenGL 좌표계로 변환
void window_to_gl(float& x, float& y);
void gl_size(float size, float& w, float& h);

class DPoint {
public:
	float x, y;

	// 생성자
	DPoint(float x = 0, float y = 0) : x(x), y(y) {}

	// 점 출력
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

	// 생성자 : 좌하단 점과 너비, 높이
	DRect(float x, float y, float w, float h)
		:DPoint(x, y), width(w), height(h) {
		for (auto& c : color)
			c = rand_color(mt);
	}

	// 사각형 랜덤 위치 생성, 크기 지정, 랜덤 색
	DRect()
		:DPoint(rand_position(mt), rand_position(mt)) {
		float gl_w;
		float gl_h;

		rectExists = true;
		reftButtonSelect = false;

		// gl 크기로 변환
		gl_size(70.0f, gl_w, gl_h);

		width = gl_w;
		height = gl_h;

		for (auto& c : color)
			c = rand_color(mt);
	}

	// 사각형 정보 출력
	void print() const {
		cout << "사각형 정보 출력 : ";
		DPoint::print();
		cout << ", width : " << width << ", height" << height
			<< "color (" << color[0] << ',' << color[1] << ',' << color[2] << ')' << endl;
	}

	// 사각형 우상단 점 계산
	float topRightX() const { return (x + width); }

	// 사각형 우상단 점 계산
	float topRightY() const { return (y + height); }
};

// 사각형이 서로 겹치는지 확인
bool check_clash(DRect a, DRect b);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int, int, int, int);
GLvoid Motion(int x, int y);
GLvoid keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

std::vector<DRect> rt;
bool left_button = false;	// 사각형을 선택한 경우
int s_idx = 0;	// 선택된 사각형 인덱스
int count_a = 0;
bool timerActive = true;  // 타이머가 활성화 상태인지 추적

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(50, 100); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("실습 1-1"); // 윈도우 생성(윈도우 이름)

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
	glutMotionFunc(Motion);
	glutKeyboardFunc(keyboard);
	glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() //--- 콜백 함수: 그리기 콜백 함수 
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 바탕색 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	for (const auto& d : rt) {
		if(d.rectExists){
			glColor3f(d.color[0], d.color[1], d.color[2]);
			glRectf(d.x, d.y, d.topRightX(), d.topRightY());
		}
	}

	// 그리기 부분 구현: 그리기 관련 부분이 여기에 포함된다.
	glutSwapBuffers(); // 화면에 출력하기
}

GLvoid Reshape(int w, int h) //--- 콜백 함수: 다시 그리기 콜백 함수
{
	glViewport(0, 0, w, h);
}

GLvoid keyboard(unsigned char key, int x = 0, int y = 0)	//--- 키보드 입력 처리
{
	switch (key)
	{
		// 랜덤한 위치에 다른 색상의 사각형 생성
	case 'a':
		if (count_a < 10) {	// 10개 까지 사각형 생성
			++count_a;
			rt.emplace_back();
		}
		break;
	case 't':
		for (const auto& t : rt) {
			t.print();
		}
		break;
	case 'q':	// 프로그램 종료
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
}

GLvoid Mouse(int button, int state, int x, int y) {
	// gl 좌표로 변경
	float gl_x = x;
	float gl_y = y;
	window_to_gl(gl_x, gl_y);

	// 왼쪽 마우스
	if (button == GLUT_LEFT_BUTTON) {
		// 클릭시
		if (state == GLUT_DOWN) {

			for (int i = 0; i < rt.size(); ++i) {
				if (rt[i].x <= gl_x && gl_x <= rt[i].topRightX() &&
					rt[i].y <= gl_y && gl_y <= rt[i].topRightY() &&
					rt[i].rectExists && rt[i].reftButtonSelect == false) {

					rt[i].reftButtonSelect = true;	// 좌클릭 선택된 인덱스
					s_idx = i;	// 선택된 사각형 인덱스
					left_button = true;		// 사각형 선택에 성공
					cout << "선택되었다." << endl;
					break;
				}
			}
		}

		// 땐 경우: 사각형을 놓는 부분
		if (state == GLUT_UP) {
			if (left_button) {
				for (auto& r : rt) {
					if (r.reftButtonSelect == false && r.rectExists &&
						check_clash(rt[s_idx], r)) {

						// 겹치는 사각형 처리
						r.rectExists = false;  // 겹치는 사각형 제거
						rt[s_idx].reftButtonSelect = false;

						// 좌하단 좌표 계산
						float bottomLeftX = min(rt[s_idx].x, r.x);
						float bottomLeftY = min(rt[s_idx].y, r.y);

						// 우상단 좌표 계산
						float topRightX = max(rt[s_idx].topRightX(), r.topRightX());
						float topRightY = max(rt[s_idx].topRightY(), r.topRightY());

						// 사각형의 새로운 크기 계산
						rt[s_idx].x = bottomLeftX;
						rt[s_idx].y = bottomLeftY;
						rt[s_idx].width = topRightX - bottomLeftX;
						rt[s_idx].height = topRightY - bottomLeftY;

						// 색상 변경
						for (auto& c : rt[s_idx].color) {
							c = rand_color(mt);
						}

						cout << "크기가 바뀐 사각형 : ";
						rt[s_idx].print();
						break;
					}
				}

				for (auto& r : rt) {
					r.reftButtonSelect = false;
				}

				left_button = false;
				cout << "해제되었다" << endl;
			}
		}
	}

	glutPostRedisplay();  // 화면을 갱신하여 색상 변화를 반영
}

GLvoid Motion(int x, int y) {
	if (left_button) {
		float gl_x = x;
		float gl_y = y;
		window_to_gl(gl_x, gl_y);

		// 선택된 사각형의 좌표 갱신
		rt[s_idx].x = gl_x - rt[s_idx].width / 2;
		rt[s_idx].y = gl_y - rt[s_idx].height / 2;

		glutPostRedisplay();  // 좌표 갱신 후 다시 그리기
	}
}


// 윈도우 좌표계를 OpenGL 좌표계로 변환
void window_to_gl(float& x, float& y) {
	float windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	float windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	// X 좌표 변환 (OpenGL 좌표계에서 왼쪽이 -1, 오른쪽이 1)
	x = (float)x / (windowWidth / 2) - 1.0f;

	// Y 좌표 변환 (OpenGL 좌표계에서 아래가 -1, 위가 1)
	y = 1.0f - (float)y / (windowHeight / 2);
}

// 윈도우 크기에서 gl 사이즈로 변환
void gl_size(float size, float& w, float& h)
{
	float windowWidth = glutGet(GLUT_WINDOW_WIDTH);
	float windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	w = size / windowWidth * 2.0f;
	h = size / windowHeight * 2.0f;
}

// 사각형이 서로 겹치는지 확인
bool check_clash(DRect a, DRect b) {
	if (a.x < b.topRightX() && a.topRightX() > b.x &&
		a.y < b.topRightY() && a.topRightY() > b.y) {
		return true;
	}
	else {
		return false;
	}
}