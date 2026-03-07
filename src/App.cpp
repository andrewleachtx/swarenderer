#include "App.h"

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

    render_to_png();
}

void App::render_to_png() {
    // i have no idea if its 4 channels, and if the args i used are right
    const int NUM_CHANNELS = 4;
    int rc =
        stbi_write_png(output_png_path_.c_str(), width_, height_, NUM_CHANNELS,
                       pixels_.data(), width_ * height_ * NUM_CHANNELS);
    if (rc == 0) {
        printf("stbi_write_png failed!\n");
        exit(1);
    }
}
