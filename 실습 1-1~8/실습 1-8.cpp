#include <iostream>
#include <gl/glew.h>
#include <gl/freeglut.h>
#include <gl/freeglut_ext.h>

#include <gl/glm/glm.hpp>
#include <gl/glm/ext.hpp>
#include <gl/glm/gtc/matrix_transform.hpp>


#include <vector>
#include <random>
#include <fstream>
#include <sstream>

using namespace std;

// �ﰢ�� ������ ����ü
struct Triangle {
    float x, y;      // �߽� ��ġ
    float size;      // ũ��
    float color[3];  // RGB ����

    Triangle(float x, float y, float size) : x(x), y(y), size(size) {
        // ���� ���� ����
        color[0] = static_cast<float>(rand()) / RAND_MAX; // R
        color[1] = static_cast<float>(rand()) / RAND_MAX; // G
        color[2] = static_cast<float>(rand()) / RAND_MAX; // B
    }
};

vector<Triangle> quadrants[4]; // �� ��и��� �ﰢ�� ����
bool filled = true;              // �� �Ǵ� ������ �׸��� ��ȯ

GLuint vao, shaderProgram;

// �Լ� ������Ÿ��
GLvoid drawScene(GLvoid);
GLvoid Reshape(int w, int h);
GLvoid Mouse(int button, int state, int x, int y);
GLvoid keyboard(unsigned char key, int x, int y);
void drawTriangle(const Triangle& triangle);
void drawQuadrantTriangles(int quadrant);
void window_to_gl(float& x, float& y);
string readShaderSource(const char* filePath);
GLuint createShader(const char* filePath, GLenum shaderType);
void createShaderProgram();

int main(int argc, char** argv) {
    glutInit(&argc, argv); // GLUT �ʱ�ȭ
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // ���÷��� ��� ����
    glutInitWindowPosition(50, 100); // ������ ��ġ
    glutInitWindowSize(800, 600); // ������ ũ��
    glutCreateWindow("Triangle Drawing with Shaders"); // ������ ����

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "Unable to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }
    else {
        cout << "GLEW Initialized\n";
    }

    createShaderProgram(); // ���̴� ���α׷� ����

    glutDisplayFunc(drawScene); // ��� �Լ� ����
    glutReshapeFunc(Reshape); // ũ�� ���� �� �ٽ� �׸��� �Լ� ����
    glutMouseFunc(Mouse); // ���콺 Ŭ�� ó�� �Լ� ����
    glutKeyboardFunc(keyboard); // Ű���� �Է� ó�� �Լ� ����
    glutMainLoop(); // �̺�Ʈ ó�� ����
}

GLvoid drawScene() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // ���� ����
    glClear(GL_COLOR_BUFFER_BIT); // ������ ȭ�� �����

    glUseProgram(shaderProgram); // ���̴� ���α׷� ���

    // �� ��и��� �ﰢ�� �׸���
    for (int i = 0; i < 4; ++i) {
        drawQuadrantTriangles(i);
    }

    glutSwapBuffers(); // ȭ�鿡 ���
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h); // ����Ʈ ũ�� ����
}

GLvoid Mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        // OpenGL ��ǥ�� ��ȯ
        float gl_x = x;
        float gl_y = y;
        window_to_gl(gl_x, gl_y);

        int quadrant = -1;
        // ��и� �Ǵ�
        if (gl_x >= 0 && gl_y >= 0) quadrant = 0; // ��1��и�
        else if (gl_x < 0 && gl_y >= 0) quadrant = 1; // ��2��и�
        else if (gl_x < 0 && gl_y < 0) quadrant = 2; // ��3��и�
        else if (gl_x >= 0 && gl_y < 0) quadrant = 3; // ��4��и�

        if (button == GLUT_LEFT_BUTTON && quadrant != -1) {
            // ���� ���콺 Ŭ��: ���� �ﰢ�� ���� �� ���ο� �ﰢ�� �߰�
            if (!quadrants[quadrant].empty()) {
                quadrants[quadrant].pop_back(); // ���� �ﰢ�� ����
            }
            quadrants[quadrant].emplace_back(gl_x, gl_y, static_cast<float>(rand()) / RAND_MAX * 0.1f); // ���ο� �ﰢ�� �߰�
        }
        else if (button == GLUT_RIGHT_BUTTON && quadrant != -1) {
            // ������ ���콺 Ŭ��: ���ο� �ﰢ�� �߰� (�ִ� 3��)
            if (quadrants[quadrant].size() < 3) {
                quadrants[quadrant].emplace_back(gl_x, gl_y, static_cast<float>(rand()) / RAND_MAX * 0.1f);
            }
        }

        glutPostRedisplay(); // ȭ�� ����
    }
}

GLvoid keyboard(unsigned char key, int x, int y) {
    if (key == 'a') {
        filled = true; // ������ �׸���
    }
    else if (key == 'b') {
        filled = false; // ������ �׸���
    }
    glutPostRedisplay(); // ȭ�� ����
}

void drawTriangle(const Triangle& triangle) {
    // ������ ���� ������ �迭
    float vertices[18] = {
        triangle.x, triangle.y + triangle.size, // �� ������
        triangle.color[0], triangle.color[1], triangle.color[2], // �� ������ ����

        triangle.x - triangle.size, triangle.y - triangle.size, // ���� ������
        triangle.color[0], triangle.color[1], triangle.color[2], // ���� ������ ����

        triangle.x + triangle.size, triangle.y - triangle.size, // ������ ������
        triangle.color[0], triangle.color[1], triangle.color[2]  // ������ ������ ����
    };

    glBindVertexArray(vao); // VAO ���ε�

    // VBO ���� �� ������ ����
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // ��ġ �Ӽ� ����
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);

    // ���� �Ӽ� ����
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    // �ﰢ�� �׸���
    if (filled) {
        glDrawArrays(GL_TRIANGLES, 0, 3); // ������ �׸���
    }
    else {
        glDrawArrays(GL_LINE_LOOP, 0, 3); // ������ �׸���
    }

    glDeleteBuffers(1, &vbo); // VBO ����
}

void drawQuadrantTriangles(int quadrant) {
    for (const auto& triangle : quadrants[quadrant]) {
        drawTriangle(triangle);
    }
}

// ������ ��ǥ�踦 OpenGL ��ǥ��� ��ȯ
void window_to_gl(float& x, float& y) {
    float windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    float windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    // X ��ǥ ��ȯ (OpenGL ��ǥ�迡�� ������ -1, �������� 1)
    x = (float)x / (windowWidth / 2) - 1.0f;

    // Y ��ǥ ��ȯ (OpenGL ��ǥ�迡�� �Ʒ��� -1, ���� 1)
    y = 1.0f - (float)y / (windowHeight / 2);
}

// ���̴� �ҽ� �ڵ� �б�
string readShaderSource(const char* filePath) {
    ifstream shaderFile(filePath);
    stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    return shaderStream.str();
}

// ���̴� ����
GLuint createShader(const char* filePath, GLenum shaderType) {
    string shaderCode = readShaderSource(filePath);
    const char* shaderSource = shaderCode.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // ������ ���� üũ
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << endl;
    }

    return shader;
}

// ���̴� ���α׷� ����
void createShaderProgram() {
    GLuint vertexShader = createShader("vertex_shader.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader("fragment_shader.glsl", GL_FRAGMENT_SHADER);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // ���̴� ����
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
