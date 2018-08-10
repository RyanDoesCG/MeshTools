//
//  Validation.hpp
//  ModelValidator
//
//  Created by macbook on 18/06/2018.
//  Copyright © 2018 Hobby. All rights reserved.
//

#ifndef Validation_hpp
#define Validation_hpp

#include "Vertex.hpp"
#include "Repair.hpp"
#include <vector>

/**
 *  ensures the mesh in the global arrays has a
 *  unique parameterisation
 */
bool testUniqueParameterisation (std::vector<Vertex>& vertices)
    { // testUniqueParameterisation
    std::vector<bool> isUnique;
    isUnique.assign(vertices.size(), true);
    for (uint32_t i = 0; i < vertices.size(); ++i)
        { // for each vertex
        // ensure the texture coordinates are unique
        for (uint32_t j = i; j < vertices.size(); ++j)
            if (i != j)
                if (vertices[i].uvs.s == vertices[j].uvs.s &&
                    vertices[i].uvs.t == vertices[j].uvs.t)
                    { // duplicate found
                    isUnique[i] = false;
                    isUnique[j] = false;
                    } // duplicate found
        } // for each vertex
    int duplicates = 0;
    for (bool b : isUnique)
        if (!b) ++duplicates;
    std::cout << "duplicates: " << duplicates << std::endl;
    return duplicates == 0;
    } // testUniqueParameterisation

/**
 *
 */
bool testParameterisationRange (std::vector<Vertex>& vertices)
    { // testParameterisationRange
    for (uint32_t v = 0; v < vertices.size(); ++v)
        { // for each vertex
        if (vertices[v].uvs.s > 1.0) return false;
        if (vertices[v].uvs.s < 0.0) return false;
        if (vertices[v].uvs.t > 1.0) return false;
        if (vertices[v].uvs.t < 0.0) return false;
        } // for each vertex
    return true;
    } // testParameterisationRange


/**
 *  validationSuite
 *
 *  validates, repairs and reformats the mesh at the given path
 *
 */
void validationSuite (std::string& path)
    { // validationSuite

    std::string objPostfix (".obj");
    std::string meshPostfix (".mesh");
    
    // Read in Object File
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::string objPath;
    objPath += path;
    objPath += objPostfix;
    ModelLoader::load(objPath.c_str(), vertices, indices);
    std::cout << std::endl;
    std::cout << objPath << " read from disk" << std::endl;
    std::cout << "vertices: " << vertices.size() << std::endl;
    std::cout << "indices:  " << indices.size() << std::endl;
    std::cout << std::endl;

    // Validate Mesh
    bool correctUVs = true;//testParameterisationRange(vertices);
    bool uniqueUVs  = true;//testUniqueParameterisation(vertices);
    while (!uniqueUVs || !correctUVs)
        {
        // Report issue to user
        std::cout << "MESH INVALID" << std::endl;
        if (!correctUVs) std::cout << "Invalid uvs Found" << std::endl;
        if (!uniqueUVs) std::cout << "Duplicate uvs Found" << std::endl;
        // Attempt repair
        if (!correctUVs) repairInvalidUVs(vertices);
        if (!uniqueUVs) repairDuplicateUVs(vertices);
        // Check repair success
        correctUVs = testParameterisationRange(vertices);
        uniqueUVs  = testUniqueParameterisation(vertices);
        }
    std::cout << "MESH VALID" << std::endl;
    MeshIO::center (vertices);
    MeshIO::paint  (vertices, 0.64f, 0.64f, 0.64f);
    
    // Write mesh file
    std::string meshPath;
    meshPath += path;
    meshPath += meshPostfix;
    MeshIO::writeMeshFile(meshPath.c_str(), vertices, indices);
    std::cout << meshPath << " written to disk" << std::endl;
    std::cout << std::endl;
    } // validationSuite


#endif /* Validation_hpp */
