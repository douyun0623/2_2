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


	// 1. ����ü ����
	struct Polygon {
		float x; // X ��ǥ
		float y; // Y ��ǥ
		float r; // Red ���� ��
		float g; // Green ���� ��
		float b; // Blue ���� ��
		int shape;	// 1 : ��, 2 : ��, 3 : �ﰢ��, 4: �簢��

		// ������ (���ϰ� �ʱ�ȭ�� �� �ְ�)
		Polygon(float x_, float y_, float r_, float g_, float b_, int shape_)
			: x(x_), y(y_), r(r_), g(g_), b(b_), shape(shape_) {}
	};

	std::vector<Polygon> polygons;
	GLint uPosLoc;
	GLint uColorLoc;

	int selectPolygon;	// 1 : ��, 2 : ��, 3 : �ﰢ��, 4 : �簢��

	int cntPolygonNum;

	float offsetSize = 0.1f;

public:
	// ���� ��������, update, draw..., Ű �Է�
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

