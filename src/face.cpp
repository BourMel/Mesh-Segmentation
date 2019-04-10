#include "face.hpp"

Face::Face(HalfEdge *edge)
{
    static int id = 0;
    
    m_id = id++;
    m_edge = edge;
}