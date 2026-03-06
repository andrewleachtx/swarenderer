#include "App.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

void App::init() {
    // todo glfw setup when realtime

    // load input mesh

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

void App::render_to_png() {}
