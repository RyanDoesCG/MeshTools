/* * * * * * * * * * * * * * * * * * * * * * * *
 *  ModelLoader.hpp
 *  Ryan Needham
 *
 *  2018
 * * * * * * * * * * * * * * * * * * * * * * * */
#ifndef ModelLoader_hpp
#define ModelLoader_hpp

#include "Vertex.hpp"
#include <sstream>
#include <fstream>
#include <vector>

struct ModelLoader
    {
    
    /**
     *  load
     *
     *  path     - location of the obj model on disk
     *  vertices - target vertex array
     *  indicse  - target index array
     *
     *  reads an obj in from disk
     */
    static void load (const char* path, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
        { // ModelLoader :: load

        // make sure we're working with empty arrays
        vertices.clear();
        indices.clear();

        // create some vectors to hold the characteristics
        // we're expecting to handle. These will be parsed
        // into the mesh's vertex and index vectors.
        std::vector<glm::vec3> positions;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec2> uvs;

        // most values will be represented by their index
        // when reading the faces in.
        std::vector<uint32_t> positionIndices;
        std::vector<uint32_t> normalIndices;
        std::vector<uint32_t> uvIndices;

        // state to read the file from disk
        std::ifstream input(path);
        std::string buffer;

        // we also track how many faces we're reading in
        // to allow more intuitive processing loops
        uint32_t faces = 0;

        // process the data line by line
        while (std::getline(input, buffer))
            { // loop through the file

            std::istringstream in (buffer);
            std::string value;
            std::string type;
            std::getline(in, type, ' ');

            // Read in the vertex / normal / uv data
            if (type ==  "v")
                { // parse vertex position
                glm::vec3 pos; in >> pos.x >> pos.y >> pos.z;
                positions.push_back(pos);
                } // parse vertex position
            
            else if (type == "vn")
                { // parse vertex normal
                glm::vec3 norm; in >> norm.x >> norm.y >> norm.z;
                normals.push_back(glm::normalize(norm));
                } // parse vertex normal

            else if (type == "vt")
                { // parse texture coordinate
                glm::vec2 tc; in >> tc.x >> tc.y;
                uvs.push_back(glm::vec2(tc.x, 1.0 - tc.y));
                } // parse texture coordinate

            else if (type == "s")
                { // parse smoothing toggle
            
                } // parse smoothing toggle

            else if (type == "usemtl")
                { // parse material file

                } // parse material file

            else if (type == "f")
                { // parse face data
                for (uint32_t i = 0; i < 3; ++i)
                    { // for each vertex in the face
                    std::getline(in, value, '/'); if (!value.empty()) positionIndices.push_back(stoi(value) - 1);
                    std::getline(in, value, '/'); if (!value.empty()) uvIndices.push_back(stoi(value) - 1);
                    std::getline(in, value, ' '); if (!value.empty()) normalIndices.push_back(stoi(value) - 1);
                    } // for each vertex in the face
                ++faces;
                } // parse face data
            } // loop through the file

        // Build mesh vertex positions and compute centroid
        for (uint32_t n = 0; n < faces * 3; ++n)
            { // for each face
            Vertex vertex = {
                positions[positionIndices[n]],
                normals[normalIndices[n]],
                glm::vec3 { 0.32, 0.32, 0.32 },
                uvs[uvIndices[n]]};

            // check if this vertex already exists
            bool unique = true;
            uint32_t replicates = 0;
            for (uint32_t v = 0; v < vertices.size(); ++v)
                { // for each existing vertex
                if (vertex == vertices[v])
                    { // vertex replicates the one at v
                    unique = false;
                    replicates = v;
                    } // vertex replicates the one at v
                } // for each existing vertex

            if (unique)
                { // the new vertex is unique
                vertices.push_back(vertex);
                indices.push_back(vertices.size() - 1);
                } // the new vertex is unique
            else
                { // the new vertex already exists
                indices.push_back(replicates);
                } // the new vertex already exists

            } // for each face
            
        if (normals.size() == 0)
            generateNormals (vertices, indices);

        input.close();
        
        } // ModelLoader :: load
        
        static void generateNormals (std::vector<Vertex>& vertices, std::vector<uint32_t>& indices)
            { // ModelLoader :: generateNormals
            for (uint32_t v = 0; v < vertices.size(); ++v)
                { // for each vertex
                glm::vec3 n = glm::vec3(0, 0, 0);

                for (int i = 0; i < indices.size(); i += 3)
                    { // for each face
                    Vertex* a = &vertices[indices[i + 0]];
                    Vertex* b = &vertices[indices[i + 1]];
                    Vertex* c = &vertices[indices[i + 2]];

                    int idInFace = -1;

                    if (*a == vertices[v]) idInFace = 0;
                    if (*b == vertices[v]) idInFace = 1;
                    if (*c == vertices[v]) idInFace = 2;

                    if (idInFace != -1)
                        {
                        glm::vec3 u = b->position - c->position;
                        glm::vec3 v = b->position - a->position;
                        n += glm::cross(u, v);
                        }
                    } // for each face

                n = glm::normalize(n);
                vertices[v].normal = n;
                } // for each vertex
            } // ModelLoader :: generateNormals
    };

#endif /* ModelLoader_hpp */
