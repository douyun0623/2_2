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

	// ȭ�� ���� �������� ���
	if (pos.x + size.x > 1.f  || pos.x - size.x < -1.f)
		dx *= -1.f;
}

void Basket::draw(GLuint shader) const
{
	glUseProgram(shader);

	// �� ��ȯ
	glm::mat4 unitMat(1.f);
	glm::mat4 initPos = glm::translate(unitMat, glm::vec3(pos.x, pos.y, 0.f));
	GLint posLoc = glGetUniformLocation(shader, "modelPos");
	if (posLoc < 0)
		std::cout << "posLoc ã�� ����\n";
	glUniformMatrix4fv(posLoc, 1, GL_FALSE, glm::value_ptr(initPos));

	// ������ ����
	GLint sizeLoc = glGetUniformLocation(shader, "modelSize");
	if (sizeLoc < 0)
		std::cout << "sizeLoc ã�� ����\n";
	glUniform2fv(sizeLoc, 1, glm::value_ptr(size));

	glBindVertexArray(VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}
