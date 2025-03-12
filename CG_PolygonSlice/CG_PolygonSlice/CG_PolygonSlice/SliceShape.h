#pragma once

#include <iostream>
#include <vector>
#include <random>

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

// ����
class SliceShape {
public:
	std::vector<glm::vec3> posData;

	glm::vec2 pos;	// ���忡���� ��ġ
	glm::vec2 deltaPos;	// x,y ��ȭ��

	GLuint VAO;
	GLuint VBO_pos;

	bool onBasket = false;

	const float gravity = 9.8f;   // ���� �߷� ���ӵ� (m/s^2)
	const float dragCoefficient = 0.001f;  // ���� ���� ��� (�ӵ��� ����� ����)
	float deltaTime = 0.005f;   //0.016  // �ð� ���� (�� ����)

public:
	SliceShape(std::vector<glm::vec3> pos, glm::vec2 worldPos,float deltaPosX, float deltaPosY, float speed) 
		:posData(pos), pos(worldPos){

		for (auto& pd : posData) {
			std::cout << "�߸� ���� pos : " << pd.x << ' ' << pd.y << '\n';
		}

		// �ӵ� ����
		deltaTime = speed;

		// �޾ƿ���
		this->deltaPos = glm::vec2(deltaPosX, deltaPosY);

		// ���� ����
		{
			// VAO ����
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			//vbo
			glGenBuffers(1, &VBO_pos);

			// pos ������ VBO�� ���ε�
			glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
			glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec3), pos.data(), GL_STATIC_DRAW);

			// ������ �Ӽ� ���� (x, y, z ��ǥ)
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(0);

			// ����
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	}

	void update() {

		if (onBasket == false) {
			deltaPos.y -= gravity * deltaTime;		// �߷� ����
			deltaPos.x > 0 ? deltaPos.x -= dragCoefficient : deltaPos.x += dragCoefficient;	// ���� ���� ����, * deltaPos.x * deltaTime

			pos += deltaPos * deltaTime;
		}

	}

	void draw(GLuint shader) const {
		glUseProgram(shader);

		// �𵨸� ��ȯ
		glm::mat4 matrix(1.f);
		GLint worldLoc = glGetUniformLocation(shader, "modelTransform");
		if (worldLoc < 0)
			std::cout << "worldLoc ã�� ����\n";
		else
			glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(matrix));

		// �� ��ȯ
		glm::mat4 unitMat(1.f);
		glm::mat4 initPos = glm::translate(unitMat, glm::vec3(pos.x, pos.y, 0.f));
		glm::mat4 finalMat = initPos;
		glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(finalMat));

		int drawVertexNum = 0;
		// posData�� ���� ������ ����.
		for (const auto& pd : posData) {
			++drawVertexNum;
		}

		// �׸��� (�Ϲ� )
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, drawVertexNum);	
		glBindVertexArray(0);
	}
};