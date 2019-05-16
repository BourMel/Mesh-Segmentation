#define GLM_ENABLE_EXPERIMENTAL

#include "edge.hpp"
#include "vertex.hpp"
#include "face.hpp"
#include "utils.hpp"

#include <ostream>
#include <algorithm>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

void Edge::init()
{
    static int i = 0;
    m_id = i;
    i++;
}

Edge::Edge()
{
    init();
    m_v1 = nullptr;
    m_v2 = nullptr;
}

Edge::Edge(Vertex *first, Vertex *last)
{
    init();
    m_v1 = first;
    m_v2 = last;
    m_cost = glm::length2(m_v2->pos() - m_v1->pos());
    m_isLocked = false;
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

void Edge::computeArea() {
    float totalArea = 0.0f;
    float currentArea = 0.0f;
    // lengths of edges
    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;

    for(unsigned int i=0; i<m_ATL.size(); i++) {
      currentArea = 0.0f;

      // we get the length of each edge of the face (triangle)
      a = std::sqrt(m_ATL[i]->edges()[0]->cost());
      b = std::sqrt(m_ATL[i]->edges()[1]->cost());
      c = std::sqrt(m_ATL[i]->edges()[2]->cost());

      // Heron's formula
      currentArea = (a + b + c)*(-a + b + c)*(a - b + c)*(a + b -c);
      currentArea = std::sqrt(currentArea)/4;

      totalArea += currentArea;
    }
    m_area = totalArea;
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

void Edge::removeFaceATL(Face *face) {
    int index;
    std::vector<Face*>::iterator faceInATL;

    faceInATL = std::find(m_ATL.begin(), m_ATL.end(), face);
    index = std::distance(m_ATL.begin(), faceInATL);
    m_ATL.erase(m_ATL.begin() + index);
}

void Edge::computeCost()
{
    if(m_v1 != nullptr && m_v2 != nullptr)
    {
        m_cost = glm::length2(m_v2->pos()-m_v1->pos());
    }
    else
    {
        m_cost = -1.0f;
    }
}

glm::vec3 Edge::getNormal() {
	return glm::normalize(m_v2->pos() - m_v1->pos());
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
float Edge::area() const{return m_area;}

// setter
void Edge::v1(Vertex *v1)
{
    if(v1->id() <= m_v2->id())
    {
        m_v1 = v1;
    }
    else
    {
        m_v1 = m_v2;
        m_v2 = v1;
    }
    computeCost();
}

void Edge::v2(Vertex *v2)
{
    if(v2->id() >= m_v1->id())
    {
        m_v2 = v2;
    }
    else
    {
        m_v2 = m_v1;
        m_v1 = v2;
    }
    computeCost();
}
void Edge::type(EdgeType type) { m_type = type; }
void Edge::isLocked(bool b) { m_isLocked = b; }

std::ostream &operator<<(std::ostream &o, Edge &e)
{
    return o << e.id() << ":[" << e.v1()->id() << "," << e.v2()->id() << "]("<<e.faces().size()<<")"<<"("<<(e.type() == Edge::MESH ? "M)" : (e.type()==Edge::VIRTUAL ? "V)":"B)"));
}
