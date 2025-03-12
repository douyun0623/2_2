#version 330 core

layout (location = 0) in vec2 vPos;
layout (location = 1) in vec3 vColor;

out vec3 outColor;		// fragment shader와 통신!을 할 수 있게 해준다!

//uniform vec2 uPos;		// C++ 프로그램(CPU)과 통신(값을 주고받을 수 있다) CPU프로그램과 전역변수 느낌으로 값을 주고받는다
						// 내가 원하는 시점에!
						// 이 친구는 어떠한 것으로 연결되어서, 우리가 접근 가능하도록 인덱스를 생성해 준다!

void main()
{
	gl_Position = vec4(vPos, 0.f, 1.f);
	outColor = vColor;
}
