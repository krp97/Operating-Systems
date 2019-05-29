#include "control_tower.hpp"
#include "incoming_airplane.hpp"
#include "outgoing_airplane.hpp"

class Flight_Generator
{
   public:
    Flight_Generator(Window& win, Control_Tower& ct, float out_in_ratio,
                     std::chrono::milliseconds frequency);
    Flight_Generator(const Flight_Generator&) = delete;
    Flight_Generator& operator=(const Flight_Generator&) = delete;
    Flight_Generator(Flight_Generator&&)                 = default;
    Flight_Generator& operator=(Flight_Generator&&) = default;
    ~Flight_Generator() { f_generator_th_.join(); };

    void generate_loop();

   private:
    Window& win_;
    Control_Tower& ct_;
    float out_in_ratio_;
    std::chrono::milliseconds frequency_;
    std::thread f_generator_th_;
};