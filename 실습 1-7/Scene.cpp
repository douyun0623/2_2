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
				glPointSize(5.f);	// 이후 그려질 점의 크기 설정
				uPosLoc = glGetUniformLocation(pointShader, "uPos");
				if (uPosLoc < 0)
					std::cerr << "point uPos 찾지 못함" << std::endl;
				glUniform2f(uPosLoc, p.x, p.y);

				uColorLoc = glGetUniformLocation(pointShader, "uColor");
				if (uColorLoc < 0)
					std::cerr << "point uColor 찾지 못함" << std::endl;
				glUniform3f(uColorLoc, p.r, p.g, p.b);

				glDrawArrays(GL_POINTS, 0, 1);
				break;
			case 2:
				glUseProgram(lineShader);
				uPosLoc = glGetUniformLocation(lineShader, "uPos");
				if (uPosLoc < 0)
					std::cerr << "point uPos 찾지 못함" << std::endl;
				glUniform2f(uPosLoc, p.x, p.y);

				uColorLoc = glGetUniformLocation(pointShader, "uColor");
				if (uColorLoc < 0)
					std::cerr << "point uColor 찾지 못함" << std::endl;
				glUniform3f(uColorLoc, p.r, p.g, p.b);

				glDrawArrays(GL_LINES, 0, 2);
				break;
			case 3:
				glUseProgram(triangleShader);

				uPosLoc = glGetUniformLocation(triangleShader, "uPos");
				if (uPosLoc < 0)
					std::cerr << "point uPos 찾지 못함" << std::endl;
				glUniform2f(uPosLoc, p.x, p.y);

				uColorLoc = glGetUniformLocation(pointShader, "uColor");
				if (uColorLoc < 0)
					std::cerr << "point uColor 찾지 못함" << std::endl;
				glUniform3f(uColorLoc, p.r, p.g, p.b);

				glDrawArrays(GL_TRIANGLES, 0, 3);
				break;
			case 4:
				glUseProgram(rectangleShader);

				uPosLoc = glGetUniformLocation(lineShader, "uPos");
				if (uPosLoc < 0)
					std::cerr << "point uPos 찾지 못함" << std::endl;
				glUniform2f(uPosLoc, p.x, p.y);

				uColorLoc = glGetUniformLocation(pointShader, "uColor");
				if (uColorLoc < 0)
					std::cerr << "point uColor 찾지 못함" << std::endl;
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
	//std::cout << selectRandPoly << "도형 선택됨\n";

	switch (key) {
	case 27:	// ESC Key
		glutLeaveMainLoop();
		break;
	case 'p':
		std::cout << "모드 : 점 선택\n";
		selectPolygon = 1;
		break;
	case 'l':
		std::cout << "모드 : 선 선택\n";
		selectPolygon = 2;
		break;
	case 't':
		std::cout << "모드 : 삼각형 선택\n";
		selectPolygon = 3;
		break;
	case 'r':
		std::cout << "모드 : 사각형 선택\n";
		selectPolygon = 4;
		break;
	case 'c':
		std::cout << "모든 도형 삭제\n";
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
		std::cout << "움직임\n";
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
	// 화면 업데이트가 된다....

	constexpr int WHEEL_UP = 3;
	constexpr int WHEEL_DOWN = 4;

	switch (state) {
	case GLUT_DOWN:
		switch (button) {
		case GLUT_LEFT_BUTTON: {

			if (cntPolygonNum < 10) {	// 폴리곤 10개만 생성가능

				float xPos = static_cast<float>(x) / width * 2.f - 1.f;
				float yPos = -(static_cast<float>(y) / height * 2.f - 1.f);

				float r = randColor(gen);
				float g = randColor(gen);
				float b = randColor(gen);

				polygons.push_back(Polygon(xPos, yPos, r, g, b, selectPolygon));

				std::cout << cntPolygonNum + 1<< "번째 도형\n";
				++cntPolygonNum;
			}
			else {
				std::cout << "모든 도형이 그려짐\n";
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
	// 쉐이더 만들자...

	GLint result;
	GLchar errorLog[512];

	GLuint vertexShader, fragmentShader;


	// vertexStr
	std::string vertexStr = readFile(vertexFilename);	// 이 변수는 지역변수라서
	const char* vertexSource{ vertexStr.c_str() };		// 이 변수에 저장되려면 위 변수가 소멸되면 안된다..!

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
		std::cout << "vertex shader 컴파일 성공!" << std::endl;
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
		std::cout << "fragment shader 컴파일 성공!" << std::endl;
	}

	GLuint shaderID = glCreateProgram();
	glAttachShader(shaderID, vertexShader);		// 만든 vertex Shader를 쉐이더 프로그램에 추가해 준다
	glAttachShader(shaderID, fragmentShader);	// 만든 fragment Shader를 쉐이더 프로그램에 추가해 준다
	glLinkProgram(shaderID);					// 추가한 쉐이더들을 하나의 프로그램으로 생성한다!

	glDeleteShader(vertexShader);				// 이제 더이상 vertex Shaer는 필요없다...
	glDeleteShader(fragmentShader);				// 이제 더이상 fragment Shaer는 필요없다...

	glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
	if (!result) {
		glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
		std::cerr << "ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		//return;
	}
	else {
		std::cout << "shader program 생성 성공!" << std::endl;
	}

	glUseProgram(shaderID);						// 최종적으로 만들어진 쉐이더 프로그램을 적용한다!

	return shaderID;
}

std::string Scene::readFile(std::string filename)
{
	std::ifstream in{ filename };
	if (!in) {
		std::cout << filename << "파일 존재하지 않음!" << std::endl;
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
