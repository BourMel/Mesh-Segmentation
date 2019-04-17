#include "vertex.hpp"

#include <cmath>
#include <algorithm>

#include "edge.hpp"

float cotangentWeight(Edge *he);

Vertex::Vertex(glm::vec3 pos, Edge *edge) : m_pos(pos)
{
    static int id = 0;

    m_id = id++;
    m_edge = edge;
}

std::vector<Edge*> Vertex::getIncidentEdges() {
    std::vector<Edge*> list;
    std::vector<Edge*> edgeNeighbours;

    Edge* first = edge();
    Edge* current = first;
    list.push_back(first);

    // look at the edges we can get to with the first edge
    edgeNeighbours.push_back(first->previousEdgeLeft());
    edgeNeighbours.push_back(first->previousEdgeRight());
    edgeNeighbours.push_back(first->nextEdgeLeft());
    edgeNeighbours.push_back(first->nextEdgeRight());

    for(unsigned int i=0; i<edgeNeighbours.size(); i++) {
        // if the neighbours has our vertex
        if((edgeNeighbours[i]->first()->id() == id()) || (edgeNeighbours[i]->last()->id() == id())) {
            // it means it is incident to it
            list.push_back(edgeNeighbours[i]);
        }
    }

    current = edgeNeighbours.back();
    edgeNeighbours.clear();

    while(current != first) {
        edgeNeighbours.push_back(current->previousEdgeLeft());
        edgeNeighbours.push_back(current->previousEdgeRight());
        edgeNeighbours.push_back(current->nextEdgeLeft());
        edgeNeighbours.push_back(current->nextEdgeRight());

        for(unsigned int i=0; i<edgeNeighbours.size(); i++) {
            // if we didn't registered the edge already
            if(std::find(list.begin(), list.end(), edgeNeighbours[i]) != list.end()) {
                if((edgeNeighbours[i]->first()->id() == id()) || (edgeNeighbours[i]->last()->id() == id())) {
                    list.push_back(edgeNeighbours[i]);
                }
            }
        }

        current = edgeNeighbours.back();
        edgeNeighbours.empty();
    }

    return list;
}

void Vertex::bindVertexEdges(std::vector<Edge*> toBind, Edge* edgeToDelete) {
	for(unsigned int i = 0; i < toBind.size(); i++) {
		if(glm::all(glm::equal(toBind[i]->first()->pos(), edgeToDelete->first()->pos())) ||
			glm::all(glm::equal(toBind[i]->first()->pos(), edgeToDelete->last()->pos()))) {
			toBind[i]->first(this);
		} else if(glm::all(glm::equal(toBind[i]->last()->pos(), edgeToDelete->first()->pos())) ||
			glm::all(glm::equal(toBind[i]->last()->pos(), edgeToDelete->last()->pos()))) {
			toBind[i]->last(this);
		}
	}
}

const glm::vec3& Vertex::pos() const
{
    return m_pos;
}

int Vertex::id() const
{
    return m_id;
}
Edge *Vertex::edge() const
{
    return m_edge;
}

void Vertex::pos(glm::vec3 p)
{
    m_pos = p;
}

void Vertex::edge(Edge *e)
{
    m_edge = e;
}

std::vector<Vertex *> Vertex::getNeighbours()
{
    std::vector<Vertex*> neighbours;

    // ...

    return neighbours;
}
