// Vertex Shader (vertex_shader.glsl)
#version 330 core

layout(location = 0) in vec2 position; // 위치
layout(location = 1) in vec3 color;    // 색상

out vec3 fragColor; // 프래그먼트 셰이더로 전달할 색상

void main() {
    gl_Position = vec4(position, 0.0, 1.0); // OpenGL 좌표로 변환
    fragColor = color; // 색상 전달
}
