#ifndef FACE_H
#define FACE_H

#include <vector>

using namespace std;

class Face{
    vector<int> verts;
    vector<int> norms;
    vector<int> texts;
    
    public:
        void printMessage();
};

#endif // FACE_H