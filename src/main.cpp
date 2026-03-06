#include "App.h"
#include <iostream>

// prbably need glfw include to open a window, and tinyobjloader.h to load mesh -> verts

// todo it would be nice to use replusplus vector

int main(int argc, char** argv) {
    if (argc != 3) {
        perror("Usage: ./swarenderer <input mesh obj> <output image>");
        return 1;
    }

    printf("hello\n");
}
