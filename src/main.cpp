#include "App.h"
#include "Mesh.h"

#include <iostream>

// prbably need glfw include to open a window, and tinyobjloader.h to load mesh -> verts

// todo it would be nice to use replusplus vector

int main(int argc, char** argv) {
    if (argc != 3) {
        printf("Usage: ./swarenderer <input mesh obj> <output image>");
        return 1;
    }

    std::string input_mesh_filename = argv[1];
    std::string output_png_filename = argv[2];
    printf(
        "Loading %s and later exporting to %s\n", input_mesh_filename.c_str(),
        output_png_filename.c_str()
    );

    App app {input_mesh_filename, output_png_filename, 1024, 1024};
    app.init();
    
    app.run();
}
