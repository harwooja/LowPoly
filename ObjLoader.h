//
//  ObjLoader.h
//  LowPoly
//
//  Created by Jake Harwood on 2014-12-13.
//  Copyright (c) 2014 Stuart Douglas. All rights reserved.
//

#ifndef __LowPoly__ObjLoader__
#define __LowPoly__ObjLoader__

#include "glm/glm/glm.hpp"
#include <vector>


class ObjLoader{
    
public:
    ObjLoader();
private:
    void loadOBJ(const char * path, std::vector < glm::vec3 > & out_vertices, std::vector < glm::vec2 > &out_uvs, std::vector < glm::vec3 > &out_normals);
    
};




#endif /* defined(__LowPoly__ObjLoader__) */

