#pragma once

#include "library.h"

class MoveTriangle
{
	// 삼각형을 속성을 가지는 객체로 생각
private:
	GLuint VAO, VBO;

	float position[2];
	float size;
	float color[3];

	bool filled;

public:
	MoveTriangle(float x, float y, float r, float g, float b, float size);

	~MoveTriangle();

	//void update(int mode);
	void draw(GLuint shaderID) const;
};