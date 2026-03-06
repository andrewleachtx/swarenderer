#include "App.h"

void App::init() {
    // todo glfw setup when realtime

    // load input mesh

    is_initialized_ = true;
}

void App::run() {
    // while !exit: listen for events -> render_to_glfw()

    render_to_png();
}

void App::render_to_png() {
    // loop over all tris in the mesh ... apply transforms
}
