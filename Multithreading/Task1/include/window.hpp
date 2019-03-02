#include <ncurses.h>
#include <memory>

class Window
{
   public:
    Window();
    Window(int h_lines, int v_lines, int x_start, int y_start);
    Window(const Window&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(Window&&) noexcept = default;
    Window(Window&&) noexcept            = default;
    ~Window();

   private:
    std::shared_ptr<WINDOW> window_;
};