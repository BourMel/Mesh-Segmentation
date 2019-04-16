#include "edge.hpp"
#include "vertex.hpp"

Edge::Edge(Vertex *first, Vertex* last)
{
    m_first = first;
    m_last = last;
}

float Edge::cost()
{
    Vec3f v = m_last->pos(); 
    return (v-m_first->pos()).sqLength();
}

void Edge::first(Vertex *v)
{
    m_first = v;
}

void Edge::last(Vertex *v)
{
    m_last = v;
}

void Edge::faceLeft(Face *f)
{
    m_faceL = f;
}

void Edge::faceRight(Face *f)
{
    m_faceR = f;
}

void Edge::previousEdgeLeft(Edge *e)
{
    m_prevL = e;
}

void Edge::previousEdgeRight(Edge *e)
{
    m_prevR = e;
}

void Edge::nextEdgeLeft(Edge *e)
{
    m_nextL = e;
}

void Edge::nextEdgeRight(Edge *e)
{
    m_nextR = e;
}

Vertex *Edge::first() const 
{
    return m_first;
}

Vertex *Edge::last() const 
{
    return m_last;
}

Face *Edge::faceRight() const
{
    return m_faceR;
}

Face *Edge::faceLeft() const
{
    return m_faceL;
}

Edge *Edge::previousEdgeLeft()  const
{
    return m_prevL;
}

Edge *Edge::previousEdgeRight()  const
{
    return m_prevR;
}

Edge *Edge::nextEdgeLeft() const
{
    return m_nextL;
}

Edge *Edge::nextEdgeRight() const
{
    return m_nextR;
}