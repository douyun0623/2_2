#include "Shape.h"

Shape::Shape(int vertexCnt, std::vector<glm::vec3> pos, glm::vec3 color, float speed)
    : vertexCnt(vertexCnt), posData(pos), color(color) {
	std::random_device rd;
	std::mt19937 gen(rd());  // �õ� ���� �� ���� �ʱ�ȭ

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

	// ���� ��ȭ
	std::uniform_real_distribution<float> randAngle(1.0f, 2.f);
	this->deltaAngle = randAngle(gen);	// ����

	// �ʱ� ��ġ ����
	std::uniform_real_distribution<float> dis(-0.7f, 0.5f);	// �ʱ� ��ġ ����
	this->pos.x = rand() % 2 == 0 ? -1.f : 1.0f;
	this->pos.y = dis(gen);

	// �ӵ� ����
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

		if (isSliceShapeExist) {	// �ڸ� ������ �����Ҷ��� ������Ʈ
			for (auto it = slicedShapes.begin(); it != slicedShapes.end();) {
				it->update();

				if (it->pos.y < -2.f) {
					it = slicedShapes.erase(it);  // ���� �� iterator�� ���ο� ��Ҹ� ����Ŵ
					isSliceShapeExist = false;
				}
				else {
					++it;  // �������� ���� ��� iterator�� �������� ��� ����
				}
			}
		}

	}
	else {	// �⺻ ���� ������Ʈ

		// ��� 
		PathVertex.push_back(pos);	// ������ ���
		updatePathBuffer();

		// pos ������Ʈ
		deltaPos.y -= gravity * deltaTime;		// �߷� ����
		deltaPos.x > 0 ? deltaPos.x -= dragCoefficient : deltaPos.x += dragCoefficient;	// ���� ���� ����, * deltaPos.x * deltaTime

		pos += deltaPos * deltaTime;

		// ���� ������Ʈ
		angle += deltaAngle;
	}

}

void Shape::draw(GLuint shader) const
{
	// shape
	{
		glUseProgram(shader);

		// �𵨸� ��ȯ
		glm::mat4 matrix(1.f);
		GLint worldLoc = glGetUniformLocation(shader, "modelTransform");
		if (worldLoc < 0)
			std::cout << "worldLoc ã�� ����\n";
		GLint colorLoc = glGetUniformLocation(shader, "inColor");
		if (colorLoc < 0)
			std::cout << "colorLoc ã�� ����\n";

		glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(matrix));
		glUniform3fv(colorLoc, 1, glm::value_ptr(color));

		// �� ��ȯ
		glm::mat4 unitMat(1.f);
		glm::mat4 initPos = glm::translate(unitMat, glm::vec3(pos.x, pos.y, 0.f));
		glUniformMatrix4fv(worldLoc, 1, GL_FALSE, glm::value_ptr(initPos));


		// posData�� ���� ������ ����.
		int drawVertexNum = 0;
		for (const auto& pd : posData) {
			++drawVertexNum;
		}


		// �׸���
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

	glLineWidth(3.0f);  // ���� ���⸦ 3.0���� ����

	glBindVertexArray(VAO_path);
	glDrawArrays(GL_LINE_STRIP, 0, PathVertex.size());
	glBindVertexArray(0);

}

