//
//  Repair.hpp
//  ModelValidator
//
//  Created by macbook on 18/06/2018.
//  Copyright © 2018 Hobby. All rights reserved.
//

#ifndef Repair_hpp
#define Repair_hpp

#include "Vertex.hpp"
#include <vector>
#include <cmath>
#include <random>

/**
 *
 */
void shrinkUVs (std::vector<Vertex>& vertices, float f)
    { // shrinkUVs
    for (uint32_t v = 0; v < vertices.size(); ++v)
        { // for each vertex
        vertices[v].uvs.s *= 1.0f - f;
        vertices[v].uvs.t *= 1.0f - f;
        } // for each vertex
    } // shrinkUVs

/**
 *
 */
void repairInvalidUVs (std::vector<Vertex>& vertices)
    { // repairUVs
    // ensure all uv coordinates are positive
    for (uint32_t v = 0; v < vertices.size(); ++v)
        { // for each vertex
        vertices[v].uvs.s = fabs(vertices[v].uvs.s);
        vertices[v].uvs.t = fabs(vertices[v].uvs.t);
        } // for each vertex
    // ensure all uv coordinates are restricted to
    // the range [ 0.0 ... 1.0 ]
    for (uint32_t v = 0; v < vertices.size(); ++v)
        { // for each vertex
        if (vertices[v].uvs.s > 1.0f)
            {
            shrinkUVs(vertices, vertices[v].uvs.s - 1.0f);
            v = 0;
            }
        if (vertices[v].uvs.t > 1.0f)
            {
            shrinkUVs(vertices, vertices[v].uvs.t - 1.0f);
            v = 0;
            }
        } // for each vertex
    } // repairUVs

/**
 *
 */
void repairDuplicateUVs (std::vector<Vertex>& vertices)
    { // repairDuplicates
    std::uniform_real_distribution<float> dist (-0.001, 0.001);
    std::default_random_engine rng (time(0));
    for (uint32_t i = 0; i < vertices.size(); ++i)
        { // for each vertex
        // ensure the texture coordinates are unique
        for (uint32_t j = i; j < vertices.size(); ++j)
            if (i != j) if (vertices[i].uvs.s == vertices[j].uvs.s &&
                vertices[i].uvs.t == vertices[j].uvs.t)
                { // duplicate found
                vertices[i].uvs += glm::vec2(dist(rng), dist(rng));
                vertices[j].uvs += glm::vec2(dist(rng), dist(rng));
                } // duplicate found
        } // for each vertex
    } // repairDuplicates

#endif /* Repair_hpp */
