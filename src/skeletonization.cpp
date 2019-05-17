#define GLM_ENABLE_EXPERIMENTAL

#include "mesh.hpp"
#include "edge.hpp"
#include "utils.hpp"

#include <glm/gtx/string_cast.hpp>
#include <iostream>

void Mesh::skeletonization()
{
    bool stop = false;

    // while not all remaining edges ar not bones (or virtuals), keep going
    while(!stop)
    {
        std::size_t i = 0;

        // sort all edges from shortest to longest
        std::sort(m_edges.begin(), m_edges.end(), Edge::compEdgePtr);

        // while we are not dealing with a MESH type edge
        while(i < m_edges.size())
        {
            Edge *e = m_edges.at(i);
            if(e) // if the edge e is not null
            {
                if(e->type() == Edge::MESH)
                {
                    if(e->faces().size() > 0) // if an edge is connect to at least one face, we can collapse it
                    {
                        dissolveEdge(e);
                        //if(e->v1()->locked() == false) {m_vertices.erase(find(m_vertices,e->v1()));}
                        //if(e->v2()->locked() == false) {m_vertices.erase(find(m_vertices,e->v2()));}

                        // remove the edge
                        m_edges.erase(find(m_edges,e));
                        break;
                    }
                    else // if an edge is not connected to any face, lock it
                    {
                        //std::cout << *e <<"to bone" << std::endl;
                        e->type(Edge::BONE);
                        e->v1()->lock(true);
                        e->v2()->lock(true);
                        m_nbBV++;
                    }
                }
                else
                {
                    // is a bone or a virtual edge
                }
                debug();
                i++;
            }
            else
            {
                std::cerr << "the edge does not extist" << std::endl;
            }
        }
        if(i >= m_edges.size()-1)
        {
            stop = true;
        }
        else
        {
            //std::cout << "\r" << static_cast<float>(m_edges.size())<<std::flush;
            //std::cout << m_nbBV << std::endl;
        }
        debug();
    }
    std::cout << std::endl;

}

void Mesh::dissolveEdge(Edge *edge)
{
    Vertex *mean = edge->getMeanPosition(); // the mean position on the edge
    // add the mean to the list as new
    m_vertices.push_back(mean);

    std::cout << "trying to collapse " << *edge << " @ " << glm::to_string(mean->pos()) << std::endl;

    // all the edges connected to edge
    auto edges = edge->getConnectedEdges();
    std::cout << edges.size() << " edges connected." << std::endl;

    for(Edge* e : edges)
    {
        if(e == edge)
        {
            std::cout << "same edge" << std::endl;
        }
        else if(e->isLocked() == false)
        {
            std::cout << "edge is free: "<< *e << std::endl;
            if(e->v1() == edge->v1() || e->v1() == edge->v2()) // e.v1 is connected to edge
            {
                // on deconnect e.v1 de e
                e->v1()->edges().erase(find(e->v1()->edges(),e));
                // on connect e à mean
                e->v1(mean);
                // on ajoute e à mean
                mean->addEdge(e);
            }
            else if(e->v2() == edge->v1() || e->v2() == edge->v2()) // e.v2
            {
                e->v2()->edges().erase(find(e->v2()->edges(),e));
                e->v2(mean);
                mean->addEdge(e);
            }
            else
            {
                // erreur
                std::cerr << "edge not connected to the collapsing edge!" << std::endl;
            }

        }
        else // edge is locked so is bone
        {
            std::cout <<"hit bone" << std::endl;
            Edge *ve = new Edge;
            if(e->v1() == edge->v1() || e->v1() == edge->v2()) // e.v1 is connected to edge
            {
                ve->v1(e->v1()); // the order is not important, it is handled by the setters
                ve->v2(mean);
            }
            else if(e->v2() == edge->v1() || e->v2() == edge->v2()) // e.v2
            {
                ve->v1(e->v2());
                ve->v2(mean);
            }
            ve->v1()->lock(true);
            ve->v2()->lock(true);
            ve->type(Edge::VIRTUAL);
            m_edges.push_back(ve);
            m_nbBV++;
            //std::cout << "new virtual edge:" << *ve << std::endl;
        }
    }
    std::cout << "cleaning" << std::endl;

    // all free edges are now connected to mean
    // go through all edges of mean to detect duplicates and update ATL
    for(auto e1 = mean->edges().begin(); e1 != mean->edges().end(); e1++) // iterator
    {
        for(auto e2 = mean->edges().begin(); e2 != mean->edges().end(); e2++) // iterator
        {
            std::cout << "test: " << *(*e1) << *(*e2) << std::endl;
            // if two edges have same vertices and are not the same edge
            if(((*e1)->v1() == (*e2)->v1() && (*e1)->v2() == (*e2)->v2()) && ((*e1) != (*e2)))
            {
                Face *f = faceInCommon(*e1, *e2);
                if(f != nullptr)
                {
                    std::cout << "have a face in common" << std::endl;
                    // on enlève la face commune de e1 et e2
                    (*e1)->removeFace(f);
                    (*e2)->removeFace(f);

                    // on la met en ATL à (*e1)
                    (*e1)->addFaceATL(f);

                    std::cout << "1" << std::endl;

                    // si e2 était adjacent à une 2e face, l'ajouter à la liste de e1
                    if((*e2)->faces().at(0) != f)
                    {
                        (*e1)->addFace((*e2)->faces().at(0));
                    }

                    std::cout << "2" << std::endl;

                    // si e2 avait une ATL non-vide
                    if((*e2)->ATL().size() > 0)
                    {
                        for(Face *f : (*e2)->ATL())
                        {
                            (*e1)->addFaceATL(f);
                        }
                    }

                    std::cout << "3" << std::endl;

                    //on supprime e2
                    //(*e2)->v1()->removeEdge(*e2);
                    //(*e2)->v2()->removeEdge(*e2);

                    // on supprime e2 de la liste des edges de l'extremité qui n'est pas mean
                    if((*e2)->v1() != mean)
                    {
                        std::cout << *((*e2)->v1()) << std::endl;
                        (*e2)->v1()->removeEdge(*e2);
                    }
                    else
                    {
                        std::cout << "b" << std::endl;
                        (*e2)->v2()->removeEdge(*e2);
                    }

                    std::cout << "4" << std::endl;
                }

                // si la face commune était la seule face adjacente aux deux edges, c'est mtn une BONE
                if((*e1)->faces().size() <= 0)
                {
                    (*e1)->v1()->lock(true);
                    (*e1)->v2()->lock(true);
                    (*e1)->type(Edge::BONE);
                    m_nbBV++;
                }

                // on supprime le doublon de la liste globale
                std::cout << "remove doubled edge" << std::endl;
                m_edges.erase(find(m_edges,*e2));
                std::cout << "remove from mean" << std::endl;
                // on supprime le doublon de la liste de mean
                mean->edges().erase(e2);
                break; // cannot have another match
            }
        }
    }
}
