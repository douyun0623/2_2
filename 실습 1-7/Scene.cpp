#include "Scene.h"

#include <iostream>
#include <fstream>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_int_distribution<int> randPolygon(0, 9);
std::uniform_real_distribution<float> randColor(-1.0f, 1.0f);

Scene::Scene(int winWidth, int winHeight)
	: width{ winWidth }, height{ winHeight }
{
	selectPolygon = 1;
	cntPolygonNum = 0;
}

void Scene::initialize()
{
	pointShader = makeShader("point_vertex.glsl", "fragment.glsl");
	lineShader = makeShader("line_vertex.glsl", "fragment.glsl");
	triangleShader = makeShader("triangle_vertex.glsl", "fragment.glsl");
	rectangleShader = makeShader("rectangle_vertex.glsl", "fragment.glsl");
}

void Scene::update()
{
}

void Scene::draw()
{
	if(polygons.size()>0){
		for (const auto& p : polygons) {
			switch (p.shape)
			{
			case 1:
				glUseProgram(pointShader);
				glPointSize(5.f);	// ���� �׷��� ���� ũ�� ����
				uPosLoc = glGetUniformLocation(pointShader, "uPos");
				if (uPosLoc < 0)
					std::cerr << "point uPos ã�� ����" << std::endl;
				glUniform2f(uPosLoc, p.x, p.y);

				uColorLoc = glGetUniformLocation(pointShader, "uColor");
				if (uColorLoc < 0)
					std::cerr << "point uColor ã�� ����" << std::endl;
				glUniform3f(uColorLoc, p.r, p.g, p.b);

				glDrawArrays(GL_POINTS, 0, 1);
				break;
			case 2:
				glUseProgram(lineShader);
				uPosLoc = glGetUniformLocation(lineShader, "uPos");
				if (uPosLoc < 0)
					std::cerr << "point uPos ã�� ����" << std::endl;
				glUniform2f(uPosLoc, p.x, p.y);

				uColorLoc = glGetUniformLocation(pointShader, "uColor");
				if (uColorLoc < 0)
					std::cerr << "point uColor ã�� ����" << std::endl;
				glUniform3f(uColorLoc, p.r, p.g, p.b);

				glDrawArrays(GL_LINES, 0, 2);
				break;
			case 3:
				glUseProgram(triangleShader);

				uPosLoc = glGetUniformLocation(triangleShader, "uPos");
				if (uPosLoc < 0)
					std::cerr << "point uPos ã�� ����" << std::endl;
				glUniform2f(uPosLoc, p.x, p.y);

				uColorLoc = glGetUniformLocation(pointShader, "uColor");
				if (uColorLoc < 0)
					std::cerr << "point uColor ã�� ����" << std::endl;
				glUniform3f(uColorLoc, p.r, p.g, p.b);

				glDrawArrays(GL_TRIANGLES, 0, 3);
				break;
			case 4:
				glUseProgram(rectangleShader);

				uPosLoc = glGetUniformLocation(lineShader, "uPos");
				if (uPosLoc < 0)
					std::cerr << "point uPos ã�� ����" << std::endl;
				glUniform2f(uPosLoc, p.x, p.y);

				uColorLoc = glGetUniformLocation(pointShader, "uColor");
				if (uColorLoc < 0)
					std::cerr << "point uColor ã�� ����" << std::endl;
				glUniform3f(uColorLoc, p.r, p.g, p.b);

				glDrawArrays(GL_TRIANGLES, 0, 6);
				break;
			}
		}
	}
}

void Scene::keyboard(unsigned char key)
{
	//int selectRandPoly = randPolygon(gen) % cntPolygonNum;
	//std::cout << selectRandPoly << "���� ���õ�\n";

	switch (key) {
	case 27:	// ESC Key
		glutLeaveMainLoop();
		break;
	case 'p':
		std::cout << "��� : �� ����\n";
		selectPolygon = 1;
		break;
	case 'l':
		std::cout << "��� : �� ����\n";
		selectPolygon = 2;
		break;
	case 't':
		std::cout << "��� : �ﰢ�� ����\n";
		selectPolygon = 3;
		break;
	case 'r':
		std::cout << "��� : �簢�� ����\n";
		selectPolygon = 4;
		break;
	case 'c':
		std::cout << "��� ���� ����\n";
		cntPolygonNum = 0;
		polygons.clear();
		break;

	case 'w':
		polygons[randPolygon(gen) % cntPolygonNum].y += offsetSize;
		glutPostRedisplay();
		break;
	case 'a':
		polygons[randPolygon(gen) % cntPolygonNum].x -= offsetSize;
		glutPostRedisplay();
		break;
	case 's':
		polygons[randPolygon(gen) % cntPolygonNum].y -= offsetSize;
		glutPostRedisplay();
		break;
	case 'd':
		polygons[randPolygon(gen) % cntPolygonNum].x += offsetSize;
		std::cout << "������\n";
		glutPostRedisplay();
		break;
		
	default:
		break;
	}
}

void Scene::keyboardUp(unsigned char key)
{
}

void Scene::specialKeyboard(int key)
{
}

void Scene::specialKeyboardUp(int key)
{
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

			if (cntPolygonNum < 10) {	// ������ 10���� ��������

				float xPos = static_cast<float>(x) / width * 2.f - 1.f;
				float yPos = -(static_cast<float>(y) / height * 2.f - 1.f);

				float r = randColor(gen);
				float g = randColor(gen);
				float b = randColor(gen);

				polygons.push_back(Polygon(xPos, yPos, r, g, b, selectPolygon));

				std::cout << cntPolygonNum + 1<< "��° ����\n";
				++cntPolygonNum;
			}
			else {
				std::cout << "��� ������ �׷���\n";
			}

			break;
		}
			
		case GLUT_MIDDLE_BUTTON:
			break;
		case GLUT_RIGHT_BUTTON:
			break;
		case WHEEL_UP:
			break;
		case WHEEL_DOWN:
			break;
		}
		break;
	case GLUT_UP:
		switch (button) {
		case GLUT_LEFT_BUTTON:
			break;
		case GLUT_RIGHT_BUTTON:
			break;
		case GLUT_MIDDLE_BUTTON:
			break;
		}
		break;
	}
}

void Scene::setWindowSize(int winWidth, int winHeight)
{
	width = winWidth;
	height = winHeight;
}

GLuint Scene::makeShader(std::string vertexFilename, std::string fragmentFilename)
{
	// ���̴� ������...

	GLint result;
	GLchar errorLog[512];

	GLuint vertexShader, fragmentShader;


	// vertexStr
	std::string vertexStr = readFile(vertexFilename);	// �� ������ ����������
	const char* vertexSource{ vertexStr.c_str() };		// �� ������ ����Ƿ��� �� ������ �Ҹ�Ǹ� �ȵȴ�..!

	vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
		std::cerr << "ERROR: vertex shader error\n" << errorLog << std::endl;
		//return;
	}
	else {
		std::cout << "vertex shader ������ ����!" << std::endl;
	}


	// fragement
	std::string fragmentStr = readFile(fragmentFilename);
	const char* fragmentSource{ fragmentStr.c_str() };

	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
		std::cerr << "ERROR: fragment shader error\n" << errorLog << std::endl;
		//return;
	}
	else {
		std::cout << "fragment shader ������ ����!" << std::endl;
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
		std::cerr << "ERROR: shader program ���� ����\n" << errorLog << std::endl;
		//return;
	}
	else {
		std::cout << "shader program ���� ����!" << std::endl;
	}

	glUseProgram(shaderID);						// ���������� ������� ���̴� ���α׷��� �����Ѵ�!

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
