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

// 삼각형 데이터 구조체
struct Triangle {
    float x, y;      // 중심 위치
    float size;      // 크기
    float color[3];  // RGB 색상

    Triangle(float x, float y, float size) : x(x), y(y), size(size) {
        // 랜덤 색상 생성
        color[0] = static_cast<float>(rand()) / RAND_MAX; // R
        color[1] = static_cast<float>(rand()) / RAND_MAX; // G
        color[2] = static_cast<float>(rand()) / RAND_MAX; // B
    }
};

vector<Triangle> quadrants[4]; // 각 사분면의 삼각형 저장
bool filled = true;              // 면 또는 선으로 그리기 전환

GLuint vao, shaderProgram;

// 함수 프로토타입
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
    glutInit(&argc, argv); // GLUT 초기화
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // 디스플레이 모드 설정
    glutInitWindowPosition(50, 100); // 윈도우 위치
    glutInitWindowSize(800, 600); // 윈도우 크기
    glutCreateWindow("Triangle Drawing with Shaders"); // 윈도우 생성

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        cerr << "Unable to initialize GLEW" << endl;
        exit(EXIT_FAILURE);
    }
    else {
        cout << "GLEW Initialized\n";
    }

    createShaderProgram(); // 셰이더 프로그램 생성

    glutDisplayFunc(drawScene); // 출력 함수 지정
    glutReshapeFunc(Reshape); // 크기 변경 시 다시 그리기 함수 지정
    glutMouseFunc(Mouse); // 마우스 클릭 처리 함수 지정
    glutKeyboardFunc(keyboard); // 키보드 입력 처리 함수 지정
    glutMainLoop(); // 이벤트 처리 시작
}

GLvoid drawScene() {
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // 배경색 설정
    glClear(GL_COLOR_BUFFER_BIT); // 색으로 화면 지우기

    glUseProgram(shaderProgram); // 셰이더 프로그램 사용

    // 각 사분면의 삼각형 그리기
    for (int i = 0; i < 4; ++i) {
        drawQuadrantTriangles(i);
    }

    glutSwapBuffers(); // 화면에 출력
}

GLvoid Reshape(int w, int h) {
    glViewport(0, 0, w, h); // 뷰포트 크기 조정
}

GLvoid Mouse(int button, int state, int x, int y) {
    if (state == GLUT_DOWN) {
        // OpenGL 좌표로 변환
        float gl_x = x;
        float gl_y = y;
        window_to_gl(gl_x, gl_y);

        int quadrant = -1;
        // 사분면 판단
        if (gl_x >= 0 && gl_y >= 0) quadrant = 0; // 제1사분면
        else if (gl_x < 0 && gl_y >= 0) quadrant = 1; // 제2사분면
        else if (gl_x < 0 && gl_y < 0) quadrant = 2; // 제3사분면
        else if (gl_x >= 0 && gl_y < 0) quadrant = 3; // 제4사분면

        if (button == GLUT_LEFT_BUTTON && quadrant != -1) {
            // 왼쪽 마우스 클릭: 기존 삼각형 제거 후 새로운 삼각형 추가
            if (!quadrants[quadrant].empty()) {
                quadrants[quadrant].pop_back(); // 기존 삼각형 제거
            }
            quadrants[quadrant].emplace_back(gl_x, gl_y, static_cast<float>(rand()) / RAND_MAX * 0.1f); // 새로운 삼각형 추가
        }
        else if (button == GLUT_RIGHT_BUTTON && quadrant != -1) {
            // 오른쪽 마우스 클릭: 새로운 삼각형 추가 (최대 3개)
            if (quadrants[quadrant].size() < 3) {
                quadrants[quadrant].emplace_back(gl_x, gl_y, static_cast<float>(rand()) / RAND_MAX * 0.1f);
            }
        }

        glutPostRedisplay(); // 화면 갱신
    }
}

GLvoid keyboard(unsigned char key, int x, int y) {
    if (key == 'a') {
        filled = true; // 면으로 그리기
    }
    else if (key == 'b') {
        filled = false; // 선으로 그리기
    }
    glutPostRedisplay(); // 화면 갱신
}

void drawTriangle(const Triangle& triangle) {
    // 정점과 색상 데이터 배열
    float vertices[18] = {
        triangle.x, triangle.y + triangle.size, // 위 꼭지점
        triangle.color[0], triangle.color[1], triangle.color[2], // 위 꼭지점 색상

        triangle.x - triangle.size, triangle.y - triangle.size, // 왼쪽 꼭지점
        triangle.color[0], triangle.color[1], triangle.color[2], // 왼쪽 꼭지점 색상

        triangle.x + triangle.size, triangle.y - triangle.size, // 오른쪽 꼭지점
        triangle.color[0], triangle.color[1], triangle.color[2]  // 오른쪽 꼭지점 색상
    };

    glBindVertexArray(vao); // VAO 바인딩

    // VBO 생성 및 데이터 전송
    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // 위치 속성 설정
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);
    glEnableVertexAttribArray(0);

    // 색상 속성 설정
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 2));
    glEnableVertexAttribArray(1);

    // 삼각형 그리기
    if (filled) {
        glDrawArrays(GL_TRIANGLES, 0, 3); // 면으로 그리기
    }
    else {
        glDrawArrays(GL_LINE_LOOP, 0, 3); // 선으로 그리기
    }

    glDeleteBuffers(1, &vbo); // VBO 삭제
}

void drawQuadrantTriangles(int quadrant) {
    for (const auto& triangle : quadrants[quadrant]) {
        drawTriangle(triangle);
    }
}

// 윈도우 좌표계를 OpenGL 좌표계로 변환
void window_to_gl(float& x, float& y) {
    float windowWidth = glutGet(GLUT_WINDOW_WIDTH);
    float windowHeight = glutGet(GLUT_WINDOW_HEIGHT);

    // X 좌표 변환 (OpenGL 좌표계에서 왼쪽이 -1, 오른쪽이 1)
    x = (float)x / (windowWidth / 2) - 1.0f;

    // Y 좌표 변환 (OpenGL 좌표계에서 아래가 -1, 위가 1)
    y = 1.0f - (float)y / (windowHeight / 2);
}

// 셰이더 소스 코드 읽기
string readShaderSource(const char* filePath) {
    ifstream shaderFile(filePath);
    stringstream shaderStream;
    shaderStream << shaderFile.rdbuf();
    return shaderStream.str();
}

// 셰이더 생성
GLuint createShader(const char* filePath, GLenum shaderType) {
    string shaderCode = readShaderSource(filePath);
    const char* shaderSource = shaderCode.c_str();

    GLuint shader = glCreateShader(shaderType);
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    // 컴파일 에러 체크
    GLint success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        cerr << "ERROR::SHADER::COMPILATION_FAILED\n" << infoLog << endl;
    }

    return shader;
}

// 셰이더 프로그램 생성
void createShaderProgram() {
    GLuint vertexShader = createShader("vertex_shader.glsl", GL_VERTEX_SHADER);
    GLuint fragmentShader = createShader("fragment_shader.glsl", GL_FRAGMENT_SHADER);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // 셰이더 삭제
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}
