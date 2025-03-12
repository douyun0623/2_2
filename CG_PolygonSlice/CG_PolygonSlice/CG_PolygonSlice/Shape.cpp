#include "Shape.h"

Shape::Shape(int vertexCnt, std::vector<glm::vec3> pos, glm::vec3 color, float speed)
    : vertexCnt(vertexCnt), posData(pos), color(color) {
	std::random_device rd;
	std::mt19937 gen(rd());  // 시드 생성 및 엔진 초기화

	switch (vertexCnt) {
	case 3:
		comVertexData.assign(triPos, triPos + 3);
		break;
	case 4:
		comVertexData.assign(rectPos, rectPos + 4);
		break;
	case 5:
		comVertexData.assign(pentagonPos, pentagonPos + 5);
		break;
	case 6:
		comVertexData.assign(hexagonPos, hexagonPos + 6);
		break;
	}

	// 각도 변화
	std::uniform_real_distribution<float> randAngle(1.0f, 2.f);
	this->deltaAngle = randAngle(gen);	// 각도

	// 초기 위치 설정
	std::uniform_real_distribution<float> dis(-0.7f, 0.5f);	// 초기 위치 설정
	this->pos.x = rand() % 2 == 0 ? -1.f : 1.0f;
	this->pos.y = dis(gen);

	// 속도 설정
	deltaTime += speed;

	std::uniform_real_distribution<float> randSpeed(1.5f, 2.f);
	deltaPos.x = this->pos.x < 0 ? randSpeed(gen) : -randSpeed(gen);

	std::uniform_real_distribution<float> randY(2.f, 3.f);
	deltaPos.y = randY(gen);

	initBuffer();

}

void Shape::setSpeed(float speed)
{
	deltaTime += speed;
}

void Shape::update()
{
	if (isSliced) {

		if (isSliceShapeExist) {	// 자른 도형이 존재할때만 업데이트
			for (auto it = slicedShapes.begin(); it != slicedShapes.end();) {
				it->update();

				if (it->pos.y < -2.f) {
					it = slicedShapes.erase(it);  // 삭제 후 iterator는 새로운 요소를 가리킴
					isSliceShapeExist = false;
				}
				else {
					++it;  // 삭제하지 않은 경우 iterator를 증가시켜 계속 진행
				}
			}
		}

	}
	else {	// 기본 도형 업데이트

		// 경로 
		PathVertex.push_back(pos);	// 지나온 경로
		updatePathBuffer();

		// pos 업데이트
		deltaPos.y -= gravity * deltaTime;		// 중력 적용
		deltaPos.x > 0 ? deltaPos.x -= dragCoefficient : deltaPos.x += dragCoefficient;	// 공기 저항 적용, * deltaPos.x * deltaTime

		pos += deltaPos * deltaTime;

		// 각도 업데이트
		angle += deltaAngle;
	}

}

void Shape::draw(GLuint shader) const
{
	// shape
	{
		glUseProgram(shader);

		// 모델링 변환
		glm::mat4 matrix(1.f);
		GLint worldLoc = glGetUniformLocation(shader, "modelTransform");
		if (worldLoc < 0)
			std::cout << "worldLoc 찾지 못함\n";
		GLint colorLoc = glGetUniformLocation(shader, "inColor");
		if (colorLoc < 0)
			std::cout << "colorLoc 찾지 못함\n";

		glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(matrix));
		glUniform3fv(colorLoc, 1, glm::value_ptr(color));

		// 모델 변환
		glm::mat4 unitMat(1.f);
		glm::mat4 initPos = glm::translate(unitMat, glm::vec3(pos.x, pos.y, 0.f));
		glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(initPos));


		// posData의 점의 개수를 센다.
		int drawVertexNum = 0;
		for (const auto& pd : posData) {
			++drawVertexNum;
		}


		// 그리기
		if (isSliced) {
			for (auto& sliceShape : slicedShapes) {
				sliceShape.draw(shader);
			}
		}
		else {
			glBindVertexArray(VAO);
			glDrawArrays(GL_TRIANGLES, 0, drawVertexNum);
			glBindVertexArray(0);
		}
	}

}

void Shape::pathDraw(GLuint shader2) const
{
	glUseProgram(shader2);

	glLineWidth(3.0f);  // 선의 굵기를 3.0으로 설정

	glBindVertexArray(VAO_path);
	glDrawArrays(GL_LINE_STRIP, 0, PathVertex.size());
	glBindVertexArray(0);

}

