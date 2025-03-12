#pragma once

#include "library.h"

class MoveTriangle
{
	// �ﰢ���� �Ӽ��� ������ ��ü�� ����
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