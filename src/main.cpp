#include <iostream>

#include "mesh.hpp"
#include "edge.hpp"

std::string getFileExt(const std::string& s) {

   size_t i = s.rfind('.', s.length());
   if (i != std::string::npos) {
      return(s.substr(i+1, s.length() - i));
   }

   return("");
}

int main(int argc, char const *argv[])
{
    Mesh mesh;

    if(argc == 2)
    {
        std::string ext = getFileExt(argv[1]);
        std::cout << ext << std::endl;
        if(ext == "obj" || ext == "OBJ")
        {
            mesh.importOBJ(argv[1]);
        }
        else if(ext == "off" || ext == "OFF")
        {
            mesh.importOFF(argv[1]);
        }
        else
        {
            std::cout << "File extension unknown!" << std::endl;
            return 0;
        }
        mesh.skeletonization();
        mesh.exportOBJ("out.obj");
    }
    
    return 0;
}
