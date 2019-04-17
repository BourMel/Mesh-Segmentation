#define GLM_ENABLE_EXPERIMENTAL

#include "edge.hpp"
#include "vertex.hpp"

#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>

Edge::Edge(Vertex *first, Vertex* last)
{
    m_first = first;
    m_last = last;
}

float Edge::cost()
{
    return glm::length2(m_last->pos()-m_first->pos());
}


void Edge::merge(Edge* e, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* Copy data to L[] and R[] */
    Edge * L = (Edge*)malloc(n1*sizeof(Edge));
    Edge * R = (Edge*)malloc(n2*sizeof(Edge));

    for (i = 0; i < n1; i++)
        L[i] = e[l + i];

    for (i = 0; i < n2; i++)
        R[i] = e[m + 1 + i];

    // Merge tmp arrays in e[l..r]
    i = 0;
    j = 0;
    k = l;
    while (i < n1 && j < n2)
    {
        //compare the cost
        if (L[i].cost() <= R[j].cost())
        {
            e[k] = L[i];
            i++;
        }
        else
        {
            e[k] = R[j];
            j++;
        }
        k++;
    }

    // Copy the remaining elements of L[] and R[], if there are any
    while (i < n1)
    {
        e[k] = L[i];
        i++;
        k++;
    }
    while (j < n2)
    {
        e[k] = R[j];
        j++;
        k++;
    }

    free(L);
    free(R);
}


void Edge::mergeSort(Edge * e, int l, int r)
{
    if (l < r)
    {
        // Sort the two half of the array
        int m = (l+r)/2;
        mergeSort(e, l, m);
        mergeSort(e, m+1, r);

        // Merge the two arrays
        merge(e, l, m, r);
    }
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
