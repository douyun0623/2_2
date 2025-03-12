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

// 도형
class SliceShape {
public:
	std::vector<glm::vec3> posData;

	glm::vec2 pos;	// 월드에서의 위치
	glm::vec2 deltaPos;	// x,y 변화량

	GLuint VAO;
	GLuint VBO_pos;

	bool onBasket = false;

	const float gravity = 9.8f;   // 실제 중력 가속도 (m/s^2)
	const float dragCoefficient = 0.001f;  // 공기 저항 계수 (속도에 비례한 저항)
	float deltaTime = 0.005f;   //0.016  // 시간 간격 (초 단위)

public:
	SliceShape(std::vector<glm::vec3> pos, glm::vec2 worldPos,float deltaPosX, float deltaPosY, float speed) 
		:posData(pos), pos(worldPos){

		for (auto& pd : posData) {
			std::cout << "잘린 도형 pos : " << pd.x << ' ' << pd.y << '\n';
		}

		// 속도 설정
		deltaTime = speed;

		// 받아오기
		this->deltaPos = glm::vec2(deltaPosX, deltaPosY);

		// 버퍼 설정
		{
			// VAO 설정
			glGenVertexArrays(1, &VAO);
			glBindVertexArray(VAO);

			//vbo
			glGenBuffers(1, &VBO_pos);

			// pos 데이터 VBO에 업로드
			glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
			glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(glm::vec3), pos.data(), GL_STATIC_DRAW);

			// 포지션 속성 설정 (x, y, z 좌표)
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
			glEnableVertexAttribArray(0);

			// 정리
			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	}

	void update() {

		if (onBasket == false) {
			deltaPos.y -= gravity * deltaTime;		// 중력 적용
			deltaPos.x > 0 ? deltaPos.x -= dragCoefficient : deltaPos.x += dragCoefficient;	// 공기 저항 적용, * deltaPos.x * deltaTime

			pos += deltaPos * deltaTime;
		}

	}

	void draw(GLuint shader) const {
		glUseProgram(shader);

		// 모델링 변환
		glm::mat4 matrix(1.f);
		GLint worldLoc = glGetUniformLocation(shader, "modelTransform");
		if (worldLoc < 0)
			std::cout << "worldLoc 찾지 못함\n";
		else
			glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(matrix));

		// 모델 변환
		glm::mat4 unitMat(1.f);
		glm::mat4 initPos = glm::translate(unitMat, glm::vec3(pos.x, pos.y, 0.f));
		glm::mat4 finalMat = initPos;
		glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(finalMat));

		int drawVertexNum = 0;
		// posData의 점의 개수를 센다.
		for (const auto& pd : posData) {
			++drawVertexNum;
		}

		// 그리기 (일반 )
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, drawVertexNum);	
		glBindVertexArray(0);
	}
};