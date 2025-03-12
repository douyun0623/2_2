#pragma once

#include "library.h"

#include <string>

#include <list>
#include <vector>


class Scene
{
private:
	GLuint pointShader;
	GLuint lineShader;
	GLuint triangleShader;
	GLuint rectangleShader;

	int width;
	int height;


	// 1. 구조체 정의
	struct Polygon {
		float x; // X 좌표
		float y; // Y 좌표
		float r; // Red 색상 값
		float g; // Green 색상 값
		float b; // Blue 색상 값
		int shape;	// 1 : 점, 2 : 선, 3 : 삼각형, 4: 사각형

		// 생성자 (편리하게 초기화할 수 있게)
		Polygon(float x_, float y_, float r_, float g_, float b_, int shape_)
			: x(x_), y(y_), r(r_), g(g_), b(b_), shape(shape_) {}
	};

	std::vector<Polygon> polygons;
	GLint uPosLoc;
	GLint uColorLoc;

	int selectPolygon;	// 1 : 점, 2 : 선, 3 : 삼각형, 4 : 사각형

	int cntPolygonNum;

	float offsetSize = 0.1f;

public:
	// 게임 로직에서, update, draw..., 키 입력
	Scene(int winWidth, int winHeight);

	void initialize();

	void update();

	void draw();

	void keyboard(unsigned char key);		// press
	void keyboardUp(unsigned char key);		// release
	void specialKeyboard(int key);			// press
	void specialKeyboardUp(int key);		// release
	void mouse(int button, int state, int x, int y);

	void setWindowSize(int winWidth, int winHeight);

private:
	GLuint makeShader(std::string vertexFilename, std::string fragmentFilename);
	std::string readFile(std::string filename);

};