void Shape::initBuffer()
{
	// shape
	{
		// VAO 설정
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//vbo
		glGenBuffers(1, &VBO_pos);

		// pos 데이터 VBO에 업로드
		glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
		glBufferData(GL_ARRAY_BUFFER, posData.size() * sizeof(glm::vec3), posData.data(), GL_STATIC_DRAW);

		// 포지션 속성 설정 (x, y, z 좌표)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		// 정리
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// 경로
	{
		// VAO 설정
		glGenVertexArrays(1, &VAO_path);
		glBindVertexArray(VAO_path);

		//vbo
		glGenBuffers(1, &VBO_path);

		// pos 데이터 VBO에 업로드
		glBindBuffer(GL_ARRAY_BUFFER, VBO_path);
		glBufferData(GL_ARRAY_BUFFER, PathVertex.size() * sizeof(glm::vec2), PathVertex.data(), GL_STATIC_DRAW);

		// 포지션 속성 설정 (x, y, z 좌표)
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
		glEnableVertexAttribArray(0);

		// 정리
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

}

void Shape::updatePathBuffer()
{
	glBindVertexArray(VAO_path);  // VAO 바인딩
	glBindBuffer(GL_ARRAY_BUFFER, VBO_path);  // VBO 바인딩

	// VBO에 새 경로 데이터를 업데이트
	glBufferData(GL_ARRAY_BUFFER, PathVertex.size() * sizeof(glm::vec2), PathVertex.data(), GL_DYNAMIC_DRAW);

	// 포지션 속성 설정 (x, y 좌표)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);  // 정리
	glBindVertexArray(0);

}

bool Shape::isIntersecting(glm::vec2 A1, glm::vec2 A2, glm::vec2 B1, glm::vec2 B2, glm::vec2& intersection)
{
	float denominator = (A1.x - A2.x) * (B1.y - B2.y) - (A1.y - A2.y) * (B1.x - B2.x);

	// 두 선분이 평행한 경우 교차하지 않음
	if (denominator == 0) return false;

	float t = ((A1.x - B1.x) * (B1.y - B2.y) - (A1.y - B1.y) * (B1.x - B2.x)) / denominator;
	float u = ((A1.x - B1.x) * (A1.y - A2.y) - (A1.y - B1.y) * (A1.x - A2.x)) / denominator;

	if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
		intersection = A1 + t * (A2 - A1); // 교차점 좌표 계산
		return true;
	}

	return false;
}

void Shape::slice(glm::vec2 start, glm::vec2 end)
{
	std::vector<glm::vec2> intersectionPoints;  // 교차점 저장 벡터
	std::vector<int> intersectedEdges;  // 교차된 변의 인덱스를 저장하는 벡터

	// 도형의 모든 변을 검사하여 교차점 찾기
	for (int i = 0; i < vertexCnt; i++) {
		// comVertexData + pos 형태로 변환하여 현재 변의 두 점을 계산
		glm::vec2 P1 = glm::vec2(comVertexData[i].x + pos.x, comVertexData[i].y + pos.y);
		glm::vec2 P2 = glm::vec2(comVertexData[(i + 1) % vertexCnt].x + pos.x, comVertexData[(i + 1) % vertexCnt].y + pos.y);
		glm::vec2 intersection;  // 교차점

		// 현재 변과 슬라이스 선(start, end)이 교차하는지 확인
		if (isIntersecting(start, end, P1, P2, intersection)) {
			intersectionPoints.push_back(intersection);  // 교차점 저장
			intersectedEdges.push_back(i);  // 교차된 변의 인덱스 저장
		}
	}

	if (intersectionPoints.size() >= 2) {
		// 교차점이 2개 이상일 때 슬라이스 진행
		isSliced = true;
		this->intersectedEdges = intersectedEdges;  // 교차된 변의 인덱스를 저장
		createSlicedShapes(intersectionPoints, start, end);
	}
	else {
		return;
	}

}

void Shape::createSlicedShapes(const std::vector<glm::vec2>& intersectionPoints, const glm::vec2& start, const glm::vec2& end)
{
	if (intersectionPoints.size() != 2) return;

	std::vector<glm::vec3> shape1, shape2;

	int firstIntersectIdx = this->intersectedEdges[0];
	int secondIntersectIdx = this->intersectedEdges[1];

	// 첫 번째 슬라이스 도형
	std::vector<glm::vec3> tempShape1;
	tempShape1.push_back(glm::vec3(intersectionPoints[0], 0.0f));  // 첫 번째 교차점
	for (int i = firstIntersectIdx + 1; i <= secondIntersectIdx; i++) {
		tempShape1.push_back(glm::vec3(pos, 0.0f) + comVertexData[i % vertexCnt]);
	}
	tempShape1.push_back(glm::vec3(intersectionPoints[1], 0.0f));  // 두 번째 교차점 추가

	// 첫 번째 슬라이스 도형을 삼각형으로 분해하여 shape1에 저장
	for (int i = 1; i < tempShape1.size() - 1; i++) {
		shape1.push_back(tempShape1[0]);      // 첫 번째 교차점
		shape1.push_back(tempShape1[i]);      // 현재 점
		shape1.push_back(tempShape1[i + 1]);  // 다음 점
	}

	// 두 번째 슬라이스 도형
	std::vector<glm::vec3> tempShape2;
	tempShape2.push_back(glm::vec3(intersectionPoints[1], 0.0f));  // 두 번째 교차점
	for (int i = secondIntersectIdx + 1; i != firstIntersectIdx + 1; i = (i + 1) % vertexCnt) {
		tempShape2.push_back(glm::vec3(pos, 0.0f) + comVertexData[i % vertexCnt]);
	}
	tempShape2.push_back(glm::vec3(intersectionPoints[0], 0.0f));  // 첫 번째 교차점 추가

	// 두 번째 슬라이스 도형을 삼각형으로 분해하여 shape2에 저장
	for (int i = 1; i < tempShape2.size() - 1; i++) {
		shape2.push_back(tempShape2[0]);      // 두 번째 교차점
		shape2.push_back(tempShape2[i]);      // 현재 점
		shape2.push_back(tempShape2[i + 1]);  // 다음 점
	}

	for (auto& s1 : shape1) {
		s1.x -= pos.x;
		s1.y -= pos.y;
	}
	for (auto& s2 : shape2) {
		s2.x -= pos.x;
		s2.y -= pos.y;
	}

	// 슬라이스된 도형들 저장
	if (!shape1.empty()) {
		slicedShapes.emplace_back(shape1, pos, deltaPos.x, deltaPos.y, deltaTime);
	}
	if (!shape2.empty()) {
		slicedShapes.emplace_back(shape2, pos, -deltaPos.x, deltaPos.y, deltaTime);
	}

}