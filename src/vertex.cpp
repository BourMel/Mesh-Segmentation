#include "vertex.hpp"

#include <cmath>
#include <algorithm>

#include "edge.hpp"

float cotangentWeight(Edge *he);

Vertex::Vertex(glm::vec3 pos) : m_pos(pos)
{
    static int id = 0;

    m_id = id++;
    m_locked = false;
}

const glm::vec3& Vertex::pos() const
{
    return m_pos;
}

int Vertex::id() const
{
    return m_id;
}

bool Vertex::locked()
{
    return m_locked;
}

void Vertex::pos(glm::vec3 p)
{
    m_pos = p;
}

void Vertex::lock(bool l)
{
    m_locked = l;
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

void Vertex::bindEdges(std::vector<Edge*> toBind, Edge* toDelete)
{
    for(Edge *e : toBind)
    {
        if(e->v1() == toDelete->v1())
            e->v1(this);
        else if(e->v1() == toDelete->v2())
            e->v1(this);
        else if(e->v2() == toDelete->v1())
            e->v2(this);
        else if(e->v2() == toDelete->v2())
            e->v2(this);
    }
}
