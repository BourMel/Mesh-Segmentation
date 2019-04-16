#include "face.hpp"

Face::Face(Edge *edge)
{
    static int id = 0;
    
    m_id = id++;
    m_edge = edge;
}

void Face::edge(Edge *edge)
{
    m_edge = edge;
}

Edge *Face::edge() const
{
    return m_edge;   
}

int Face::id() const
{
    return m_id;
}