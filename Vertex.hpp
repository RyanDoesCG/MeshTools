//
//  Vertex.hpp
//  ModelValidator
//
//  Created by macbook on 18/06/2018.
//  Copyright © 2018 Hobby. All rights reserved.
//

#ifndef Vertex_h
#define Vertex_h

#include <glm/glm.hpp>
#include <glm/gtx/hash.hpp>

struct Vertex
    {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec3 color;
    glm::vec2 uvs;
    uint32_t  id;
    
    
    bool operator==(const Vertex& other) const {
        return position == other.position && color == other.color && normal == other.normal && uvs == other.uvs;
    }
    };

namespace std {
    template<> struct hash<Vertex> {
        size_t operator()(Vertex const& vertex) const {
            return ((hash<glm::vec3>()(vertex.position) ^ (hash<glm::vec3>()(vertex.color) << 1)) >> 1) ^ (hash<glm::vec2>()(vertex.uvs) << 1);
        }
    };
}


#endif /* Vertex_h */
