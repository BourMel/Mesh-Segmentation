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

bool Face::isSimple() {
    bool isSimple = true;

    // test each couple of edges
    for(unsigned int i=0; (i<m_edges.size()-1) && isSimple; i++) {
      for(unsigned int j=i+1; (j<m_edges.size()) && isSimple; j++) {

        if(m_edges[i]->intersectWith(m_edges[j])) {
          isSimple = false;
        }
      }
    }
    return isSimple;
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
