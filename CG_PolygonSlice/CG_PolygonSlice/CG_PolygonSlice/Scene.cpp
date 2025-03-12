#include "Scene.h"

#include <iostream>
#include <fstream>
#include <sstream>

#include <vector>
#include <random>

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{
}

void Scene::initialize()
{
	sceneShader = makeShader("vertex.glsl", "fragment.glsl");
	dragShader = makeShader("dragVertex.glsl", "dragFrag.glsl");
	basketShader = makeShader("BasketVertex.glsl", "BasketFrag.glsl");
	pathShader = makeShader("pathVertex.glsl", "pathFrag.glsl");

	// shape 2�� ����
	for (int i = 0; i < 2; ++i) {
		createShape();
	}

	// basket 
	basket = new Basket(0.01f);
}

void Scene::update()
{
	// �ٱ��� ������Ʈ
	basket->update();

	// �⺻ ��������
	for (auto it = shapes.begin(); it != shapes.end();) {
		it->update();

		if (it->pos.y < -1.f) {
			it = shapes.erase(it);  // ���� �� iterator�� ���ο� ��Ҹ� ����Ŵ
			createShape();
		}
		else {
			++it;  // �������� ���� ��� iterator�� �������� ��� ����
		}
	}

	// �����̽��� ���� 
	for (auto& shape : shapes) {
		if (shape.isSliceShapeExist == false) {
			shape.isSliceShapeExist = true;
			createShape();
		}

		for (auto& sliceShape : shape.slicedShapes) {
			if(sliceShape.onBasket == false){
				if (basket->pos.x - basket->size.x < sliceShape.pos.x && sliceShape.pos.x < basket->pos.x + basket->size.x &&
					basket->pos.y - basket->size.y < sliceShape.pos.y && sliceShape.pos.y < basket->pos.y + basket->size.y) {

					// �ٱ��� ���� ���� �ö�
					sliceShape.onBasket = true;
					++onBasket;
					if (onBasket == 2) {
						createShape();
						onBasket = 0;
					}
				}
			}
			else {
				sliceShape.pos.x += basket->dx;
			}
		}
	}
}

void Scene::draw() const
{
	// shape
	for (const auto& shape : shapes) {

		// rotate
		GLint rotatePosLoc = glGetUniformLocation(sceneShader, "rotateTransform");
		if (rotatePosLoc < 0)
			std::cout << "ERROR invalid dragShader Loc\n";

		glm::mat4 rotateMat = glm::mat4(1.f);
		if(drawRot)
			rotateMat = glm::rotate(glm::mat4(1.f), glm::radians(shape.angle), glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(rotatePosLoc, 1, GL_FALSE, glm::value_ptr(rotateMat));

		shape.draw(sceneShader);
		if(shape.isSliced == false && drawPath){
			shape.pathDraw(pathShader);
		}
	}

	// drag
	if(isDrag){
		glUseProgram(dragShader);
		GLint startEndPosLoc = glGetUniformLocation(dragShader, "startEndPos");
		if (startEndPosLoc < 0)
			std::cout << "ERROR invalid dragShader Loc\n";

		glUniform4f(startEndPosLoc, mouseStart.x, mouseStart.y, mouseEnd.x, mouseEnd.y);

		glDrawArrays(GL_LINES, 0, 2);
	}

	// basket
	basket->draw(basketShader);
}

void Scene::keyboard(unsigned char key, bool isPressed)
{
	if (isPressed) {			// �������� ��
		switch (key) {
		case 'q':	// ESC Key
			glutLeaveMainLoop();
			break;
		case '+':
			throwSpeed += 0.002;
			for (auto& shape : shapes) {
				shape.setSpeed(throwSpeed);
			}
			break;
		case '-':
			if (throwSpeed > 0) {
				throwSpeed -= 0.002;

				for (auto& shape : shapes) {
					shape.setSpeed(throwSpeed);
				}
			}
			break;
		case 'f':
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			break;
		case 'l':
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			break;
		case 'p':
			drawPath = !drawPath;
			break;
		case 'r':
			drawRot = !drawRot;
			break;
		default:
			break;
		}
	}
	else {						// ������ ��
		switch (key) {
		default:
			break;
		}
	}
}

void Scene::mouse(int button, int state, int x, int y)
{
	// ȭ�� ������Ʈ�� �ȴ�....

	constexpr int WHEEL_UP = 3;
	constexpr int WHEEL_DOWN = 4;

	switch (state) {
	case GLUT_DOWN:

		switch (button) {
		case GLUT_LEFT_BUTTON: {
			float xPos = (static_cast<float>(x) / width * 2.f - 1.f);
			float yPos = -(static_cast<float>(y) / height * 2.f - 1.f);

			mouseStart = mouseEnd = glm::vec2(xPos, yPos);
			isDrag = true;
			break;
		}

		}
		break;
	case GLUT_UP:

		switch (button) {

		case GLUT_LEFT_BUTTON: {
			float xPos = (static_cast<float>(x) / width * 2.f - 1.f);
			float yPos = -(static_cast<float>(y) / height * 2.f - 1.f);

			mouseEnd = glm::vec2(xPos, yPos);

			// ��� ���� �����̽� �õ�
			for (auto& shape : shapes) {
				if(shape.isSliced == false){
					shape.slice(mouseStart, mouseEnd);
				}
			}

			isDrag = false;

			break;
		}

		}
		break;
	}
}

void Scene::mouseMove(int x, int y)
{
	if(isDrag){
		float xPos = (static_cast<float>(x) / width * 2.f - 1.f);
		float yPos = -(static_cast<float>(y) / height * 2.f - 1.f);

		mouseEnd = glm::vec2(xPos, yPos);
	}
}

void Scene::setWindowSize(int winWidth, int winHeight)
{
	width = winWidth;
	height = winHeight;
}

void Scene::createShape()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<int> randVertex(3, 6);	// ������ ���� ����
	std::uniform_real_distribution<float> randColor(0.f, 1.f);	// �� ����

	int randVertexCnt = randVertex(gen);	// 3 - 4	(���� ����/ 3 : �ﰢ�� , 4 : �簢��)

	vertexData.clear();

	switch (randVertexCnt)
	{
	case 3:
		for (const auto& tv : triPos) {
			vertexData.push_back(tv);
		}
		break;
	case 4:
		for (const auto& rp : rectPos) {
			vertexData.push_back(rp);
		}
		break;
	case 5:
		for (const auto& pp : pentagonPos) {
			vertexData.push_back(pp);
		}
		break;
	case 6:
		for (const auto& pp : hexagonPos) {
			vertexData.push_back(pp);
		}
		break;
	}

	shapes.emplace_back(randVertexCnt, vertexData, glm::vec3(randColor(gen), randColor(gen), randColor(gen)), throwSpeed);
}

