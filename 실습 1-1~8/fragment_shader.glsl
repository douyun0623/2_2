// Fragment Shader (fragment_shader.glsl)
#version 330 core

in vec3 fragColor; // ���ؽ� ���̴����� ���޹��� ����
out vec4 finalColor; // ���� ����

void main() {
    finalColor = vec4(fragColor, 1.0f); // RGBA ���� ���
}
