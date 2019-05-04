#include <iostream>

#include "mesh.hpp"
#include "edge.hpp"

int main(int argc, char const *argv[])
{
    Mesh mesh;

    if(argc == 2)
    {
        mesh.importOFF(argv[1]);
        //mesh.dissolveEdge(mesh.edges()[4]);
        //mesh.dissolveEdge(mesh.edges()[0]);
        mesh.skeletonization();
        mesh.exportOBJ("out.obj");
    }
    
    return 0;
}
