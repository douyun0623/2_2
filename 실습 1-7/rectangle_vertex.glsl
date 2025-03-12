#version 330 core

out vec3 outColor;		// fragment shader�� ���!�� �� �� �ְ� ���ش�!

uniform vec2 uPos;
uniform vec3 uColor;

void main()
{
	// gl_VertexID		// �� ģ���� ���� �׸��� �ִ� ������ �ε����� ��Ÿ����!

	vec4 outPos;

	float size = 0.1f;

	if (gl_VertexID == 0){
		outPos = vec4(-size, size, 0.0, 1.0);		// �׻� �Ǽ� .0 Ȥ�� .xxf, 1.f �� �Ǽ��� ��Ÿ���� ��!
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
		outPos = vec4(size, -size, 0.0, 1.0);		// �׻� �Ǽ� .0 Ȥ�� .xxf, 1.f �� �Ǽ��� ��Ÿ���� ��!
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
