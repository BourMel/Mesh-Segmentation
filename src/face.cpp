#include "face.hpp"
#include "edge.hpp"

#include <sstream>

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

std::ostream &operator<<(std::ostream &o, Face &f)
{
    std::stringstream ss;
    ss << f.id() << " ";
    for(auto e : f.edges())
    {
        ss << *e;
    }
    return o << ss.str();
}