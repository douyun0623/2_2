#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
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
	int direction = 0;
	bool reftButtonSelect = false;
	bool isSRexist = false;
	float color[3];
	vector <DRect> s_rt;	// 쪼개지는 사각형

	float dx = 0.0f;
	float dy = 0.0f;

	// 쪼개지는 사각형
	DRect(float x, float y, float w, float h, int key, int dir)
		:DPoint(x, y), width(w), height(h), isSRexist(true){

		// 위치 변경
		changePose(key, dir);

		// 방향 설정
		if (key == 1 && dir == 0 ||(key == 4 && dir == 0)) {	// 상
			dx = 0.0f;
			dy = 0.05f;
		}
		else if (key == 1 && dir == 1 || (key == 4 && dir == 1)) {	// 좌
			dx = -0.05f;
			dy = 0.0f;
		}
		else if (key == 1 && dir == 2 || (key == 4 && dir == 2)) {	// 하
			dx = 0.0f;
			dy = -0.05f;
		}
		else if ((key == 1 && dir == 3) || key == 3 || (key == 4 && dir == 3)) {	// 우
			dx = 0.05f;
			dy = 0.0f;
		}
		else if (key == 2 && dir == 4 || (key == 4 && dir == 4)) {	// 좌상
			dx = -0.05f;
			dy = 0.05f;
		}
		else if (key == 2 && dir == 5 || (key == 4 && dir == 5)) {	// 우상
			dx = 0.05f;
			dy = 0.05f;
		}
		else if (key == 2 && dir == 6 || (key == 4 && dir == 6)) {	// 좌하
			dx = -0.05f;
			dy = -0.05f;
		}
		else if (key == 2 && dir == 7 || (key == 4 && dir == 7)) {	// 우하
			dx = 0.05f;
			dy = -0.05f;
		}
	}

	// 사각형 랜덤 위치 생성, 크기 지정, 랜덤 색
	DRect()
		:DPoint(rand_position(mt), rand_position(mt)) {
		float gl_w;
		float gl_h;

		reftButtonSelect = false;

		// gl 크기로 변환
		gl_size(90.0f, gl_w, gl_h);

		width = gl_w;
		height = gl_h;

		for (auto& c : color)
			c = rand_color(mt);
	}

	void changePose(int key, int dir)	// 3이하 경우 4개로 , 4일 경우 8개로
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

	void move()
	{
		x += dx;
		y += dy;

		// x 범위에서 벗어났을때
		if (x <= -1.0f || topRightX() >= 1.0f) {
			dx *= -1.0f;
		}// y 범위에서 벗어났을때 방향 변경
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

// 사각형이 서로 겹치는지 확인
bool check_clash(DRect a, DRect b);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int, int, int, int);
GLvoid Motion(int x, int y);
GLvoid keyboard(unsigned char key, int x, int y);
GLvoid TimerFunction(int value);

int isKey = 0;
bool timerActive = true;  // 타이머가 활성화 상태인지 추적
bool once[2]{ false,false };

vector <DRect> rt;

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(50, 100); // 윈도우의 위치 지정
	glutInitWindowSize(800, 600); // 윈도우의 크기 지정
	glutCreateWindow("실습 1-6"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
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

	for (const auto& r : rt) {

		if (r.reftButtonSelect == false) {	// 클릭이 되지 않은 경우 (메인 사각형 출력
			glColor3f(r.color[0], r.color[1], r.color[2]);
			glRectf(r.x, r.y, r.topRightX(), r.topRightY());
		}

		else {		// 클릭된 경우, 쪼개진 사각형 출력
			for (const auto& s_t : r.s_rt) {
				glColor3f(r.color[0], r.color[1], r.color[2]);
				glRectf(s_t.x, s_t.y, s_t.topRightX(), s_t.topRightY());
			}
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
	case '1':
		isKey = 1;
		cout << "key는 1로 설정되었습니다" << endl;
		break;
	case '2':
		isKey = 2;
		cout << "key는 2로 설정되었습니다" << endl;
		break;
	case '3':
		isKey = 3;
		cout << "key는 3로 설정되었습니다" << endl;
		break;
	case '4':
		isKey = 4;
		cout << "key는 4로 설정되었습니다" << endl;
		break;

	case 'r':

		rt.clear();
		rt.shrink_to_fit();
		for (int i = 0; i < g_rectNum; ++i) {
			rt.emplace_back();
		}
		cout << "사각형이 재설정되었습니다" << endl;

		break;
	}

	glutPostRedisplay();
}

GLvoid Mouse(int button, int state, int x, int y) {
	// gl 좌표로 변경
	float gl_x = x;
	float gl_y = y;
	window_to_gl(gl_x, gl_y);

	for (auto& r : rt) {
		// 사각형 안쪽을 클릭했을 경우
		if (r.x <= gl_x && gl_x <= r.topRightX() &&
			r.y <= gl_y && gl_y <= r.topRightY() && r.reftButtonSelect == false) {

			r.reftButtonSelect = true;
			cout << "선택되었다." << endl;

			// 크기 구하기
			float gl_w = r.width / 2;
			float gl_h = r.height / 2;
			float gl_w8 = r.width / 3;
			float gl_h8 = r.height / 3;

			// size 작은 크기로 변경
			switch (isKey) {

			case 1:
				
				// 작은 사각형 추가
				for (int i = 0; i < 4; ++i) {
					r.s_rt.emplace_back(r.x, r.y, gl_w, gl_h, isKey, i);
				}

				if (once[0] == false) {
					once[0] = true;
					glutTimerFunc(16, TimerFunction, 1);
				}

				break;

			case 2:

				// 작은 사각형 추가
				for (int i = 0; i < 4; ++i) {
					r.s_rt.emplace_back(r.x, r.y, gl_w, gl_h, isKey, i + 4);
				}

				if (once[0] == false) {
					once[0] = true;
					glutTimerFunc(16, TimerFunction, 1);
				}

				break;


			case 3:

				// 작은 사각형 추가
				for (int i = 0; i < 4; ++i) {
					r.s_rt.emplace_back(r.x, r.y, gl_w, gl_h, isKey, i);	// 모두 오른쪽으로 이동
				}

				if (once[0] == false) {
					once[0] = true;
					glutTimerFunc(16, TimerFunction, 1);
				}

				break;

			case 4:

				// 작은 사각형 추가
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

	glutPostRedisplay();  // 화면을 갱신하여 색상 변화를 반영
}

GLvoid Motion(int x, int y) {
	glutPostRedisplay();  // 좌표 갱신 후 다시 그리기
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

void TimerFunction(int value) {

	switch (value)
	{
	case 1:
		for (auto& r : rt) {
			if (r.reftButtonSelect) {
				for (auto& s : r.s_rt) {

					if (s.isSRexist) {	// 존재시 타이머 지속
						s.sizeDown();	// false시 타이머 끝
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