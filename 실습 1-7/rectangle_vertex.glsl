#version 330 core

out vec3 outColor;		// fragment shader와 통신!을 할 수 있게 해준다!

uniform vec2 uPos;
uniform vec3 uColor;

void main()
{
	// gl_VertexID		// 이 친구가 현재 그리고 있는 정점의 인덱스를 나타낸다!

	vec4 outPos;

	float size = 0.1f;

	if (gl_VertexID == 0){
		outPos = vec4(-size, size, 0.0, 1.0);		// 항상 실수 .0 혹은 .xxf, 1.f 등 실수로 나타내야 함!
		outColor = vec3(uColor);
	}
	else if (gl_VertexID == 1){
		outPos = vec4(-size, -size, 0.0, 1.0);
		outColor = vec3(uColor);
	}
	else if (gl_VertexID == 2){
		outPos = vec4(size, -size, 0.0, 1.0);
		outColor = vec3(uColor);
	}
	else if (gl_VertexID == 3){
		outPos = vec4(size, -size, 0.0, 1.0);		// 항상 실수 .0 혹은 .xxf, 1.f 등 실수로 나타내야 함!
		outColor = vec3(uColor);
	}
	else if (gl_VertexID == 4){
		outPos = vec4(size, size, 0.0, 1.0);
		outColor = vec3(uColor);
	}
	else if (gl_VertexID == 5){
		outPos = vec4(-size, size, 0.0, 1.0);
		outColor = vec3(uColor);
	}

	gl_Position = outPos + vec4(uPos, 0.f, 0.f);
}
