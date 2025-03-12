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
	// ���̴�
	GLuint sceneShader;
	GLuint dragShader;
	GLuint basketShader;
	GLuint pathShader;

	// �� 
	std::vector<glm::vec3> vertexData;

	// �ٰ��� ��
	const glm::vec3 triPos[3] = {
		glm::vec3(-0.1f, -0.1f, 0.0f),
		glm::vec3(0.1f, -0.1f, 0.0f),
		glm::vec3(0.0f, 0.1f, 0.0f)
	};

	const glm::vec3 rectPos[6] = {
		glm::vec3(-0.1f, -0.1f, 0.0f),  // ù ��° �ﰢ��
		glm::vec3(0.1f, -0.1f, 0.0f),
		glm::vec3(0.1f, 0.1f, 0.0f),

		glm::vec3(0.1f, 0.1f, 0.0f),   // �� ��° �ﰢ��
		glm::vec3(-0.1f, 0.1f, 0.0f),
		glm::vec3(-0.1f, -0.1f, 0.0f)
	};

	const glm::vec3 pentagonPos[9] = {
		// ù ��° �ﰢ��
		glm::vec3(0.0f, 0.1f, 0.0f),    // �������� �߽� ������
		glm::vec3(-0.1f, 0.0f, 0.0f),
		glm::vec3(-0.06f, -0.1f, 0.0f),

		// �� ��° �ﰢ��
		glm::vec3(0.0f, 0.1f, 0.0f),    // �߽�
		glm::vec3(-0.06f, -0.1f, 0.0f),
		glm::vec3(0.06f, -0.1f, 0.0f),

		// �� ��° �ﰢ��
		glm::vec3(0.0f, 0.1f, 0.0f),    // �߽�
		glm::vec3(0.06f, -0.1f, 0.0f),
		glm::vec3(0.1f, 0.0f, 0.0f)
	};

	const glm::vec3 hexagonPos[12] = {
		// ù ��° �ﰢ��
		glm::vec3(-0.1f, 0.0f, 0.0f),   // ����
		glm::vec3(-0.05f, -0.0866f, 0.0f), // ���� �Ʒ�
		glm::vec3(0.05f, -0.0866f, 0.0f),  // ������ �Ʒ�

		// �� ��° �ﰢ��
		glm::vec3(-0.1f, 0.0f, 0.0f),   // ����
		glm::vec3(0.05f, -0.0866f, 0.0f),  // ������ �Ʒ�
		glm::vec3(0.1f, 0.0f, 0.0f),    // ������

		// �� ��° �ﰢ��
		glm::vec3(-0.1f, 0.0f, 0.0f),   // ����
		glm::vec3(0.1f, 0.0f, 0.0f),    // ������
		glm::vec3(0.05f, 0.0866f, 0.0f),  // ������ ��

		// �� ��° �ﰢ��
		glm::vec3(-0.1f, 0.0f, 0.0f),   // ����
		glm::vec3(0.05f, 0.0866f, 0.0f),  // ������ ��
		glm::vec3(-0.05f, 0.0866f, 0.0f)  // ���� ��
	};



	// ���콺
	glm::vec2 mouseStart;
	glm::vec2 mouseEnd;
	bool isDrag;

	// ������ â ũ��
	int width;
	int height;

	// ���󰡴� ���� ����Ʈ
	std::list <Shape> shapes;

	// �ٱ���
	Basket* basket;
	int onBasket = 0;

	// shape �Ӽ�
	bool drawPath = false;
	bool drawRot = false;
	float throwSpeed;	// ������ �ӵ�

public:
	// ���� ��������, update, draw..., Ű �Է�
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

