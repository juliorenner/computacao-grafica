#ifndef MATERIAL_H
#define MATERIAL_H

#include <string>

using namespace std;

class Material {
private:
    string file;
    
public:
    Material(string file);
    string getFile();
};

#endif // MATERIAL_H
