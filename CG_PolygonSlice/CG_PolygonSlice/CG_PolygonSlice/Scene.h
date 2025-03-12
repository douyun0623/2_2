#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <list>	

#include "Shape.h"
#include "Basket.h"

class Scene
{
private:
	// 쉐이더
	GLuint sceneShader;
	GLuint dragShader;
	GLuint basketShader;
	GLuint pathShader;

	// 점 
	std::vector<glm::vec3> vertexData;

	// 다각형 점
	const glm::vec3 triPos[3] = {
		glm::vec3(-0.1f, -0.1f, 0.0f),
		glm::vec3(0.1f, -0.1f, 0.0f),
		glm::vec3(0.0f, 0.1f, 0.0f)
	};

	const glm::vec3 rectPos[6] = {
		glm::vec3(-0.1f, -0.1f, 0.0f),  // 첫 번째 삼각형
		glm::vec3(0.1f, -0.1f, 0.0f),
		glm::vec3(0.1f, 0.1f, 0.0f),

		glm::vec3(0.1f, 0.1f, 0.0f),   // 두 번째 삼각형
		glm::vec3(-0.1f, 0.1f, 0.0f),
		glm::vec3(-0.1f, -0.1f, 0.0f)
	};

	const glm::vec3 pentagonPos[9] = {
		// 첫 번째 삼각형
		glm::vec3(0.0f, 0.1f, 0.0f),    // 오각형의 중심 꼭짓점
		glm::vec3(-0.1f, 0.0f, 0.0f),
		glm::vec3(-0.06f, -0.1f, 0.0f),

		// 두 번째 삼각형
		glm::vec3(0.0f, 0.1f, 0.0f),    // 중심
		glm::vec3(-0.06f, -0.1f, 0.0f),
		glm::vec3(0.06f, -0.1f, 0.0f),

		// 세 번째 삼각형
		glm::vec3(0.0f, 0.1f, 0.0f),    // 중심
		glm::vec3(0.06f, -0.1f, 0.0f),
		glm::vec3(0.1f, 0.0f, 0.0f)
	};

	const glm::vec3 hexagonPos[12] = {
		// 첫 번째 삼각형
		glm::vec3(-0.1f, 0.0f, 0.0f),   // 왼쪽
		glm::vec3(-0.05f, -0.0866f, 0.0f), // 왼쪽 아래
		glm::vec3(0.05f, -0.0866f, 0.0f),  // 오른쪽 아래

		// 두 번째 삼각형
		glm::vec3(-0.1f, 0.0f, 0.0f),   // 왼쪽
		glm::vec3(0.05f, -0.0866f, 0.0f),  // 오른쪽 아래
		glm::vec3(0.1f, 0.0f, 0.0f),    // 오른쪽

		// 세 번째 삼각형
		glm::vec3(-0.1f, 0.0f, 0.0f),   // 왼쪽
		glm::vec3(0.1f, 0.0f, 0.0f),    // 오른쪽
		glm::vec3(0.05f, 0.0866f, 0.0f),  // 오른쪽 위

		// 네 번째 삼각형
		glm::vec3(-0.1f, 0.0f, 0.0f),   // 왼쪽
		glm::vec3(0.05f, 0.0866f, 0.0f),  // 오른쪽 위
		glm::vec3(-0.05f, 0.0866f, 0.0f)  // 왼쪽 위
	};



	// 마우스
	glm::vec2 mouseStart;
	glm::vec2 mouseEnd;
	bool isDrag;

	// 윈도우 창 크기
	int width;
	int height;

	// 날라가는 도형 리스트
	std::list <Shape> shapes;

	// 바구니
	Basket* basket;
	int onBasket = 0;

	// shape 속성
	bool drawPath = false;
	bool drawRot = false;
	float throwSpeed;	// 던지는 속도

public:
	// 게임 로직에서, update, draw..., 키 입력
	Scene(int winWidth, int winHeight);

	void initialize();

	void update();

	void draw() const;

	void keyboard(unsigned char key, bool isPressed);
	void mouse(int button, int state, int x, int y);
	void mouseMove(int x, int y);

	void setWindowSize(int winWidth, int winHeight);

	void createShape();
public:
	static GLuint makeShader(std::string vertexFilename, std::string fragmentFilename);
	static std::string readFile(std::string filename);
};

