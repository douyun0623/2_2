#version 330 core

uniform vec4 startEndPos;

void main()
{
	if (0 == gl_VertexID)
		gl_Position = vec4(startEndPos.x, startEndPos.y, 0.f, 1);
	else
		gl_Position = vec4(startEndPos.z, startEndPos.w, 0.f, 1);
}
