#define GLM_ENABLE_EXPERIMENTAL

#include "edge.hpp"
#include "vertex.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

Edge::Edge(Vertex *first, Vertex* last)
{
    m_v1 = first;
    m_v2 = last;
}

float Edge::cost()
{
    return glm::length2(m_v2->pos()-m_v1->pos());
}

Vertex *Edge::getMeanPosition() {
	return new Vertex( glm::vec3((m_v1->pos().x + m_v2->pos().x )/2,
	(m_v1->pos().y + m_v2->pos().y)/2,
	(m_v1->pos().z + m_v2->pos().z)/2));
}

void Edge::addFace(Face *face)
{
    m_faces.push_back(face);
}

//getter
Vertex *Edge::v1() { return m_v1; }
Vertex *Edge::v2() { return m_v2; }

// setter
void Edge::v1(Vertex *v1) { m_v1 = v1; }
void Edge::v2(Vertex *v2) { m_v2 = v2; }
