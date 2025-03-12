#include "Scene.h"

#include <iostream>
#include <fstream>
#include <memory>
#include <random>

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<float> randColor(-0.9f, 0.9f);

std::uniform_real_distribution<float> randSize(0.05f, 0.1f);

Scene::Scene(int winWidth, int winHeight)
    : width{ winWidth }, height{ winHeight }
{
    isSurface = true;
}

void Scene::initialize()
{
    sceneShader = makeShader("vertex.glsl", "fragment.glsl");
    initBuffer();
}

void Scene::update()
{
}

void Scene::draw() {
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(sceneShader);

    // 축 그리기
    glBindVertexArray(axisVAO);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);

    // 모든 삼각형을 그리기
    // 도형을 채우거나 선으로 그릴지 설정

    if (isSurface) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    else {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    for (int i = 0; i < 4; ++i) {
        for (const auto& triangle : triangles[i]) {
            triangle.draw(sceneShader);
        }
    }

    glUseProgram(0); // 쉐이더 프로그램 비활성화
}


void Scene::keyboard(unsigned char key)
{
    switch (key) {
    case 27:    // ESC Key
        glutLeaveMainLoop();
        break;
    case 'a':
        isSurface = true;
        break;
    case 'b':
        isSurface = false;
        break;
    }
}

void Scene::keyboardUp(unsigned char key)
{
    // 처리할 키업 이벤트 없음
}

void Scene::specialKeyboard(int key)
{
}

void Scene::specialKeyboardUp(int key)
{
}

void Scene::mouse(int button, int state, int x, int y)
{
    if (state == GLUT_DOWN) {
        float openGLX = (static_cast<float>(x) / width) * 2.0f - 1.0f;
        float openGLY = (static_cast<float>(height - y) / height) * 2.0f - 1.0f;

        // 몇 사분면인지 결정
        int quadrant = (openGLX > 0) ? (openGLY > 0 ? 0 : 3) : (openGLY > 0 ? 1 : 2);

        if (button == GLUT_LEFT_BUTTON) {
            std::cout << "좌클릭 눌림\n";
            triangles[quadrant].clear();    // 전에 있던 삼각형 지우고 하나만 남기기
            addTriangle(openGLX, openGLY, randColor(gen), randColor(gen), randColor(gen), randSize(gen), quadrant);
        }
        if (button == GLUT_RIGHT_BUTTON) {
            std::cout << "우클릭 눌림\n";
            addTriangle(openGLX, openGLY, randColor(gen), randColor(gen), randColor(gen), randSize(gen), quadrant);
        }
    }
}

void Scene::addTriangle(float x, float y, float r, float g, float b, float size, int quadrant)
{
    // 어떤 사분면의 삼각형의 개수
    if (triangles[quadrant].size() < 3) {
        triangles[quadrant].emplace_back(x, y, r, g, b, size);
    }
}

void Scene::setWindowSize(int winWidth, int winHeight)
{
    width = winWidth;
    height = winHeight;
}

void Scene::initBuffer()
{
    // 축 VAO 및 VBO 초기화
    glGenVertexArrays(1, &axisVAO);
    glBindVertexArray(axisVAO);
    glGenBuffers(1, &axisVBO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);

    // 축 데이터 설정
    float axisVertices[] = {
        -1.0f, 0.0f,  // x축 시작
         1.0f, 0.0f,  // x축 끝
         0.0f, -1.0f, // y축 시작
         0.0f,  1.0f  // y축 끝
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // VAO 바인딩 해제
}

GLuint Scene::makeShader(std::string vertexFilename, std::string fragmentFilename)
{
    // 쉐이더 만들기
    GLint result;
    GLchar errorLog[512];
    GLuint vertexShader, fragmentShader;

    std::string vertexStr = readFile(vertexFilename);
    const char* vertexSource{ vertexStr.c_str() };

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(vertexShader, 512, NULL, errorLog);
        std::cerr << vertexFilename << " ERROR: vertex shader error\n" << errorLog << std::endl;
    } else {
        std::cout << vertexFilename << " 컴파일 성공!" << std::endl;
    }

    std::string fragmentStr = readFile(fragmentFilename);
    const char* fragmentSource{ fragmentStr.c_str() };

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &result);
    if (!result) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, errorLog);
        std::cerr << fragmentFilename << " ERROR: fragment shader error\n" << errorLog << std::endl;
    } else {
        std::cout << fragmentFilename << " 컴파일 성공!" << std::endl;
    }

    GLuint shaderID = glCreateProgram();
    glAttachShader(shaderID, vertexShader);
    glAttachShader(shaderID, fragmentShader);
    glLinkProgram(shaderID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glGetProgramiv(shaderID, GL_LINK_STATUS, &result);
    if (!result) {
        glGetProgramInfoLog(shaderID, 512, NULL, errorLog);
        std::cerr << vertexFilename << ", " << fragmentFilename << " ERROR: shader program 연결 실패\n" << errorLog << std::endl;
    } else {
        std::cout << vertexFilename << ", " << fragmentFilename << " shader program 생성 성공!" << std::endl;
    }

    return shaderID;
}

std::string Scene::readFile(std::string filename)
{
    std::ifstream in{ filename };
    if (!in) {
        std::cout << filename << " 파일 존재하지 않음!" << std::endl;
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