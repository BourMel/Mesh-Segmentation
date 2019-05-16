#include "utils.hpp"
#include <cstdlib>
#include <vector>

// mergeVector()
// merge vector is missing because the compiler must be able to see 
// the implementation of the function in order to generate code 
// for all specializations.

void merge(std::vector<Edge*> &e, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;

    /* Copy data to L[] and R[] */
    std::vector<Edge*> L;
    std::vector<Edge*> R;

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
        if (L[i]->cost() <= R[j]->cost())
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
}


void mergeSort(std::vector<Edge*> &e, int l, int r)
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

Face *faceInCommon(Edge *e1, Edge *e2)
{
    for(Face *f1: e1->faces())
    {
        for(Face *f2: e2->faces())
        {
            if(f1 == f2)
            {
                return f1;
            }
        }
    }
    return nullptr;
}

float changeRate(float a, float b) {
    return b-a;
}


void exportMesh(std::string filename, std::vector<Mesh *> meshes) {
    std::ofstream file;
    glm::vec3 v1 = glm::vec3();
    glm::vec3 v2 = glm::vec3();
    std::vector<Face*> faces;
    std::vector<Edge*> edges;
    int m =  meshes.size();
    int f, e;
    int i, j, k, l;
    int c;

    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    for (i = 0; i < m; i++)
    {
        //get faces of the current mesh
        faces = meshes[i]->faces();
        f = faces.size();
        
        file << "newmtl color" << m << std::endl;
        file << "Kd " << static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ;
        file << " " << static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
        file << " " << static_cast <float> (rand()) / static_cast <float> (RAND_MAX) << std::endl;
        
        file << "g group" << m << std::endl;
        file << "usemtl color" << m << std::endl;

        for(j = 0; j < f; j++)
        {
            //get edges of the current face
            edges = faces[j]->edges();
            e = edges.size();

            for(k = 0; k< e; k++)
            {
                //add vertices to the file
                v1 = edges[i]->v1()->pos();
                v2 = edges[i]->v2()->pos();

                file << "v " << v1.x << " " << v1.y << " " << v1.z << std::endl;
                file << "v " << v2.x << " " << v2.y << " " << v2.z << std::endl;
            }

            //offset
            c = j*(2*e+2) + m*(f*(2*e+2));

            //add faces to the file
            file << "f ";
            for(l = c; l < 2*k+2 + c; i++)
                file << l << " ";

        }
        file << std::endl;
    }
    file.close();
}
