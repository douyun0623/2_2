#version 330 core

layout (location = 0) in vec2 vPos;

uniform mat4 modelPos;
uniform vec2 modelSize;

void main()
{
	vec2 pos[6];
	float temp = 0.05f;
	pos[0] = vec2(modelSize.x, modelSize.y -temp);
	pos[1] = vec2(-modelSize.x, modelSize.y-temp);
	pos[2] = vec2(modelSize.x, -modelSize.y-temp);
	pos[3] = vec2(-modelSize.x, modelSize.y-temp);
	pos[4] = vec2(-modelSize.x, -modelSize.y-temp);
	pos[5] = vec2(modelSize.x, -modelSize.y-temp);


	gl_Position = modelPos * vec4(pos[gl_VertexID], 0.f, 1.f);
}