GLuint Scene::makeShader(std::string vertexFilename, std::string fragmentFilename)
{
	// ���̴� ������...

	GLint result;
	GLchar errorLog[512];

	GLuint vertexShader, fragmentShader;

	std::string vertexStr = readFile(vertexFilename);	// �� ������ ����������
	const char* vertexSource{ vertexStr.c_str() };		// �� ������ ����Ƿ��� �� ������ �Ҹ�Ǹ� �ȵȴ�..!

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << vertexFilename << " ERROR: vertex shader error\n" << errorLog << std::endl;
	}
	else {
		std::cout << vertexFilename << " ������ ����!" << std::endl;
	}

	std::string fragmentStr = readFile(fragmentFilename);
	const char* fragmentSource{ fragmentStr.c_str() };

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << fragmentFilename << " ERROR: fragment shader error\n" << errorLog << std::endl;
	}
	else {
		std::cout << fragmentFilename << " ������ ����!" << std::endl;
	}

	GLuint shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);		// ���� vertex Shader�� ���̴� ���α׷��� �߰��� �ش�
	glAttachShader(shaderID, fragmentShader);	// ���� fragment Shader�� ���̴� ���α׷��� �߰��� �ش�
	glLinkProgram(shaderID);					// �߰��� ���̴����� �ϳ��� ���α׷����� �����Ѵ�!

	glDeleteShader(vertexShader);				// ���� ���̻� vertex Shaer�� �ʿ����...
	glDeleteShader(fragmentShader);				// ���� ���̻� fragment Shaer�� �ʿ����...

	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << vertexFilename << ", " << fragmentFilename << " ERROR: shader program ���� ����\n" << errorLog << std::endl;
		//return;
	}
	else {
		std::cout << vertexFilename << ", " << fragmentFilename << " shader program ���� ����!" << std::endl;
	}

	return shaderID;
}

std::string Scene::readFile(std::string filename)
{
	std::ifstream in{ filename };
	if (!in) {
		std::cout << filename << "���� �������� ����!" << std::endl;
		exit(1);
	}

	std::string str;
	std::string temp;
	while (std::getline(in, temp)) {
		str += temp;
		str.append(1, '\n');
	}

	return str;
}