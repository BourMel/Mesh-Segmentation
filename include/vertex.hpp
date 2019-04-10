#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

#include "vec3.hpp"

class HalfEdge;

class Vertex
{
  public:
    Vertex(Vec3f pos, HalfEdge *edge = nullptr);

    // getter
    const Vec3f& pos() const;
    int id() const;
    HalfEdge * halfEdge() const;
    
    //setter
    void halfEdge(HalfEdge *he);
    void pos(Vec3f p);

    // functions
    float voronoiArea();
    float sumAngles();
    float gaussianCurvature();
    float meanCurvature();

  private:
    std::vector<Vertex *> getNeighbours();
    int m_id;
    Vec3f m_pos;
    HalfEdge * m_half_edge;
};

#endif