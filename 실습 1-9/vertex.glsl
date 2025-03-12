#version 330 core

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec3 vColor;

out vec3 outColor;		// fragment shader�� ���!�� �� �� �ְ� ���ش�!

//uniform vec2 uPos;		// C++ ���α׷�(CPU)�� ���(���� �ְ���� �� �ִ�) CPU���α׷��� �������� �������� ���� �ְ�޴´�
						// ���� ���ϴ� ������!
						// �� ģ���� ��� ������ ����Ǿ, �츮�� ���� �����ϵ��� �ε����� ������ �ش�!

void main()
{
	gl_Position = vec4(vPos, 0.f, 1.f);
	outColor = vColor;
}
