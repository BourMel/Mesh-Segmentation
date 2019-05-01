#define GLM_ENABLE_EXPERIMENTAL

#include "edge.hpp"
#include "vertex.hpp"
#include "utils.hpp"

#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

Edge::Edge(Vertex *first, Vertex *last)
{
    m_v1 = first;
    m_v2 = last;
    m_cost = glm::length2(m_v2->pos() - m_v1->pos());
}

Vertex *Edge::getMeanPosition()
{
    return new Vertex(glm::vec3((m_v1->pos().x + m_v2->pos().x) / 2,
                                (m_v1->pos().y + m_v2->pos().y) / 2,
                                (m_v1->pos().z + m_v2->pos().z) / 2));
}

std::vector<Edge*> Edge::getConnectedEdges()
{
    std::vector<Edge*> edges;

    edges = mergeVector(m_v1->edges(), m_v2->edges());
    auto it = std::find(edges.begin(), edges.end(), this);
    if(it != edges.end())
    {
        edges.erase(it);
    }
    
    return edges;
}

void Edge::addFace(Face *face)
{
    m_faces.push_back(face);
}

void Edge::addFaceATL(Face *face)
{
    m_ATL.push_back(face);
}

void Edge::computeCost()
{
    m_cost = glm::length2(m_v2->pos()-m_v1->pos());
}

//getter
Vertex *Edge::v1() { return m_v1; }
Vertex *Edge::v2() { return m_v2; }
std::vector<Face*> &Edge::faces() { return m_faces; }
std::vector<Face*> &Edge::ATL() { return m_ATL; }
Edge::EdgeType Edge::type() { return m_type; }
float Edge::cost() { return m_cost; }

// setter
void Edge::v1(Vertex *v1) { m_v1 = v1; }
void Edge::v2(Vertex *v2) { m_v2 = v2; }
void Edge::type(EdgeType type) { m_type = type; }
