#ifndef MESH_H
#define MESH_H

#include <string>
#include <vector> 
#include "Group.h"

using namespace std;

class Mesh {
    vector<float> mappings;
    vector<float> vertex;
    vector<float> normals;
};

#endif // MESH_H