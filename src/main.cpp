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
    bool fail = false;

    if(argc == 2)
    {
        std::string ext = getFileExt(argv[1]);
        std::cout << ext << std::endl;
        if(ext == "obj" || ext == "OBJ")
        {
            if(!mesh.importOBJ(argv[1]))
                fail = true;
        }
        else if(ext == "off" || ext == "OFF")
        {
            if(!mesh.importOFF(argv[1]))
                fail = true;
        }
        else
        {
            std::cout << "File extension unknown!" << std::endl;
            return 0;
        }

        if(fail)
        {
            return 0;
        }
        else
        {
        mesh.skeletonization();
        mesh.debug();
        //mesh.segmentation();
        // mesh.exportOBJ("out.obj");
        }
    }

    return 0;
}
