#include "App.h"
#include "Mesh.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <limits>
#include <stdexcept>

static constexpr uint8_t WHITE = 255;
static constexpr uint8_t BLACK = 0;

static void
set_pixel(int x, int y, std::vector<uint8_t>& pixels, int width, int height) {
    if (x < 0 || x >= width || y < 0 || y >= height)
        return;
    size_t idx = (y * width + x) * 4;
    pixels[idx] = BLACK;
    pixels[idx + 1] = BLACK;
    pixels[idx + 2] = BLACK;
    pixels[idx + 3] = WHITE;
}

static void draw_line(
    int x0, int y0, int x1, int y1, std::vector<uint8_t>& pixels, int width,
    int height
) {
    // y = mx + b, only allow y to deviate a certain amt
    // handle steep lines by swapping axes
    bool steep = abs(y1 - y0) > abs(x1 - x0);
    if (steep) {
        std::swap(x0, y0);
        std::swap(x1, y1);
    }

    // ensure we always iterate left to right
    if (x0 > x1) {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }

    const float m = (x1 == x0) ? 0.0f : static_cast<float>(y1 - y0) / (x1 - x0);

    int y = y0;
    for (int x = x0; x <= x1; x++) {
        float expected_y = m * (x - x0) + y0;
        if (static_cast<float>(y) - expected_y > 0.5f) {
            y--;
        }
        else if (expected_y - static_cast<float>(y) > 0.5f) {
            y++;
        }

        if (steep)
            set_pixel(y, x, pixels, width, height);
        else
            set_pixel(x, y, pixels, width, height);
    }
}

void App::init() {
    // todo glfw setup when realtime

    // load input mesh, we just need a vector < vertex > to loop over here
    Mesh mesh {};
    mesh.load_mesh(input_mesh_path_);
    mesh.normalize_scale();
    meshes_.push_back(std::move(mesh));

    // init flattened pixel buffer for the frame
    pixels_ = std::vector<uint8_t>(width_ * height_ * 4, WHITE);

    is_initialized_ = true;
}

void App::run() {
    if (!is_initialized_) {
        throw std::runtime_error(
            "App must be initialized with init() before calling run()!"
        );
    }
    // while !exit: listen for events -> render_to_glfw()

    // loop over all tris in the mesh ... apply transforms
    // update pixel buffer, render to image

    // assume cam is at (x0, y0, z0) - point camera down z axis, up is {0, 1, 0}
    const glm::vec3 target {0.0f, 0.0f, 0.0f};
    const glm::vec3 eye {0.0f, 0.0f, 3.0f};
    const glm::vec3 n = glm::normalize(eye - target);
    const glm::vec3 up {0.0, 1.0f, 0.0f};

    const glm::vec3 u = glm::normalize(glm::cross(up, n));
    const glm::vec3 v = glm::cross(n, u);

    // glm is col major, so mat[col][row]
    glm::mat4x4 world_to_view_mat = {
        glm::vec4 {u.x, v.x, n.x, 0.0f}, glm::vec4 {u.y, v.y, n.y, 0.0f},
        glm::vec4 {u.z, v.z, n.z, 0.0f},
        glm::vec4 {
            -glm::dot(u, eye), -glm::dot(v, eye), -glm::dot(n, eye), 1.0f
        }
    };

    const float z_near = 0.1f;
    const float z_far = 100.0f;
    const float fov = glm::radians(90.0f);
    const float width = -2.0f * z_near * glm::tan(fov * 0.5f);
    const float aspect = 16.0f / 9.0f;
    const float height = width / aspect;
    glm::mat4x4 projection_mat {};
    projection_mat[0][0] = (2.0f * z_near) / width;
    projection_mat[1][1] = (2.0f * z_near) / height;
    projection_mat[2][2] = -(z_far + z_near) / (z_far - z_near);
    projection_mat[2][3] = -1.0f;
    projection_mat[3][2] = (-2.0f * z_far * z_near) / (z_far - z_near);

    for (const Mesh& m : meshes_) {
        float min_x = std::numeric_limits<float>::max(),
              max_x = std::numeric_limits<float>::lowest();
        float min_y = std::numeric_limits<float>::max(),
              max_y = std::numeric_limits<float>::lowest();
        float min_z = std::numeric_limits<float>::max(),
              max_z = std::numeric_limits<float>::lowest();
        for (size_t i = 0; i < m.positions_.size(); i += 3) {
            min_x = std::min(min_x, m.positions_[i]);
            max_x = std::max(max_x, m.positions_[i]);
            min_y = std::min(min_y, m.positions_[i + 1]);
            max_y = std::max(max_y, m.positions_[i + 1]);
            min_z = std::min(min_z, m.positions_[i + 2]);
            max_z = std::max(max_z, m.positions_[i + 2]);
        }
        printf(
            "bounds x[%.3f, %.3f] y[%.3f, %.3f] z[%.3f, %.3f]\n", min_x, max_x,
            min_y, max_y, min_z, max_z
        );

        for (size_t i = 0; i < m.positions_.size(); i += 9) {
            glm::ivec2 screen_verts[3];
            bool skip_tri = false;

            for (int j = 0; j < 3; j++) {
                glm::vec4 vert {
                    m.positions_[i + j * 3], m.positions_[i + j * 3 + 1],
                    m.positions_[i + j * 3 + 2], 1.0f
                };

                vert = world_to_view_mat * vert;
                vert = projection_mat * vert;

                if (vert.x < -vert.w || vert.x > vert.w || vert.y < -vert.w ||
                    vert.y > vert.w || vert.z < -vert.w || vert.z > vert.w) {
                    skip_tri = true;
                    break;
                }

                vert /= vert.w;

                screen_verts[j] = glm::ivec2 {
                    static_cast<int>(vert.x * (width_ / 2) + (width_ / 2)),
                    static_cast<int>(-vert.y * (height_ / 2) + (height_ / 2))
                };
            }

            if (skip_tri)
                continue;

            // draw 3 edges: v0-v1, v1-v2, v2-v0
            draw_line(
                screen_verts[0].x, screen_verts[0].y, screen_verts[1].x,
                screen_verts[1].y, pixels_, width_, height_
            );
            draw_line(
                screen_verts[1].x, screen_verts[1].y, screen_verts[2].x,
                screen_verts[2].y, pixels_, width_, height_
            );
            draw_line(
                screen_verts[2].x, screen_verts[2].y, screen_verts[0].x,
                screen_verts[0].y, pixels_, width_, height_
            );
        }
    }

    render_to_png();
}

void App::render_to_png() {
    // i have no idea if its 4 channels, and if the args i used are right
    const int NUM_CHANNELS = 4;
    int rc = stbi_write_png(
        output_png_path_.c_str(), width_, height_, NUM_CHANNELS, pixels_.data(),
        width_ * NUM_CHANNELS
    );
    if (rc == 0) {
        printf("stbi_write_png failed!\n");
        exit(1);
    }
}
