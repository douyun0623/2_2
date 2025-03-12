#pragma once

#include "library.h"
#include "MoveTriangle.h"

#include <string>
#include <vector>

class Scene
{
private:
	GLuint sceneShader;

	GLuint axisVAO, axisVBO;

	std::vector<MoveTriangle> triangles[4]; // �� ��и鿡 ���� �ﰢ��

	int width;
	int height;

	bool isSurface;

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

	void addTriangle(float x, float y, float r, float g, float b, float size, int quadrant);

	void setWindowSize(int winWidth, int winHeight);

private:
	void initBuffer();

	static GLuint makeShader(std::string vertexFilename, std::string fragmentFilename);
	static std::string readFile(std::string filename);

};

