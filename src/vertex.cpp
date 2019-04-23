#include "vertex.hpp"

#include <cmath>
#include <algorithm>

#include "edge.hpp"

float cotangentWeight(Edge *he);

Vertex::Vertex(glm::vec3 pos) : m_pos(pos)
{
    static int id = 0;

    m_id = id++;
}

const glm::vec3& Vertex::pos() const
{
    return m_pos;
}

int Vertex::id() const
{
    return m_id;
}

void Vertex::pos(glm::vec3 p)
{
    m_pos = p;
}

void Vertex::addEdge(Edge *e)
{
    m_edges.push_back(e);
}

std::vector<Edge *> &Vertex::edges()
{
    return m_edges;
}

float cotangentWeight(Edge *he);

void Vertex::bindVertexEdges(std::vector<Edge*> toBind, Edge* edgeToDelete) {
	for(unsigned int i = 0; i < toBind.size(); i++) {
		if(glm::all(glm::equal(toBind[i]->v1()->pos(), edgeToDelete->v1()->pos())) ||
			glm::all(glm::equal(toBind[i]->v1()->pos(), edgeToDelete->v2()->pos()))) {
			toBind[i]->v1(this);
		} else if(glm::all(glm::equal(toBind[i]->v2()->pos(), edgeToDelete->v1()->pos())) ||
			glm::all(glm::equal(toBind[i]->v1()->pos(), edgeToDelete->v2()->pos()))) {
			toBind[i]->v2(this);
		}
	}
}
