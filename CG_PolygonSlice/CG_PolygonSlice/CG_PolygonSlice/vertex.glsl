#version 330 core

layout (location = 0) in vec3 vPos;

out vec3 outColor;

uniform mat4 modelTransform;
uniform mat4 rotateTransform;
uniform vec3 inColor;

void main()
{
	gl_Position = modelTransform * rotateTransform * vec4(vPos.x, vPos.y, 0.f, 1.f);
	outColor = inColor;
}
