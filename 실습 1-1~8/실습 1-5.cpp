#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
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
	float color[3];
	bool exist = true;

	// 사각형 랜덤 위치 생성, 크기 지정, 랜덤 색
	DRect(float size)
		:DPoint(rand_position(mt), rand_position(mt)) {
		float gl_w;
		float gl_h;

		// gl 크기로 변환
		gl_size(size, gl_w, gl_h);

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

// 윈도우 좌표계를 OpenGL 좌표계로 변환
void window_to_gl(float& x, float& y);
float gl_size_h(float size);
float gl_size_w(float size);

// 사각형이 서로 겹치는지 확인
bool check_clash(DRect a, DRect b);

GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int, int, int, int);
GLvoid Motion(int x, int y);
GLvoid keyboard(unsigned char key, int x, int y);

vector <DRect> rt;

bool left_button = false;
bool timerActive = true;  // 타이머가 활성화 상태인지 추적


void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(50, 100); // 윈도우의 위치 지정
	glutInitWindowSize(800, 800); // 윈도우의 크기 지정
	glutCreateWindow("실습 1 - 5"); // 윈도우 생성(윈도우 이름)

	//--- GLEW 초기화하기
	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) // glew 초기화
	{
		std::cerr << "Unable to initialize GLEW" << std::endl;
		exit(EXIT_FAILURE);
	}
	else {
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
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // 바탕색 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	for (int i = 0; i < rt.size(); ++i) {
		if (rt[i].exist) {
			glColor3f(rt[i].color[0], rt[i].color[1], rt[i].color[2]);
			glRectf(rt[i].x, rt[i].y, rt[i].topRightX(), rt[i].topRightY());
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
	timerActive = true;

	switch (key) {
	case 'r':		// 모든 원소 삭제,  count = 0;
		rt.clear();
		rt.shrink_to_fit();
		for (int i = 0; i < 20; ++i) {
			rt.emplace_back(30);
		}

		// 지우개
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

			// 마지막 원소에 좌표 할당
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
			// rt가 비어있으면 좌표를 저장하지 않음
			std::cerr << "rt 벡터가 비어 있습니다." << std::endl;
		}

		left_button = true;

		glutPostRedisplay();  // 화면을 갱신하여 색상 변화를 반영
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		left_button = false;
		rt[rt.size() - 1].exist = false;

		glutPostRedisplay();  // 화면을 갱신하여 색상 변화를 반영
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

			// 마지막 원소에 좌표 할당
			rt[rt.size() - 1].x = gl_x - gl_w / 2;
			rt[rt.size() - 1].y = gl_y - gl_h / 2;

			// 만약 움직인 위치에 사각형이 있다면!
			for (int i = 0; i < rt.size() - 1; ++i) {
				if (check_clash(rt[i], rt[rt.size() - 1]) && rt[i].exist) {

					gl_size(10, gl_w, gl_h);

					// 지우개 크기 up
					rt[rt.size() - 1].width += gl_w;
					rt[rt.size() - 1].height += gl_h;

					// 지우개 색 먹은 색으로 변경
					rt[rt.size() - 1].color[0] = rt[i].color[0];
					rt[rt.size() - 1].color[1] = rt[i].color[1];
					rt[rt.size() - 1].color[2] = rt[i].color[2];

					// 먹힌 사각형 삭제
					rt[i].exist = false;

					glutPostRedisplay();
					break;
				}
			}

		}
		else {
			// rt가 비어있으면 좌표를 저장하지 않음
			std::cerr << "rt 벡터가 비어 있습니다." << std::endl;
		}

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

float gl_size_w(float size) {
	float windowWidth = glutGet(GLUT_WINDOW_WIDTH);

	return size / windowWidth * 2.0f;
}

float gl_size_h(float size) {
	float windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

	return size / windowHeight * 2.0f;
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