#define GLM_ENABLE_EXPERIMENTAL

#include "vertex.hpp"

#include <glm/gtx/string_cast.hpp>
#include <cmath>
#include <algorithm>
#include <sstream>
#include "edge.hpp"
#include "utils.hpp"

float cotangentWeight(Edge *he);

Vertex::Vertex(glm::vec3 pos) : m_pos(pos)
{
    static int id = 0;

    m_id = id++;
    m_locked = false;
    m_exportId = 0;
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

unsigned int Vertex::exportId()
{
    return m_exportId;
}

void Vertex::pos(glm::vec3 p)
{
    m_pos = p;
}

void Vertex::lock(bool l)
{
    m_locked = l;
    for(auto e : m_edges)
    {
        if(e->v1()->locked() && e->v2()->locked())
        {
            e->isLocked(true);
            e->type(Edge::BONE);
        }
    }
}

void Vertex::addEdge(Edge *e)
{
    m_edges.push_back(e);
}

void Vertex::removeEdge(Edge *e)
{
    auto f = find(m_edges,e);
    if(f != m_edges.end())
        m_edges.erase(f);
    if(m_edges.size()<=1)
        lock(true);
}

void Vertex::exportId(unsigned int id)
{
    m_exportId = id;
}
std::vector<Edge *> &Vertex::edges()
{
    return m_edges;
}

float cotangentWeight(Edge *he);

void Vertex::bindVertexEdges(std::vector<Edge*> toBind, Edge* edgeToDelete) {
	for(unsigned int i = 0; i < toBind.size(); i++) {
		if((toBind[i]->v1() == edgeToDelete->v1()) ||
			(toBind[i]->v1() == edgeToDelete->v2())) {
			toBind[i]->v1(this);
		} else if((toBind[i]->v2() == edgeToDelete->v1()) ||
			(toBind[i]->v1() == edgeToDelete->v2())) {
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

std::ostream &operator<<(std::ostream &o, Vertex &v)
{
    std::stringstream ss;
    ss << v.id() << ": " << glm::to_string(v.pos());
    ss << "{ ";
    for(auto e: v.edges())
    {
        ss << *e << " ";
    }
    ss << "}";
    return o << ss.str() ;
}
