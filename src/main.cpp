#include <iostream>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

// prbably need glfw include to open a window, and tinyobjloader.h to load mesh -> verts

int main(int argc, char** argv) {
    if (argc != 3) {
        perror("Usage: ./swarenderer <input mesh obj> <output image>");
        return 1;
    }

    printf("hello\n");
}