void Shape::initBuffer()
{
	// shape
	{
		// VAO ����
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		//vbo
		glGenBuffers(1, &VBO_pos);

		// pos ������ VBO�� ���ε�
		glBindBuffer(GL_ARRAY_BUFFER, VBO_pos);
		glBufferData(GL_ARRAY_BUFFER, posData.size() * sizeof(glm::vec3), posData.data(), GL_STATIC_DRAW);

		// ������ �Ӽ� ���� (x, y, z ��ǥ)
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glEnableVertexAttribArray(0);

		// ����
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	// ���
	{
		// VAO ����
		glGenVertexArrays(1, &VAO_path);
		glBindVertexArray(VAO_path);

		//vbo
		glGenBuffers(1, &VBO_path);

		// pos ������ VBO�� ���ε�
		glBindBuffer(GL_ARRAY_BUFFER, VBO_path);
		glBufferData(GL_ARRAY_BUFFER, PathVertex.size() * sizeof(glm::vec2), PathVertex.data(), GL_STATIC_DRAW);

		// ������ �Ӽ� ���� (x, y, z ��ǥ)
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
		glEnableVertexAttribArray(0);

		// ����
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

}

void Shape::updatePathBuffer()
{
	glBindVertexArray(VAO_path);  // VAO ���ε�
	glBindBuffer(GL_ARRAY_BUFFER, VBO_path);  // VBO ���ε�

	// VBO�� �� ��� �����͸� ������Ʈ
	glBufferData(GL_ARRAY_BUFFER, PathVertex.size() * sizeof(glm::vec2), PathVertex.data(), GL_DYNAMIC_DRAW);

	// ������ �Ӽ� ���� (x, y ��ǥ)
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);  // ����
	glBindVertexArray(0);

}

bool Shape::isIntersecting(glm::vec2 A1, glm::vec2 A2, glm::vec2 B1, glm::vec2 B2, glm::vec2& intersection)
{
	float denominator = (A1.x - A2.x) * (B1.y - B2.y) - (A1.y - A2.y) * (B1.x - B2.x);

	// �� ������ ������ ��� �������� ����
	if (denominator == 0) return false;

	float t = ((A1.x - B1.x) * (B1.y - B2.y) - (A1.y - B1.y) * (B1.x - B2.x)) / denominator;
	float u = ((A1.x - B1.x) * (A1.y - A2.y) - (A1.y - B1.y) * (A1.x - A2.x)) / denominator;

	if (t >= 0 && t <= 1 && u >= 0 && u <= 1) {
		intersection = A1 + t * (A2 - A1); // ������ ��ǥ ���
		return true;
	}

	return false;
}

void Shape::slice(glm::vec2 start, glm::vec2 end)
{
	std::vector<glm::vec2> intersectionPoints;  // ������ ���� ����
	std::vector<int> intersectedEdges;  // ������ ���� �ε����� �����ϴ� ����

	// ������ ��� ���� �˻��Ͽ� ������ ã��
	for (int i = 0; i < vertexCnt; i++) {
		// comVertexData + pos ���·� ��ȯ�Ͽ� ���� ���� �� ���� ���
		glm::vec2 P1 = glm::vec2(comVertexData[i].x + pos.x, comVertexData[i].y + pos.y);
		glm::vec2 P2 = glm::vec2(comVertexData[(i + 1) % vertexCnt].x + pos.x, comVertexData[(i + 1) % vertexCnt].y + pos.y);
		glm::vec2 intersection;  // ������

		// ���� ���� �����̽� ��(start, end)�� �����ϴ��� Ȯ��
		if (isIntersecting(start, end, P1, P2, intersection)) {
			intersectionPoints.push_back(intersection);  // ������ ����
			intersectedEdges.push_back(i);  // ������ ���� �ε��� ����
		}
	}

	if (intersectionPoints.size() >= 2) {
		// �������� 2�� �̻��� �� �����̽� ����
		isSliced = true;
		this->intersectedEdges = intersectedEdges;  // ������ ���� �ε����� ����
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

	// ù ��° �����̽� ����
	std::vector<glm::vec3> tempShape1;
	tempShape1.push_back(glm::vec3(intersectionPoints[0], 0.0f));  // ù ��° ������
	for (int i = firstIntersectIdx + 1; i <= secondIntersectIdx; i++) {
		tempShape1.push_back(glm::vec3(pos, 0.0f) + comVertexData[i % vertexCnt]);
	}
	tempShape1.push_back(glm::vec3(intersectionPoints[1], 0.0f));  // �� ��° ������ �߰�

	// ù ��° �����̽� ������ �ﰢ������ �����Ͽ� shape1�� ����
	for (int i = 1; i < tempShape1.size() - 1; i++) {
		shape1.push_back(tempShape1[0]);      // ù ��° ������
		shape1.push_back(tempShape1[i]);      // ���� ��
		shape1.push_back(tempShape1[i + 1]);  // ���� ��
	}

	// �� ��° �����̽� ����
	std::vector<glm::vec3> tempShape2;
	tempShape2.push_back(glm::vec3(intersectionPoints[1], 0.0f));  // �� ��° ������
	for (int i = secondIntersectIdx + 1; i != firstIntersectIdx + 1; i = (i + 1) % vertexCnt) {
		tempShape2.push_back(glm::vec3(pos, 0.0f) + comVertexData[i % vertexCnt]);
	}
	tempShape2.push_back(glm::vec3(intersectionPoints[0], 0.0f));  // ù ��° ������ �߰�

	// �� ��° �����̽� ������ �ﰢ������ �����Ͽ� shape2�� ����
	for (int i = 1; i < tempShape2.size() - 1; i++) {
		shape2.push_back(tempShape2[0]);      // �� ��° ������
		shape2.push_back(tempShape2[i]);      // ���� ��
		shape2.push_back(tempShape2[i + 1]);  // ���� ��
	}

	for (auto& s1 : shape1) {
		s1.x -= pos.x;
		s1.y -= pos.y;
	}
	for (auto& s2 : shape2) {
		s2.x -= pos.x;
		s2.y -= pos.y;
	}

	// �����̽��� ������ ����
	if (!shape1.empty()) {
		slicedShapes.emplace_back(shape1, pos, deltaPos.x, deltaPos.y, deltaTime);
	}
	if (!shape2.empty()) {
		slicedShapes.emplace_back(shape2, pos, -deltaPos.x, deltaPos.y, deltaTime);
	}

}