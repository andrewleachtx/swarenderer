#include "App.h"
#include "Mesh.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void App::init() {
    // todo glfw setup when realtime

    // load input mesh, we just need a vector < vertex > to loop over here
    // mesh_ = load_mesh()

    // init flattened pixel buffer for the frame
    pixels_ = std::vector<glm::vec4>(width_ * height_, glm::vec4 {});

    is_initialized_ = true;
}

void App::run() {
    // while !exit: listen for events -> render_to_glfw()

    // loop over all tris in the mesh ... apply transforms
    // update pixel buffer, render to image

    // assume cam is at (x0, y0, z0) - point camera down z axis, up is {0, 1, 0}
    const glm::vec3 lookAt =
        glm::vec3 {0.0f, 0.0f, -1.0f} - glm::vec3 {0.0f, 0.0f, 0.0f};
    const glm::vec3 n = glm::normalize(lookAt);
    const glm::vec3 up {0.0, 1.0f, 0.0f};

    const glm::vec3 u = glm::normalize(glm::cross(up, n));
    const glm::vec3 v = glm::cross(n, u);

    // glm is col major, so mat[col][row]
    glm::mat4x4 world_to_view_mat = {
        glm::vec4 {u.x, v.x, n.x, 0.0f}, glm::vec4 {u.y, v.y, n.y, 0.0f},
        glm::vec4 {u.z, v.z, n.z, 0.0f}, glm::vec4 {0.0f, 0.0f, 0.0f, 1.0f}
    };

    const float z_near = 0.1f;
    const float z_far = 1000.0f;
    const float fov = glm::radians(90.0f);
    const float width = -2.0f * z_near * glm::tan(fov * 0.5f);
    const float aspect = 16.0f / 9.0f;
    const float height = width / aspect;
    glm::mat4x4 view_to_clip {};
    view_to_clip[0][0] = (2.0f * z_near) / width;
    view_to_clip[1][1] = (2.0f * z_near) / height;
    view_to_clip[2][2] = -(z_far + z_near) / (z_far - z_near);
    view_to_clip[2][3] = -1.0f;
    view_to_clip[3][2] = (-2.0f * z_far * z_near) / (z_far - z_near);

    for (const Mesh& m : meshes_) {
        std::vector<glm::vec3> output_positions(m.positions_.size() / 3, {});
        for (size_t i = 0; i < m.positions_.size(); i += 3) {
            glm::vec4 vert {
                m.positions_[i], m.positions_[i + 1], m.positions_[i + 2], 1.0f
            };
            
            output_positions[i / 3] = vert;

            // world -> view
            vert = world_to_view_mat * vert;

            // view -> clip space via projection
            vert = view_to_clip * vert;

            // clip anything beyond [-w, w]
            if (vert.x < -vert.w || vert.x > vert.w || vert.y < -vert.w ||
                vert.y > vert.w || vert.z < -vert.w || vert.z > vert.w) {
                continue;
            }

            // ...

            // write back in place
            output_positions[i / 3] = vert;
        }
    }

    render_to_png();
}

void App::render_to_png() {
    // i have no idea if its 4 channels, and if the args i used are right
    const int NUM_CHANNELS = 4;
    int rc = stbi_write_png(
        output_png_path_.c_str(), width_, height_, NUM_CHANNELS, pixels_.data(),
        width_ * height_ * NUM_CHANNELS
    );
    if (rc == 0) {
        printf("stbi_write_png failed!\n");
        exit(1);
    }
}
