#ifndef OBJREADER_H
#define OBJREADER_H

#include <string>
#include <fstream> 
#include <iostream>
#include <sstream>
#include "Mesh.h"

using namespace std;

class ObjReader {
    private:
        string filename;
        ifstream file;
        Mesh* mesh;

    public:
        ObjReader(string filename);
        Mesh* readFile();
};

#endif // MESH_H
