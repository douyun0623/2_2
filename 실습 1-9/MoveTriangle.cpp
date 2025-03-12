#include "MoveTriangle.h"
#include "Scene.h"
#include <iostream>

MoveTriangle::MoveTriangle(float x, float y, float r, float g, float b, float size)
    : position{ x, y }, size(size), color{ r, g, b }, filled(true)
{
    // VAO, VBO ����
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // �ﰢ���� ���� ������
    float vertexArray[] = {
        x, y + size, r, g, b,  // ������ 1
        x - size, y - size, r, g, b,  // ������ 2
        x + size, y - size, r, g, b   // ������ 3
    };

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    // Vertex �Ӽ� ���� (��ġ�� ����)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // VAO ���ε� ����
    glBindVertexArray(0);
}

MoveTriangle::~MoveTriangle()
{
    // VAO�� VBO ����
    /*std::cout << "�Ҹ��� ȣ��\n";
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);*/
}

void MoveTriangle::draw(GLuint shaderID) const
{
    // ���̴� ���α׷� Ȱ��ȭ
   // glUseProgram(shaderID);

    // VAO ���ε�
    glBindVertexArray(VAO);

    // ������ ä��ų� ������ �׸��� ����
    //if (filled) {
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    //}
    //else {
    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    //}

    // �ﰢ�� �׸���
    glDrawArrays(GL_TRIANGLES, 0, 3);

    // VAO ���ε� ����
    glBindVertexArray(0);
}
