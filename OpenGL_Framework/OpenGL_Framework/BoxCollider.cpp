#include "BoxCollider.h"
#include <vector>


BoxCollider::BoxCollider()
{
}


BoxCollider::~BoxCollider()
{
}

void BoxCollider::update(float deltaTime)
{
}

void BoxCollider::draw(Transform* Meshtransform, float NumVertices)
{

	/*glm::vec3 size = glm::vec3(m_maxBound.x - m_minBound.x, m_maxBound.y - m_minBound.y, m_maxBound.z - m_minBound.z);
	glm::vec3 center = glm::vec3((m_minBound.x + m_maxBound.x) / 2, (m_minBound.y + m_maxBound.y) / 2, (m_minBound.z + m_maxBound.z) / 2);*/
	
	//boxTransform->m_pLocalToWorldMatrix = glm::translate(glm::mat4(1), center) * glm::scale(glm::mat4(1), size);

	///* Apply object's transformation matrix */
	//glm::mat4 m = Meshtransform->getLocalToWorldMatrix() * boxTransform->getLocalToWorldMatrix();
	//std::vector<glm::vec3> points;
	//points.resize(8);

	//points[0] = { m_minBound.x, m_minBound.y, m_minBound.z };
	//points[1] = { m_maxBound.x, m_minBound.y, m_minBound.z };
	//points[2] = { m_maxBound.x, m_minBound.y, m_maxBound.z };
	//points[3] = { m_minBound.x, m_minBound.y, m_maxBound.z };
	//points[4] = { m_minBound.x, m_maxBound.y, m_minBound.z };
	//points[5] = { m_maxBound.x, m_maxBound.y, m_minBound.z };
	//points[6] = { m_maxBound.x, m_maxBound.y, m_maxBound.z };
	//points[7] = { m_minBound.x, m_maxBound.y, m_maxBound.z };
	//


	//for (int i = 0; i < 4; i++) {
	//	if (i == 3) {
	//		line(points[i], points[i - 3], glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//	}
	//	else {
	//		line(points[i], points[i + 1], glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//	}
	//}

	//for (int i= 4; i < 7; i++) {
	//	if (i == 4) {
	//		line(points[i], points[i - 3], glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//	}
	//	else {
	//		line(points[i], points[i + 1], glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//	}
	//}

	//for (int i = 0; i < 4; i++) {
	//	line(points[i], points[i + 4], glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
	//}

}

void BoxCollider::line(glm::vec3 p0, glm::vec3 p1, glm::vec4 color)
{
	glLineWidth(3);
	float line_vertex[] =
	{
		p0.x,p0.x,p0.z,
		p1.x,p1.x,p1.z
	};
	float line_color[] =
	{
		color.x, color.y, color.z,
		color.x, color.y, color.z
	};
	glVertexPointer(3, GL_FLOAT, 0, line_vertex);
	glColorPointer(3, GL_FLOAT, 0, line_color);
	glDrawArrays(GL_LINES, 0, 2);
}




              
