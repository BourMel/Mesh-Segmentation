#include <iostream>

#include "mesh.hpp"

int main(int argc, char const *argv[])
{
    Mesh mesh;

    if(argc == 2)
    {
        mesh.importOFF(argv[1]);
    }
    
    return 0;
}
