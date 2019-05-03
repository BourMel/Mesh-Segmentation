#define GLM_ENABLE_EXPERIMENTAL

#include "edge.hpp"
#include "vertex.hpp"
#include "utils.hpp"

#include <ostream>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

Edge::Edge(Vertex *first, Vertex *last)
{
    static int i = 0;
    m_id = i;
    m_v1 = first;
    m_v2 = last;
    m_cost = glm::length2(m_v2->pos() - m_v1->pos());
    m_isLocked = false;

    i++;
}

Vertex *Edge::getMeanPosition()
{
    return new Vertex(glm::vec3((m_v1->pos().x + m_v2->pos().x) / 2,
                                (m_v1->pos().y + m_v2->pos().y) / 2,
                                (m_v1->pos().z + m_v2->pos().z) / 2));
}

bool Edge::intersectWith(Edge* e) {
    Vertex* A = m_v1;
    Vertex* B = m_v2;
    Vertex* C = e->v1();
    Vertex* D = e->v2();

    glm::mat3 ABC, ABD, CDA, CDB;

    // intersection if det(ABC) and det(ABD) are opposed
    ABC = glm::mat3(
        A->pos().x, A->pos().y, A->pos().z,
        B->pos().x, B->pos().y, B->pos().z,
        C->pos().x, C->pos().y, C->pos().z);
    ABD = glm::mat3(
        A->pos().x, A->pos().y, A->pos().z,
        B->pos().x, B->pos().y, B->pos().z,
        D->pos().x, D->pos().y, D->pos().z);

    if(glm::determinant(ABC)*glm::determinant(ABD) < 0) {
      return true;
    }

    // intersection si det(CDA) et det(CDB) sont de signe opposé
    CDA = glm::mat3(
        C->pos().x, C->pos().y, C->pos().z,
        D->pos().x, D->pos().y, D->pos().z,
        A->pos().x, A->pos().y, A->pos().z);
    CDB = glm::mat3(
        C->pos().x, C->pos().y, C->pos().z,
        D->pos().x, D->pos().y, D->pos().z,
        B->pos().x, B->pos().y, B->pos().z);

    if(glm::determinant(CDA)*glm::determinant(CDB) < 0) {
      return true;
    }

    return false;
}

std::vector<Edge*> Edge::getConnectedEdges()
{
    std::vector<Edge*> edges;

    for(auto e: m_v1->edges())
    {
        if(e != this)
        {
            edges.push_back(e);
        }
    }
    for(auto e: m_v2->edges())
    {
        if(e != this)
        {
            edges.push_back(e);
        }
    }

    return edges;
}

void Edge::addFace(Face *face)
{
    m_faces.push_back(face);
}

void Edge::removeFace(Face *face)
{
    m_faces.erase(find(m_faces, face));
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
int Edge::id() const {return m_id;}
Vertex *Edge::v1() const { return m_v1; }
Vertex *Edge::v2() const { return m_v2; }
std::vector<Face*> &Edge::faces() { return m_faces; }
std::vector<Face*> &Edge::ATL() { return m_ATL; }
Edge::EdgeType Edge::type() const { return m_type; }
float Edge::cost() const { return m_cost; }
bool Edge::isLocked() const {return m_isLocked;}

// setter
void Edge::v1(Vertex *v1) { m_v1 = v1; }
void Edge::v2(Vertex *v2) { m_v2 = v2; }
void Edge::type(EdgeType type) { m_type = type; }
void Edge::isLocked(bool b) { m_isLocked = b; }

std::ostream &operator<<(std::ostream &o, Edge &e)
{
    return o << e.id() << ":[" << e.v1()->id() << "," << e.v2()->id() << "]("<<e.faces().size()<<")";
}
