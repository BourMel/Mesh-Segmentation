#include "halfedge.hpp"

Vertex *HalfEdge::source() const 
{
    return m_source;
}

Face *HalfEdge::face() const
{
    return m_face;
}

HalfEdge *HalfEdge::previous_edge()  const
{
    return m_prev;
}

HalfEdge *HalfEdge::next_edge() const
{
    return m_next;
}

HalfEdge *HalfEdge::adjacent_edge() const
{
    return m_adj;
}

void HalfEdge::source(Vertex *v)
{
    m_source = v;
}
void HalfEdge::face(Face *f)
{
    m_face = f;
}
void HalfEdge::previous_edge(HalfEdge *he)
{
    m_prev = he;
}
void HalfEdge::next_edge(HalfEdge *he)
{
    m_next = he;
}
void HalfEdge::adjacent_edge(HalfEdge *he)
{
    m_adj = he;
}