#include "MoveTriangle.h"
#include "Scene.h"
#include <iostream>

MoveTriangle::MoveTriangle(float x, float y, float r, float g, float b, float size)
    : position{ x, y }, size(size), color{ r, g, b }, filled(true)
{
    // VAO, VBO 생성
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // 삼각형의 정점 데이터
    float vertexArray[] = {
        x, y + size, r, g, b,  // 꼭짓점 1
        x - size, y - size, r, g, b,  // 꼭짓점 2
        x + size, y - size, r, g, b   // 꼭짓점 3
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    // Vertex 속성 설정 (위치와 색상)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // VAO 바인딩 해제
    glBindVertexArray(0);
}

MoveTriangle::~MoveTriangle()
{
    // VAO와 VBO 삭제
    /*std::cout << "소멸자 호출\n";
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);*/
}

void MoveTriangle::draw(GLuint shaderID) const
{
    // 쉐이더 프로그램 활성화
   // glUseProgram(shaderID);

    // VAO 바인딩
    glBindVertexArray(VAO);

    // 도형을 채우거나 선으로 그릴지 설정
    //if (filled) {
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //}
    //else {
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //}

    // 삼각형 그리기
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // VAO 바인딩 해제
    glBindVertexArray(0);
}
