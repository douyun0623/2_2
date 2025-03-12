#include "Basket.h"

#include <iostream>

Basket::Basket(float dx)
	: dx(dx)
{
	pos = glm::vec2(0.f, -0.7f);
	size = glm::vec2(0.2f, 0.025f);
}

void Basket::update()
{
	pos.x += dx;

	// 화면 끝에 도달했을 경우
	if (pos.x + size.x > 1.f  || pos.x - size.x < -1.f)
		dx *= -1.f;
}

void Basket::draw(GLuint shader) const
{
	glUseProgram(shader);

	// 모델 변환
	glm::mat4 unitMat(1.f);
	glm::mat4 initPos = glm::translate(unitMat, glm::vec3(pos.x, pos.y, 0.f));
	GLint posLoc = glGetUniformLocation(shader, "modelPos");
	if (posLoc < 0)
		std::cout << "posLoc 찾지 못함\n";
	glUniformMatrix4fv(posLoc, 1, GL_FALSE, glm::value_ptr(initPos));

	// 사이즈 전달
	GLint sizeLoc = glGetUniformLocation(shader, "modelSize");
	if (sizeLoc < 0)
		std::cout << "sizeLoc 찾지 못함\n";
	glUniform2fv(sizeLoc, 1, glm::value_ptr(size));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
