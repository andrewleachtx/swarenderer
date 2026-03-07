#define TINYOBJLOADER_IMPLEMENTATION
#include "Mesh.h"

#include <iostream>

#include "tiny_obj_loader.h"

// Originally retrieved from Shinjiro Sueda https://people.engr.tamu.edu/sueda/index.html#
void Mesh::load_mesh(const std::string& input_mesh_filename) {
    // Load geometry
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string errStr;
    bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr,
                               input_mesh_filename.c_str());
    if (!rc) {
        std::cerr << errStr << std::endl;
    }
    else {
        // Some OBJ files have different indices for vertex positions, normals,
        // and texture coordinates. For example, a cube corner vertex may have
        // three different normals. Here, we are going to duplicate all such
        // vertices.
        // Loop over shapes
        for (size_t s = 0; s < shapes.size(); s++) {
            // Loop over faces (polygons)
            size_t index_offset = 0;
            for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size();
                 f++) {
                size_t fv = shapes[s].mesh.num_face_vertices[f];
                // Loop over vertices in the face.
                for (size_t v = 0; v < fv; v++) {
                    // access to vertex
                    tinyobj::index_t idx =
                        shapes[s].mesh.indices[index_offset + v];
                    positions_.push_back(
                        attrib.vertices[3 * idx.vertex_index + 0]);
                    positions_.push_back(
                        attrib.vertices[3 * idx.vertex_index + 1]);
                    positions_.push_back(
                        attrib.vertices[3 * idx.vertex_index + 2]);
                    if (!attrib.normals.empty()) {
                        normals_.push_back(
                            attrib.normals[3 * idx.normal_index + 0]);
                        normals_.push_back(
                            attrib.normals[3 * idx.normal_index + 1]);
                        normals_.push_back(
                            attrib.normals[3 * idx.normal_index + 2]);
                    }
                    // if (!attrib.texcoords.empty()) {
                    //     texBuf.push_back(
                    //         attrib.texcoords[2 * idx.texcoord_index + 0]);
                    //     texBuf.push_back(
                    //         attrib.texcoords[2 * idx.texcoord_index + 1]);
                    // }
                }
                index_offset += fv;
                // per-face material (IGNORE)
                //shapes[s].mesh.material_ids[f];
            }
        }
    }
}
