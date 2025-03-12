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

// 도형
class Shape {
public:
	int vertexCnt;	// 점의 개수

	std::vector<glm::vec3> posData;		// 점의 위치
	glm::vec3 color;

	std::vector<glm::vec3> comVertexData;

	glm::vec2 pos;	// 월드에서의 위치
	glm::vec2 deltaPos;	// x,y 변화량

	std::list <SliceShape> slicedShapes;

	std::vector<glm::vec2> PathVertex;

	GLuint VAO, VAO_path;
	GLuint VBO_pos, VBO_path;

	float angle = 0.f;
	float deltaAngle;

	const float gravity = 9.8f;   // 실제 중력 가속도 (m/s^2)
	const float dragCoefficient = 0.001f;  // 공기 저항 계수 (속도에 비례한 저항)
	float deltaTime = 0.005f;   //0.016  // 시간 간격 (초 단위)

	// 슬라이스 처리 여부 확인
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
	glm::vec3(-0.1f, 0.0f, 0.0f),     // 왼쪽 위
	glm::vec3(-0.06f, -0.1f, 0.0f),   // 왼쪽 아래
	glm::vec3(0.06f, -0.1f, 0.0f),    // 오른쪽 아래
	glm::vec3(0.1f, 0.0f, 0.0f),      // 오른쪽 위
	glm::vec3(0.0f, 0.1f, 0.0f)       // 맨 위
	};

	const glm::vec3 hexagonPos[6] = {
	glm::vec3(-0.1f, 0.0f, 0.0f),   // 왼쪽
	glm::vec3(-0.05f, -0.0866f, 0.0f), // 왼쪽 아래
	glm::vec3(0.05f, -0.0866f, 0.0f),  // 오른쪽 아래
	glm::vec3(0.1f, 0.0f, 0.0f),    // 오른쪽
	glm::vec3(0.05f, 0.0866f, 0.0f),  // 오른쪽 위
	glm::vec3(-0.05f, 0.0866f, 0.0f)  // 왼쪽 위
	};


public:
	Shape(int vertexCnt, std::vector<glm::vec3> pos, glm::vec3 color, float speed);

	void setSpeed(float speed);

	void update();

	// 도형 그리기
	void draw(GLuint shader) const;

	void pathDraw(GLuint shader2) const;

	// 
	void initBuffer();
	void updatePathBuffer();

	// 선분 교차 계산 함수
	bool isIntersecting(glm::vec2 A1, glm::vec2 A2, glm::vec2 B1, glm::vec2 B2, glm::vec2& intersection);

	void slice(glm::vec2 start, glm::vec2 end);

	void createSlicedShapes(const std::vector<glm::vec2>& intersectionPoints, const glm::vec2& start, const glm::vec2& end);

};