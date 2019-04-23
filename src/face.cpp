#include "face.hpp"
#include "edge.hpp"

Face::Face()
{
    static int id = 0;
    
    m_id = id++;
}

void Face::addEdge(Edge *e)
{
    m_edges.push_back(e);
}

std::vector<Edge*> &Face::edges()
{
    return m_edges;
}

int Face::id() const
{
    return m_id;
}