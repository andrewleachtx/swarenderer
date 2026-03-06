#include <string>
#include <vector>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class App {
public:
    explicit App(const std::string& input_mesh_path,
                 const std::string& output_png_path, int width, int height)
        : is_initialized_ { false }, input_mesh_path_ { input_mesh_path },
          output_png_path_ { output_png_path }, width_ { width },
          height_ { height } {}
    ~App() {}

    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(App&&) = delete;
    App& operator=(App&&) = delete;

    void init();
    void run();

private:
    bool is_initialized_;
    std::string input_mesh_path_;
    std::string output_png_path_;
    int width_;
    int height_;
    std::vector<glm::vec4> pixels_;

    void render_to_png();
};
