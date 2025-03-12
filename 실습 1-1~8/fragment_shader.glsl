// Fragment Shader (fragment_shader.glsl)
#version 330 core

in vec3 fragColor; // 버텍스 셰이더에서 전달받은 색상
out vec4 finalColor; // 최종 색상

void main() {
    finalColor = vec4(fragColor, 1.0f); // RGBA 색상 출력
}
