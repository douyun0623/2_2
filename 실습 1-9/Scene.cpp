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

    // �� �׸���
    glBindVertexArray(axisVAO);
    glDrawArrays(GL_LINES, 0, 4);
    glBindVertexArray(0);

    // ��� �ﰢ���� �׸���
    // ������ ä��ų� ������ �׸��� ����

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

    glUseProgram(0); // ���̴� ���α׷� ��Ȱ��ȭ
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
    // ó���� Ű�� �̺�Ʈ ����
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

        // �� ��и����� ����
        int quadrant = (openGLX > 0) ? (openGLY > 0 ? 0 : 3) : (openGLY > 0 ? 1 : 2);

        if (button == GLUT_LEFT_BUTTON) {
            std::cout << "��Ŭ�� ����\n";
            triangles[quadrant].clear();    // ���� �ִ� �ﰢ�� ����� �ϳ��� �����
            addTriangle(openGLX, openGLY, randColor(gen), randColor(gen), randColor(gen), randSize(gen), quadrant);
        }
        if (button == GLUT_RIGHT_BUTTON) {
            std::cout << "��Ŭ�� ����\n";
            addTriangle(openGLX, openGLY, randColor(gen), randColor(gen), randColor(gen), randSize(gen), quadrant);
        }
    }
}

void Scene::addTriangle(float x, float y, float r, float g, float b, float size, int quadrant)
{
    // � ��и��� �ﰢ���� ����
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
    // �� VAO �� VBO �ʱ�ȭ
    glGenVertexArrays(1, &axisVAO);
    glBindVertexArray(axisVAO);
    glGenBuffers(1, &axisVBO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);

    // �� ������ ����
    float axisVertices[] = {
        -1.0f, 0.0f,  // x�� ����
         1.0f, 0.0f,  // x�� ��
         0.0f, -1.0f, // y�� ����
         0.0f,  1.0f  // y�� ��
    };
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVertices), axisVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0); // VAO ���ε� ����
}

GLuint Scene::makeShader(std::string vertexFilename, std::string fragmentFilename)
{
    // ���̴� �����
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
        std::cout << vertexFilename << " ������ ����!" << std::endl;
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
        std::cout << fragmentFilename << " ������ ����!" << std::endl;
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
        std::cerr << vertexFilename << ", " << fragmentFilename << " ERROR: shader program ���� ����\n" << errorLog << std::endl;
    } else {
        std::cout << vertexFilename << ", " << fragmentFilename << " shader program ���� ����!" << std::endl;
    }

    return shaderID;
}

std::string Scene::readFile(std::string filename)
{
    std::ifstream in{ filename };
    if (!in) {
        std::cout << filename << " ���� �������� ����!" << std::endl;
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