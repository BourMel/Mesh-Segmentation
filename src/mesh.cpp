#define GLM_ENABLE_EXPERIMENTAL

#include "mesh.hpp"
#include "edge.hpp"
#include "face.hpp"
#include "utils.hpp"
#include "output.hpp"
#include "plane.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <glm/gtx/string_cast.hpp>

Mesh::Mesh()
{
    m_nbBV = 0;
}

void Mesh::skeletonization()
{
    bool stop = false;
    while(!stop)
    {
        std::size_t i = 0;

        //std::cout <<"sorting"<< "..." << std::endl;
        std::sort(m_edges.begin(), m_edges.end(), Edge::compEdgePtr);

        while(i < m_edges.size())
        {
            Edge *e = m_edges[i];
            if(e->type() == Edge::MESH)
            {
                if(e->faces().size() > 0) // if an edge is connect to at least one face
                {
                    dissolveEdge(e);
                    if(e->v1()->locked() == false) {m_vertices.erase(find(m_vertices,e->v1()));}
                    if(e->v2()->locked() == false) {m_vertices.erase(find(m_vertices,e->v2()));}
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
                //std::cout << "is bone, skip" << std::endl;
            }

            i++;
        }
        if(i >= m_edges.size()-1)
        {
            stop = true;
        }
        else
        {
            std::cout << "\r" << static_cast<float>(m_edges.size())<<std::flush;
            //std::cout << m_nbBV << std::endl;
        }
    }
    std::cout << std::endl;
    debug();
}

void Mesh::segmentation() {
  Plane *sweepPlane;
  std::vector<Edge*> candidates;
  Mesh *crossSection = new Mesh();

  // some operations made only after a given number of iterations
  int countIterations = 0;
  // 5 last values of F (area or perimeter of the cross section)
  float F1 = 0.0f;
  float F2 = 0.0f;
  float F3 = 0.0f;
  float F4 = 0.0f;
  float F5 = 0.0f;

  // dérivations
  float d1 = 0.0f;
  float d2 = 0.0f;
  float previousD3 = 0.0f;
  float currentD3 = 0.0f;
  float nextD3 = 0.0f;

  // H : nb of simple polygons in the cross section
  int currentH = 0;
  int previousH = 0;
  float perimeter;

  int topology;

  std::vector<Mesh *> components;
  Mesh* currentComponent;

  for(unsigned int i=0; i<m_bones.size(); i++) {
    m_bones.at(i)->computeArea();
  }

  // order bones
  std::sort(m_bones.begin(), m_bones.end(), Edge::compEdgePtrArea);

  for(unsigned int i=0; i<m_bones.size(); i++){
    // get the point V1 and the vector (V1,V2)
    sweepPlane = new Plane(m_bones.at(i)->v1()->pos(), m_bones.at(i)->getNormal());

    // TODO : search for less candidates to the intersection (= ATL of the bone ?)
    candidates = m_edges;

    // intersections between sweepPlane and original mesh : edges
    for(unsigned int j=0; j<candidates.size(); j++) {
        int position1 = sweepPlane->relativePosition(candidates.at(j)->v1()->pos());
        int position2 = sweepPlane->relativePosition(candidates.at(j)->v2()->pos());

        if((position1 < 0 && position2 > 0) || (position1 > 0 && position2 < 0)) {
            crossSection->addEdge(candidates.at(j));
        }
    }
    // intersections : faces
    for(unsigned int j=0; j<m_faces.size(); j++) {
        for(unsigned int k=0; m_faces.at(j)->edges().size(); k++) {
            // if one of the edges of the face is intersecting the cross section
            if(std::find(crossSection->edges().begin(), crossSection->edges().end(),
                m_faces.at(i)->edges().at(j)) != crossSection->edges().end()) {
                
                // the face is intersecting the cross section
                crossSection->addFace(m_faces.at(j));
            }
        }
    }

    // get the polygons of the cross section = faces construction
    crossSection->constructFaces();

    // perimeter : we suppose the polygons are simple
    for(unsigned int i=0; i<crossSection->faces().size(); i++) {
        for(unsigned int j=0; j<crossSection->faces().at(i)->edges().size(); j++) {
            perimeter += std::sqrt(crossSection->faces().at(i)->edges().at(j)->cost());
        }
    }

    F1 = F2;
    F2 = F3;
    F3 = F4;
    F4 = F5;
    F5 = perimeter;

    previousD3 = currentD3;
    currentD3 = nextD3;

    if(countIterations >= 2) {
      // compute the first derivative (with 2 values)
      d1 = changeRate(F5, F4);
    }

    if(countIterations >= 3) {
      // compute the second derivative (change rate of 2 first derivatives = 3 values)
      d2 = changeRate(d1, changeRate(F4, F3));
    }

    if(countIterations >= 5) {
      // compute the third derivative (change rate of 2 second derivatives = 5 values)
      
      nextD3 = changeRate(
        d2,
        changeRate(
          changeRate(F3, F2),
          changeRate(F2, F1)
        )
      );
    }

    previousH = currentH;
    currentH = 0;

    // get H (number of simple polygons in the cross section)
    for(unsigned int j=0; j<crossSection->faces().size(); j++) {
      if(crossSection->faces().at(j)->isSimple()) {
        currentH++;
      }
    }

    if(currentH != previousH) {
      topology = 0;
    }
    else {
      topology = 1;
    }

    if(countIterations >= 7) {
      if(topology == 0 || (currentD3 == 0 && (previousD3*nextD3 < 0))) {
        // get the previous triangles and count them as a component
        
        // we look for the triangles belonging to the bones (ATL)
        for(unsigned int j=0; j<m_bones[i]->ATL().size(); j++) {
          // we look at its edges
          for(unsigned int k=0; k<m_bones[i]->ATL().at(j)->edges().size(); k++) {

            // if one vertex is under the sweep plane, it is part of our new component
            if(sweepPlane->relativePosition(m_bones.at(i)->ATL().at(j)->edges().at(k)->v1()->pos()) < 0) {
              currentComponent->addFace(m_bones.at(i)->ATL().at(j));
              // remove from ATL so it won't be considered in next components
              m_bones.at(i)->removeFaceATL(m_bones.at(i)->ATL().at(j));
              break;
            }
          }

        }


        components.push_back(currentComponent);
      }
    }

    sweepPlane->nextPoint();
  }


  delete sweepPlane;
  delete crossSection;
}

// edge: the edge to collapse
void Mesh::dissolveEdge(Edge *edge)
{
    Vertex *mean = edge->getMeanPosition(); // the mean position on the edge
    m_vertices.push_back(mean);
    //std::cout << "trying to collapse " << *edge << " @ " << glm::to_string(mean->pos()) << std::endl;

    auto edges = edge->getConnectedEdges(); //   all the edges connected
    //std::cout << edges.size() << " edges connected." << std::endl;

    for(Edge* e : edges)
    {
        if(e->isLocked() == false)
        {
            if(e->v1() == edge->v1() || e->v1() == edge->v2()) // e.v1 is connected to edge
            {
                e->v1()->edges().erase(find(e->v1()->edges(),e));
                e->v1(mean);
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
            Edge *ve = new Edge;
            if(e->v1() == edge->v1() || e->v1() == edge->v2()) // e.v1 is connected to edge
            {
                ve->v1(e->v1());
                ve->v2(mean);
            }
            else if(e->v2() == edge->v1() || e->v2() == edge->v2()) // e.v2
            {
                ve->v1(mean);
                ve->v2(e->v2());
            }
            ve->v1()->lock(true);
            ve->v2()->lock(true);
            ve->type(Edge::VIRTUAL);
            m_edges.push_back(ve);
            m_nbBV++;
            //std::cout << "new virtual edge:" << *ve << std::endl;
        }
    }

    // all free edges are now connected to mean
    // go through all edges of mean to detect duplicates and update ATL
    for(auto e1 = mean->edges().begin(); e1 != mean->edges().end(); e1++) // iterator
    {
        for(auto e2 = mean->edges().begin(); e2 != mean->edges().end(); e2++) // iterator
        {
            // if two edges have same vertices and are not the same edge
            if((((*e1)->v1() == (*e2)->v1() && (*e1)->v2() == (*e2)->v2()) || ((*e1)->v1() == (*e2)->v2() && (*e1)->v2() == (*e2)->v1())) && ((*e1) !=(*e2)))
            {
                Face *f = faceInCommon(*e1, *e2);
                if(f != nullptr)
                {
                    // on enlève la face commune de e1 et e2
                    (*e1)->removeFace(f);
                    (*e2)->removeFace(f);
                    // on la met en ATL à (*e1)
                    (*e1)->addFaceATL(f);
                    // si e2 était adjacent à une 2e face, l'ajouter à la liste de e1
                    if((*e2)->faces()[0] != f)
                    {
                        (*e1)->addFace((*e2)->faces()[0]);
                    }

                    // on supprime e2 de la liste des edges de l'extremité qui n'est pas mean
                    if((*e2)->v1() != mean)
                    {
                        (*e2)->v1()->removeEdge(*e2);
                    }
                    else
                    {
                        (*e2)->v2()->removeEdge(*e2);
                    }

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
                m_edges.erase(find(m_edges,*e2));
                // on supprime le doublon de la liste de mean
                mean->edges().erase(e2);
                break; // cannot have another match
            }
        }
    }
}

void Mesh::constructFaces() {
    std::vector<Edge*> belongsToFace;
    Edge* e1;
    Edge* e2;
    Vertex* sharedVertex = NULL;
    bool faceOver; // not working
    int count = m_vertices.size(); // avoid infinite loop (TODO)

    for(unsigned int i=0; i<m_edges.size() - 1; i++) {
      // ignore edges that already belongs to a face
      if(std::find(belongsToFace.begin(), belongsToFace.end(), m_edges[i]) != belongsToFace.end()) {
        continue;
      }

      for(unsigned int j=i+1; j<m_edges.size(); j++) {
        if(std::find(belongsToFace.begin(), belongsToFace.end(), m_edges[j]) != belongsToFace.end()) {
          continue;
        }

        e1 = m_edges[i];
        e2 = m_edges[j];
        sharedVertex = NULL;

        // they share a vertex
        if(e1->v1() == e2->v1() || e1->v1() == e2->v2()) {
          sharedVertex = e1->v1();
        }
        if(e1->v2() == e2->v1() || e1->v2() == e2->v2()) {
          sharedVertex = e1->v2();
        }

        if(sharedVertex != NULL) {
          Face* face = new Face();
          face->edges().push_back(e1);
          belongsToFace.push_back(e1);

          Vertex* currentVertex = sharedVertex;

          // get to the next vertex
          for(unsigned int k=0; k<m_edges.size(); k++) {
            if(std::find(belongsToFace.begin(), belongsToFace.end(), m_edges[k]) != belongsToFace.end()) {
              continue;
            }
            bool foundNext = false;

            if(m_edges[k]->v1() == currentVertex) {
              currentVertex = m_edges[k]->v2();
              foundNext = true;
            } else if(m_edges[k]->v2() == currentVertex) {
              currentVertex = m_edges[k]->v1();
              foundNext = true;
            }

            if(foundNext) {
              // add the edge to the face
              face->edges().push_back(m_edges[k]);
              belongsToFace.push_back(m_edges[k]);
              break;
            }
          }

          faceOver = false;

          // we look at the entire face
          while((currentVertex != sharedVertex) && (!faceOver) && count > 0) {
            count--;

            // get to the next vertex
            for(unsigned int k=0; k<m_edges.size(); k++) {
              if(std::find(belongsToFace.begin(), belongsToFace.end(), m_edges[k]) != belongsToFace.end()) {
                continue;
              }
              bool foundNext = false;

              if(m_edges[k]->v1() == currentVertex) {
                currentVertex = m_edges[k]->v2();
                foundNext = true;
              } else if(m_edges[k]->v2() == currentVertex) {
                currentVertex = m_edges[k]->v1();
                foundNext = true;
              }

              if(foundNext) {
                // add the edge to the face
                face->edges().push_back(m_edges[k]);
                belongsToFace.push_back(m_edges[k]);
                break;
              }

              // we didn't find the next vertex
              if(k == m_edges.size()-1) {
                faceOver = true;
              }
            }
          }

          m_faces.push_back(face);
        }
      }
    }
}

void Mesh::importOFF(std::string filename)
{

    std::ifstream file;   // input file
    std::string line;     // current line
    std::stringstream ss; // stream to read content of line

    int numLine = 0;       // number of the current line
    int numVert, numFaces; // number of vertices/faces

    float x, y, z;

    // open file
    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
    else
    {
        std::cout << "filename: " << filename << std::endl;
    }

    // detect OFF tag
    std::getline(file, line);
    numLine++;
    ss.str(line);
    if (!(ss.str() == "OFF"))
    {
        std::cerr << "file token is not OFF" << std::endl;
        return;
    }

    std::getline(file, line);
    numLine++;

    // get the number of vertices end faces indicated in the off file
    ss.str(line);
    ss >> numVert >> numFaces;

    // read vertices info
    for (int i = 0; i < numVert; i++)
    {
        std::getline(file, line);
        numLine++;

        std::istringstream iss(line);

        // tries to read at least 3 consecutive numbers as float
        if (iss >> x >> y >> z)
        {
            // add the new vertex to the mesh
            m_vertices.push_back(new Vertex({x, y, z}));
        }
        else
        {
            std::cout << "Wrong vertex format (at " << numLine << ")" << std::endl;
            return;
        }
    }

    // read face composition
    for (int i = 0; i < numFaces; i++)
    {
        std::getline(file, line);
        numLine++;

        uint nbv, id;
        std::istringstream iss(line);
        std::vector<uint> indices;

        // read number of vertices in face
        if (!(iss >> nbv))
        {
            std::cerr << "Unable to read vertex count of face! (at " << numLine << ")" << std::endl;
            return;
        }

        // read indices of vertices connected to the face
        // expects to read at least nbv indices
        for (uint i = 0; i < nbv; i++)
        {
            if (iss >> id)
            {
                indices.push_back(id);
            }
            else
            {
                std::cerr << "Unable to read vertex coordinate! (at " << numLine << ")" << std::endl;
                return;
            }
        }

        Face *f = new Face;

        // sort indices list in ascending order
        bubbleSort(indices);

        // gen edge couple [0,1] [1,2] [0,2]
        // we only want edges with v1.id < v2.id
        std::vector<EdgeCouple> edgeCouples;
        edgeCouples.push_back(EdgeCouple(m_vertices[indices[0]], m_vertices[indices[1]]));
        edgeCouples.push_back(EdgeCouple(m_vertices[indices[1]], m_vertices[indices[2]]));
        edgeCouples.push_back(EdgeCouple(m_vertices[indices[0]], m_vertices[indices[2]]));

        // for every edge couples
        for (auto ec : edgeCouples)
        {
            // try to find if the edge already exists
            Edge *e = findEdge(ec.v1, ec.v2);
            if (!e) // if it does't exists, create the edge and insert it in the map
            {
                e = new Edge(ec.v1, ec.v2);
                ec.v1->addEdge(e);
                ec.v2->addEdge(e);

                m_edge_map.insert({{e->v1()->id(), e->v2()->id()}, e});
            }

            // add the current face to the edge
            e->addFace(f);
            // add the edge to the face
            f->addEdge(e);
        }
        // add the face to the mesh
        m_faces.push_back(f);
    }
    // end
    std::cout << "Found " << m_vertices.size() << "vertices and " << m_faces.size() << " faces." << std::endl;

    // put all element of map into a vector
    for(auto it = m_edge_map.begin(); it != m_edge_map.end() ; ++it)
        m_edges.push_back(it->second);
}

void Mesh::importOBJ(std::string filename)
{
    std::ifstream file;   // input file
    std::string line;     // current line

    int numLine = 0;       // number of the current line

    // open file
    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }
    else
    {
        std::cout << "filename: " << filename << std::endl;
    }

    while(std::getline(file, line))
    {
        numLine++;

        std::string type;

        // lecture de la ligne
        std::istringstream iss(line);

        // lecture du flag
        iss >> type;

        if(type == "v") // vertex declaration
        {
            float x, y, z;
            // tries to read at least 3 consecutive numbers as float
            if (iss >> x >> y >> z)
            {
                m_vertices.push_back(new Vertex({x, y, z}));
            }
            else
            {
                std::cout << "Wrong vertex format (at " << numLine << ")" << std::endl;
                return;
            }
        }
        else if(type == "f") // face declaration
        {
            std::vector<uint> indices;
            for (int i = 0; i < 3; i++)
            {
                uint t;
                if(iss >> t)
                {
                    indices.push_back(t-1);
                }
                else
                {
                    std::cerr << "error" << std::endl;
                    return;
                }
            }
            Face *f = new Face;

            // sort indices list in ascending order
            bubbleSort(indices);

            // gen edge couple [0,1] [1,2] [0,2]
            // we only want edges with v1.id < v2.id
            std::vector<EdgeCouple> edgeCouples;
            edgeCouples.push_back(EdgeCouple(m_vertices[indices[0]], m_vertices[indices[1]]));
            edgeCouples.push_back(EdgeCouple(m_vertices[indices[1]], m_vertices[indices[2]]));
            edgeCouples.push_back(EdgeCouple(m_vertices[indices[0]], m_vertices[indices[2]]));

            // for every edge couples
            for (auto ec : edgeCouples)
            {
                // try to find if the edge already exists
                Edge *e = findEdge(ec.v1, ec.v2);
                if (!e) // if it does't exists, create the edge and insert it in the map
                {
                    e = new Edge(ec.v1, ec.v2);
                    ec.v1->addEdge(e);
                    ec.v2->addEdge(e);

                    m_edge_map.insert({{e->v1()->id(), e->v2()->id()}, e});
                }

                // add the current face to the edge
                e->addFace(f);
                // add the edge to the face
                f->addEdge(e);
            }
            // add the face to the mesh
            m_faces.push_back(f);
        }
        else if(type == "o") // new object (with name) :: UNSUPPORTED
        {
            // pass
        }
        else if(type == "#") // comment
        {
            // pass
        }
        else if(type == "s")
        {
            // pass
        }
        else
        {
            std::cerr << "OBJ flag not handled:\"" << type << "\" (at "<<numLine<<")"<< std::endl;
        }
    }
    // put all element of map into a vector
    for(auto it = m_edge_map.begin(); it != m_edge_map.end() ; ++it)
        m_edges.push_back(it->second);
}

void Mesh::exportOBJ(std::string filename)
{
    std::ofstream file;   // output file

    //open file
    file.open(filename);
    if (!file.is_open())
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        return;
    }

    file << "o objet" << std::endl;
    unsigned int i = 1;
    for(Vertex *v : m_vertices)
    {
        file << "v " << v->pos().x << " " << v->pos().y << " " << v->pos().z << std::endl;
        v->exportId(i);
        i++;
    }

    for(Edge *e : m_edges)
    {
        // virtual edges
        if(e->type() == Edge::VIRTUAL)
        {
            //file << "g virtual" << std::endl;
            //file << "usemtl blue" << std::endl;
        }
        // bones
        else if(e->type() == Edge::BONE)
        {
            //file << "g bone" << std::endl;
            //file << "usemtl red" << std::endl;
        }

        // line between the vertices
        file << "l " << e->v1()->exportId() << " " << e->v2()->exportId() << std::endl;
    }

    file.close();
}

void Mesh::exportMesh(std::string filename, std::vector<Mesh *> meshes) {
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

        file << "g " << m << std::endl;

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



Edge *Mesh::findEdge(Vertex *a, Vertex *b)
{
    std::map<std::pair<int, int>, Edge *>::iterator it = m_edge_map.find({a->id(), b->id()});
    if (it == m_edge_map.end())
        return nullptr;
    else
        return it->second;
}

std::vector<Face*> Mesh::cleanDouble(std::vector<Edge*> l1, std::vector<Edge*> l2) {
    std::vector<Face*> ATL;
    Face* shared;
    bool foundSharedFace = false;

    int faceIndex;
    std::vector<Face*>::iterator faceInList;

    for(unsigned int i=0; i<l1.size(); i++) {
        for(unsigned int j=0; j<l2.size(); j++) {

            // if 2 edges have the same vertices
            if(
                ((l1[i]->v1() == l2[j]->v1())
                && (l1[i]->v2() == l2[j]->v2()))
            ||
                ((l1[i]->v1() == l2[j]->v2())
                && (l1[i]->v2() == l2[j]->v1()))
            ) {

                // we find the face they shared when they were not flattened
                for(unsigned int k=0; k<l1[i]->faces().size() && !foundSharedFace; k++) {
                    for(unsigned int l=0; l<l2[j]->faces().size() && !foundSharedFace; l++) {
                        if(l1[i]->faces()[k] == l2[j]->faces()[l]) {
                            shared = l1[i]->faces()[k];

                            // remove the flattened face from l1 and l2 lists
                            faceInList = std::find(l1[i]->faces().begin(), l1[i]->faces().end(), shared);
                            faceIndex = std::distance(l1[i]->faces().begin(), faceInList);
                            l1[i]->faces().erase(l1[i]->faces().begin() + faceIndex);

                            faceInList = std::find(l2[j]->faces().begin(), l2[j]->faces().end(), shared);
                            faceIndex = std::distance(l2[j]->faces().begin(), faceInList);
                            l2[j]->faces().erase(l2[j]->faces().begin() + faceIndex);

                            // quit the 2 loops
                            foundSharedFace = true;
                        }
                    }
                }

                // we delete the last edge of the shared (and flattened) face (not l1, not l2)
                for(unsigned int k=0; k<shared->edges().size(); k++) {
                    if((shared->edges()[k] != l1[i] && (shared->edges()[k] != l2[j]))) {
                        deleteEdge(shared->edges()[k]);
                        delete shared->edges()[k];
                        break;
                    }
                }

                // and define l1's new construction (linked faces)
                l1[i]->faces().insert(l1[i]->faces().end(), l2[j]->faces().begin(), l2[j]->faces().end());

                delete l2[j];
                ATL.push_back(shared);
                deleteFace(shared);
            }
        }
    }
    return ATL;
}

void Mesh::deleteEdge(Edge* edge) {
    int index;
    std::vector<Edge*>::iterator edgeInMesh;

    edgeInMesh = std::find(m_edges.begin(), m_edges.end(), edge);
    index = std::distance(m_edges.begin(), edgeInMesh);
    m_edges.erase(m_edges.begin() + index);
}

void Mesh::deleteFace(Face* face) {
    int index;
    std::vector<Face*>::iterator faceInMesh;

    faceInMesh = std::find(m_faces.begin(), m_faces.end(), face);
    index = std::distance(m_faces.begin(), faceInMesh);
    m_faces.erase(m_faces.begin() + index);
}

void Mesh::addEdge(Edge* e) {
  m_edges.push_back(e);
}

void Mesh::addFace(Face* f) {
  m_faces.push_back(f);
}

void Mesh::debug() const
{
    for(auto v: m_vertices)
    {
        std::cout << *v << std::endl;
    }
    for(auto e: m_edges)
    {
        std::cout << *e << std::endl;
    }
    for(auto f: m_faces)
    {
        std::cout << *f << std::endl;
    }
}
