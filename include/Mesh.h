#pragma once

#include <string>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Mesh {
public:
    Mesh() = default;
    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;
    Mesh(Mesh&&) = delete;
    Mesh& operator=(Mesh&&) = delete;

    void load_mesh(const std::string& input_mesh_filename);

    // all per vertex
    bool is_initialized_ {false};
    std::vector<float> positions_;
    std::vector<float> normals_;
    // std::vector<uint32_t> indices_;
private:
};
