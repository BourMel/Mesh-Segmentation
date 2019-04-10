#include "edge.hpp"

Edge::Edge(Vertex *first, Vertex* last)
{
    m_first = first;
    m_last = last;
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
    m_left = f;
}

void Edge::faceRight(Face *f)
{
    m_right = f;
}

void Edge::previousEdge1(Edge *e)
{
    m_prev1 = e;
}

void Edge::previousEdge2(Edge *e)
{
    m_prev2 = e;
}

void Edge::nextEdge1(Edge *e)
{
    m_next1 = e;
}

void Edge::nextEdge2(Edge *e)
{
    m_next2 = e;
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
    return m_right;
}

Face *Edge::faceLeft() const
{
    return m_left;
}

Edge *Edge::previousEdge1()  const
{
    return m_prev1;
}

Edge *Edge::previousEdge2()  const
{
    return m_prev2;
}

Edge *Edge::nextEdge1() const
{
    return m_next1;
}

Edge *Edge::nextEdge2() const
{
    return m_next2;
}