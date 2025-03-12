// Vertex Shader (vertex_shader.glsl)
#version 330 core

layout(location = 0) in vec2 position; // ��ġ
layout(location = 1) in vec3 color;    // ����

out vec3 fragColor; // �����׸�Ʈ ���̴��� ������ ����

void main() {
    gl_Position = vec4(position, 0.0, 1.0); // OpenGL ��ǥ�� ��ȯ
    fragColor = color; // ���� ����
}
