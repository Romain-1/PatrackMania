#include <stdint.h>
#include <iostream>
#include <string>

#include "fbxdocument.h"
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using namespace fbx;

bool findNode(std::string name, FBXNode where) {
    if(where.getName() == name) {
        where.print();
        return true;
    }
    for(FBXNode n : where.getChildren()) {
        if(findNode(name, n)) {
            return true;
        }
    }
    return false;
}
