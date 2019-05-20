#ifndef VERTEX_HPP
#define VERTEX_HPP

#include "utils.hpp"
#include <glm/vec3.hpp>
#include <vector>
#include <ostream>

class Edge;

class Vertex
{
public:
    Vertex();
    Vertex(const glm::vec3 &v);

    void addEdge(Edge *e);
    void removeEdge(Edge *e);

    // setter
    void position(const glm::vec3 &v);
    void locked(bool b);

    // getter
    const glm::vec3 &position() const;
    ID id() const;
    std::vector<Edge *> &edges();
    bool locked() const;

    friend std::ostream& operator<<(std::ostream &o, Vertex &v);

private:
    void init();

    glm::vec3 m_pos;
    ID m_id;
    std::vector<Edge *> m_edges;
    bool m_locked;
    static ID m_gid;
};

#endif
