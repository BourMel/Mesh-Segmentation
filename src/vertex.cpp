#include "vertex.hpp"

#include <cmath>

#include "halfedge.hpp"

float cotangentWeight(HalfEdge *he);

Vertex::Vertex(Vec3f pos, HalfEdge *edge) : m_pos(pos)
{
    static int id = 0;

    m_id = id++;
    m_half_edge = edge;
}

const Vec3f& Vertex::pos() const
{
    return m_pos;
}

int Vertex::id() const
{
    return m_id;
}
HalfEdge *Vertex::halfEdge() const
{
    return m_half_edge;
}

void Vertex::pos(Vec3f p)
{
    m_pos = p;
}

void Vertex::halfEdge(HalfEdge *he)
{
    m_half_edge = he;
}

std::vector<Vertex *> Vertex::getNeighbours()
{
    std::vector<Vertex*> neighbours;

    HalfEdge *root = m_half_edge;
    HalfEdge *current = root;
    
    while(current->next_edge() != root)
    {
        current = current->next_edge();
    }
    
    
    while(root->adjacent_edge() != m_half_edge)
    {
        neighbours.push_back(current->source());
        while(current->next_edge() != root)
        {
            current = current->next_edge();
        }
        root = current->adjacent_edge();
    }
    return neighbours;
}

float cotan(float x) {return cos(x)/sin(x); }

float Vertex::voronoiArea()
{
    std::vector<Vertex *> neighbours = getNeighbours();
    float area = 0;
    Vec3f vi = m_pos;
    for(int i = 0; i < neighbours.size();i++)
    {
        Vec3f vj = neighbours[i]->pos();
        float alpha, beta;
        area += (cotan(alpha)+cotan(beta))*(vj-vi).sqLength();
    }
    return area/8.0f;
}

float Vertex::sumAngles()
{

}

float Vertex::gaussianCurvature()
{
    return ( 2*M_PI - std::fabs(sumAngles() ) / voronoiArea());
}

float Vertex::meanCurvature()
{

}

float cotangentWeight(HalfEdge *he)
{
    // v_alpha = next from he
    Vec3f v_alpha = he->next_edge()->source()->pos();
    // v_beta = next from opposed he
    Vec3f v_beta = he->adjacent_edge()->next_edge()->source()->pos();
    // v_i = source from he
    Vec3f v_i = he->source()->pos();
    // v_j = source from opposed he
    Vec3f v_j = he->adjacent_edge()->source()->pos();

    float alpha = (v_j - v_alpha).angle(v_i - v_alpha);
    float beta = (v_j - v_beta).angle(v_i - v_beta);

    return cotan(alpha) + cotan(beta);
}