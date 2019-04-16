#ifndef VERTEX_H
#define VERTEX_H

#include <vector>

//#include "vec3.hpp"
#include <glm/vec3.hpp>

class Edge;

class Vertex
{
  public:
    Vertex(glm::vec3 pos, Edge *edge = nullptr);

    // TODO
    // getAdjFaces()

    // getter
    const glm::vec3& pos() const;
    int id() const;
    Edge * edge() const;
    
    //setter
    void edge(Edge *e);
    void pos(glm::vec3 p);

  private:
    std::vector<Vertex *> getNeighbours(); // to public ?
    int m_id;
    glm::vec3 m_pos;
    Edge * m_edge;
};

#endif