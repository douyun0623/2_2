#pragma once

#include <iostream>
#include <vector>
#include <random>
#include <list>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

#include "SliceShape.h"

// ����
class Shape {
public:
	int vertexCnt;	// ���� ����

	std::vector<glm::vec3> posData;		// ���� ��ġ
	glm::vec3 color;

	std::vector<glm::vec3> comVertexData;

	glm::vec2 pos;	// ���忡���� ��ġ
	glm::vec2 deltaPos;	// x,y ��ȭ��

	std::list <SliceShape> slicedShapes;

	std::vector<glm::vec2> PathVertex;

	GLuint VAO, VAO_path;
	GLuint VBO_pos, VBO_path;

	float angle = 0.f;
	float deltaAngle;

	const float gravity = 9.8f;   // ���� �߷� ���ӵ� (m/s^2)
	const float dragCoefficient = 0.001f;  // ���� ���� ��� (�ӵ��� ����� ����)
	float deltaTime = 0.005f;   //0.016  // �ð� ���� (�� ����)

	// �����̽� ó�� ���� Ȯ��
	bool isSliced = false;
	bool isSliceShapeExist = true;
	std::vector<int> intersectedEdges;

	const glm::vec3 rectPos[4] = {
		glm::vec3(-0.1f, -0.1f, 0.0f),
		glm::vec3(0.1f, -0.1f, 0.0f),
		glm::vec3(0.1f, 0.1f, 0.0f),
		glm::vec3(-0.1f, 0.1f, 0.0f)
	};

	const glm::vec3 triPos[3] = {
		glm::vec3(-0.1f, -0.1f, 0.0f),
		glm::vec3(0.1f, -0.1f, 0.0f),
		glm::vec3(0.0f, 0.1f, 0.0f)
	};

	const glm::vec3 pentagonPos[5] = {
	glm::vec3(-0.1f, 0.0f, 0.0f),     // ���� ��
	glm::vec3(-0.06f, -0.1f, 0.0f),   // ���� �Ʒ�
	glm::vec3(0.06f, -0.1f, 0.0f),    // ������ �Ʒ�
	glm::vec3(0.1f, 0.0f, 0.0f),      // ������ ��
	glm::vec3(0.0f, 0.1f, 0.0f)       // �� ��
	};

	const glm::vec3 hexagonPos[6] = {
	glm::vec3(-0.1f, 0.0f, 0.0f),   // ����
	glm::vec3(-0.05f, -0.0866f, 0.0f), // ���� �Ʒ�
	glm::vec3(0.05f, -0.0866f, 0.0f),  // ������ �Ʒ�
	glm::vec3(0.1f, 0.0f, 0.0f),    // ������
	glm::vec3(0.05f, 0.0866f, 0.0f),  // ������ ��
	glm::vec3(-0.05f, 0.0866f, 0.0f)  // ���� ��
	};


public:
	Shape(int vertexCnt, std::vector<glm::vec3> pos, glm::vec3 color, float speed);

	void setSpeed(float speed);

	void update();

	// ���� �׸���
	void draw(GLuint shader) const;

	void pathDraw(GLuint shader2) const;

	// 
	void initBuffer();
	void updatePathBuffer();

	// ���� ���� ��� �Լ�
	bool isIntersecting(glm::vec2 A1, glm::vec2 A2, glm::vec2 B1, glm::vec2 B2, glm::vec2& intersection);

	void slice(glm::vec2 start, glm::vec2 end);

	void createSlicedShapes(const std::vector<glm::vec2>& intersectionPoints, const glm::vec2& start, const glm::vec2& end);

};