#include <iostream>

#include "mesh.hpp"
#include "edge.hpp"

int main(int argc, char const *argv[])
{
    Mesh mesh;

    if(argc == 2)
    {
        mesh.importOFF(argv[1]);
        mesh.dissolveEdge(mesh.edges()[4]);
        //mesh.dissolveEdge(mesh.edges()[0]);
        //mesh.skeletonization();
        for(auto v: mesh.vertices())
        {
            std::cout << v->id() << ": (" << v->pos().x << ", " << v->pos().y << ", " << v->pos().z << ")" << std::endl;
        }
        for(auto e: mesh.edges())
        {
            std::cout << e->v1()->id() << " " << e->v2()->id() << std::endl;
        }
        for(auto f: mesh.faces())
        {
            for(auto e: f->edges())
            {
                std::cout << e->v1()->id() << " " << e->v2()->id() << " | ";
            }
            std::cout << std::endl;
        }
    }
    
    return 0;
}
