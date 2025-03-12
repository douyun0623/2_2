#pragma once

#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>
#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>

class Basket
{
public:
	glm::vec2 pos;	// basket의 중점 좌표
	glm::vec2 size;	// x : x size, y : y size 
	
	float dx;

	GLuint VAO, VBO;

	Basket(float dx);

	void update();

	void draw(GLuint shader) const;

};

