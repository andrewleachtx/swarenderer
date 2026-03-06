#include <string>

class App {
public:
    explicit App(const std::string& input_mesh_path,
                 const std::string& output_png_path, int width, int height)
        : input_mesh_path_ { input_mesh_path },
          output_png_path_ { output_png_path }, width_ { width },
          height_ { height }, is_initialized_ { false } {}
    ~App() {}

    App(const App&) = delete;
    App& operator=(const App&) = delete;
    App(App&&) = delete;
    App& operator=(App&&) = delete;

    void init();
    void run();

private:
    std::string input_mesh_path_;
    std::string output_png_path_;
    int width_;
    int height_;
    bool is_initialized_;

    void render_to_png();
};
