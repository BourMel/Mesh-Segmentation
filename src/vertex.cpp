#include "vertex.hpp"

#include <cmath>

#include "edge.hpp"

float cotangentWeight(Edge *he);

Vertex::Vertex(glm::vec3 pos, Edge *edge) : m_pos(pos)
{
    static int id = 0;

    m_id = id++;
    m_edge = edge;
}

const glm::vec3& Vertex::pos() const
{
    return m_pos;
}

int Vertex::id() const
{
    return m_id;
}
Edge *Vertex::edge() const
{
    return m_edge;
}

void Vertex::pos(glm::vec3 p)
{
    m_pos = p;
}

void Vertex::edge(Edge *e)
{
    m_edge = e;
}

std::vector<Vertex *> Vertex::getNeighbours()
{
    std::vector<Vertex*> neighbours;

    // ...

    return neighbours;
}
