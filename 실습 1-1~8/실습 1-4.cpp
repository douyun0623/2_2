#include <iostream>
#include <gl/glew.h> //--- 필요한 헤더파일 include
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <random>
#include <vector>
#include <cmath>

using namespace std;

#define CHANGE_COLOR 1
#define CHANGE_SIZE 2
#define MOVE_DIAGONAL 3
#define MOVE_ZIGZAG 4

std::random_device rd;
std::mt19937 mt(rd());
std::uniform_real_distribution<float> rand_color(0.0f, 1.0f);	// rand_color(mt)
std::uniform_real_distribution<float> rand_position(-0.8f, 0.8f);	// rand_color(mt)
std::uniform_real_distribution<float> rand_size(-4.5f, 4.5f);	// rand_size(mt)
std::uniform_int_distribution<int> rand_distance(0, 3);	// 방향

// 윈도우 좌표계를 OpenGL 좌표계로 변환
void window_to_gl(float& x, float& y);
void gl_size(float size, float& w, float& h);
float gl_size_h(float size);
float gl_size_w(float size);

float dis[4][2] = { {0.01f, 0.01f}, {-0.01f, 0.01f},{0.01f, -0.01f},{-0.01f, -0.01f} };

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
	float draw_x, draw_y;
	float draw_w, draw_h;
	float color[3];
	int count_z = 0;

	// 방향, 속도
	float dx, dy;

	// 생성자 : 중앙점, 정사각형, 처음그린 위치 저장
	DRect(float d_x, float d_y, float size)
	{
		float gl_w = 0;
		float gl_h = 0;

		gl_size(size, gl_w, gl_h);

		float gl_x = d_x - (gl_w / 2);
		float gl_y = d_y - (gl_h / 2);

		this->x = gl_x;
		this->y = gl_y;
		this->width = gl_w;
		this->height = gl_h;
		this->draw_x = gl_x;
		this->draw_y = gl_y;
		this->draw_w = gl_w;
		this->draw_h = gl_h;

		for (auto& c : color)
			c = rand_color(mt);

		int rand_dis = rand_distance(mt);

		this->dx = dis[rand_dis][0];
		this->dy = dis[rand_dis][1];
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

		if (x <= -1.0f || topRightX() >= 1.0f) {
			dx *= -1.0f;
		}
		if (y <= -1.0f || topRightY() >= 1.0f) {
			dy *= -1.0f;
		}
	}

	void move_z() {
		++count_z;
		x += dx;
		y += dy;

		if (count_z > 7) {
			count_z = 0;
			dy *= -1.0f;
		}
		if (x <= -1.0f || topRightX() >= 1.0f) {
			dx *= -1.0f;
		}
		if (y <= -1.0f || topRightY() >= 1.0f) {
			dy *= -1.0f;
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

vector <DRect> rt;
int count_a = 0;
bool timerActive = true;  // 타이머가 활성화 상태인지 추적

void main(int argc, char** argv) //--- 윈도우 출력하고 콜백함수 설정 
{ //--- 윈도우 생성하기
	glutInit(&argc, argv); // glut 초기화
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
	glutInitWindowPosition(50, 100); // 윈도우의 위치 지정
	glutInitWindowSize(800, 800); // 윈도우의 크기 지정
	glutCreateWindow("실습 "); // 윈도우 생성(윈도우 이름)

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
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // 바탕색 지정
	glClear(GL_COLOR_BUFFER_BIT); // 설정된 색으로 전체를 칠하기

	for (const auto& r : rt) {
		glColor3f(r.color[0], r.color[1], r.color[2]);
		glRectf(r.x, r.y, r.topRightX(), r.topRightY());
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
		
	case '1':	// 대각 행동
		glutTimerFunc(30, TimerFunction, MOVE_DIAGONAL);
		break;
	case '2':	//Zigzag
		glutTimerFunc(30, TimerFunction, MOVE_ZIGZAG);
		break;
	case '3':
		glutTimerFunc(100, TimerFunction, CHANGE_SIZE);
		break;
	case '4':	// 색 랜덤
		glutTimerFunc(100, TimerFunction, CHANGE_COLOR);
		break;
	case 's':
		timerActive = false;
		break;
	case 'm':
		timerActive = false;
		for (auto& r : rt) {
			r.x = r.draw_x;
			r.y = r.draw_y;
			r.width = r.draw_w;
			r.height = r.draw_h;
		}
		break;
	case 'r':		// 모든 원소 삭제,  count = 0;
		count_a = 0;
		timerActive = false;
		rt.clear();
		rt.shrink_to_fit();
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	}

	glutPostRedisplay();
}

GLvoid TimerFunction(int value)
{
	if(timerActive){
		if (value == CHANGE_COLOR) {
			for (auto& r : rt) {
				for (auto& c : r.color) {
					c = rand_color(mt);
				}
			}

			glutPostRedisplay();
			glutTimerFunc(100, TimerFunction, CHANGE_COLOR);
		}

		if (value == CHANGE_SIZE) {
			for (auto& r : rt) {
				float size_w = gl_size_w(10.0f);
				float size_h = gl_size_h(10.0f);

				r.width = max(size_w, r.width + gl_size_w(rand_size(mt)));
				r.height = max(size_h, r.height + gl_size_h(rand_size(mt)));
			}

			glutPostRedisplay();
			glutTimerFunc(100, TimerFunction, CHANGE_SIZE);
		}

		if (value == MOVE_DIAGONAL) {
			for (auto& r : rt) {
				r.move();
			}

			glutPostRedisplay();
			glutTimerFunc(30, TimerFunction, MOVE_DIAGONAL);
		}

		if (value == MOVE_ZIGZAG) {
			for (auto& r : rt) {
				r.move_z();
			}

			glutPostRedisplay();
			glutTimerFunc(30, TimerFunction, MOVE_ZIGZAG);
		}
	}
}

GLvoid Mouse(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		if (count_a < 5) {
			// gl 좌표로 변경
			float gl_x = x;
			float gl_y = y;
			window_to_gl(gl_x, gl_y);
			rt.emplace_back(gl_x, gl_y, 50.0f);

			++count_a;
		}

		glutPostRedisplay();  // 화면을 갱신하여 색상 변화를 반영
	}
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