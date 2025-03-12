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

	// shape 2개 생성
	for (int i = 0; i < 2; ++i) {
		createShape();
	}

	// basket 
	basket = new Basket(0.01f);
}

void Scene::update()
{
	// 바구니 업데이트
	basket->update();

	// 기본 도형삭제
	for (auto it = shapes.begin(); it != shapes.end();) {
		it->update();

		if (it->pos.y < -1.f) {
			it = shapes.erase(it);  // 삭제 후 iterator는 새로운 요소를 가리킴
			createShape();
		}
		else {
			++it;  // 삭제하지 않은 경우 iterator를 증가시켜 계속 진행
		}
	}

	// 슬라이스된 도형 
	for (auto& shape : shapes) {
		if (shape.isSliceShapeExist == false) {
			shape.isSliceShapeExist = true;
			createShape();
		}

		for (auto& sliceShape : shape.slicedShapes) {
			if(sliceShape.onBasket == false){
				if (basket->pos.x - basket->size.x < sliceShape.pos.x && sliceShape.pos.x < basket->pos.x + basket->size.x &&
					basket->pos.y - basket->size.y < sliceShape.pos.y && sliceShape.pos.y < basket->pos.y + basket->size.y) {

					// 바구니 위에 도형 올라감
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
	if (isPressed) {			// 눌러졌을 때
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
	else {						// 떼졌을 때
		switch (key) {
		default:
			break;
		}
	}
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

			// 모든 도형 슬라이스 시도
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
	std::uniform_int_distribution<int> randVertex(3, 6);	// 도형의 점의 개수
	std::uniform_real_distribution<float> randColor(0.f, 1.f);	// 색 설정

	int randVertexCnt = randVertex(gen);	// 3 - 4	(도형 결정/ 3 : 삼각형 , 4 : 사각형)

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
	// 쉐이더 만들자...

	GLint result;
	GLchar errorLog[512];

	GLuint vertexShader, fragmentShader;

	std::string vertexStr = readFile(vertexFilename);	// 이 변수는 지역변수라서
	const char* vertexSource{ vertexStr.c_str() };		// 이 변수에 저장되려면 위 변수가 소멸되면 안된다..!

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
		std::cout << vertexFilename << " 컴파일 성공!" << std::endl;
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
		std::cout << fragmentFilename << " 컴파일 성공!" << std::endl;
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
		std::cerr << vertexFilename << ", " << fragmentFilename << " ERROR: shader program 연결 실패\n" << errorLog << std::endl;
		//return;
	}
	else {
		std::cout << vertexFilename << ", " << fragmentFilename << " shader program 생성 성공!" << std::endl;
	}

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