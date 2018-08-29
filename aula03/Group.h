#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <vector> 
#include "Face.h"

using namespace std;

class Group {
    string name;
    string material;
    vector<Face> faces;
};

#endif // GROUP_H