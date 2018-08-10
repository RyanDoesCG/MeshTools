//
//  main.cpp
//  ModelValidator
//
//  Created by macbook on 14/06/2018.
//  Copyright © 2018 Hobby. All rights reserved.
//
#include "ModelLoader.hpp"
#include "MeshIO.hpp"
#include "Validation.hpp"
#include "Repair.hpp"
#include <iostream>
#include <chrono>
#include <cmath>
#include <thread>

void timeComparisonTest ();
void countVertices      ();

/**
 *  main
 *
 *  This program accepts as arguments:
 *
 *      - n     : the number of models
 *      - pahts : n paths to models (WITHOUT POSTFIX)
 */
int main (int argc, char** argv)
    { // main
    
    uint32_t n = atoi(argv[1]);

    std::vector<std::string> paths (n);

    for (uint32_t i = 0; i < n; ++i)
        { // for each path
        paths[i] = argv[i + 2];
        } // for each path

    std::vector<std::thread> pool;    
    for (uint32_t i = 0; i < n; ++i)
        { // for each input mesh
        pool.push_back(std::thread(validationSuite, std::ref(paths[i])));
        } // for each input mesh

    for (uint32_t i = 0; i < n; ++i)
        { // for each input mesh
        pool[i].join();
        } // for each input mesh
        
    std::cout << std::endl;
    std::cout << "SUITE COMPLETE" << std::endl;
    std::cout << std::endl;
    return 0;
    } // main

/**
 *  countVertices
 *
 *  reads the mesh at the given path and writes the number
 *  of vertices to the console
 *
 */
void countVertices (std::string path)
    { // countVertices
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    MeshIO::readMeshFile("bust_2.mesh", vertices, indices);
    std::cout << "vertices: " << vertices.size() << std::endl;
    } // countVertices

void timeComparisonTest ()
    { // timeComparisonTest
    std::chrono::high_resolution_clock::time_point t1;
    std::chrono::high_resolution_clock::time_point t2;
    std::vector<Vertex>  vertices;
    std::vector<uint32_t> indices;
    std::vector<std::string> objPaths = {
        "bust_0.obj",
        "bust_1.obj",
        "bust_2.obj",
        "statue.obj",
        "uvsphere.obj"};
    std::vector<std::string> meshPaths = {
        "bust_0.mesh",
        "bust_1.mesh",
        "bust_2.mesh",
        "statue.mesh",
        "uvsphere.mesh"};
        
    for (uint32_t i = 0; i < objPaths.size(); ++i)
        { // for each path
        float meshTime = 0.0f;
        float objTime  = 0.0f;
        t1 = std::chrono::high_resolution_clock::now();
        MeshIO::readMeshFile(meshPaths[i].c_str(), vertices, indices);
        t2 = std::chrono::high_resolution_clock::now();
        meshTime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        t1 = std::chrono::high_resolution_clock::now();
        ModelLoader::load(objPaths[i].c_str(), vertices, indices);
        t2 = std::chrono::high_resolution_clock::now();
        objTime = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1).count();
        std::cout << "mesh read time : " << meshTime << std::endl;
        std::cout << "obj read time  : " << objTime  << std::endl;
        std::cout << "reduction      : " << objTime - meshTime << std::endl;
        std::cout << std::endl;
        } // for each path
    
    } // timeComparisonTest
