#include "airplane.hpp"
class Incoming_Airplane : public Airplane
{
   public:
    Incoming_Airplane() = delete;
    Incoming_Airplane(std::chrono::milliseconds speed, Window& win,
                      std::pair<size_t, size_t> pos);
    Incoming_Airplane(const Incoming_Airplane&) = delete;
    Incoming_Airplane& operator=(const Incoming_Airplane&) = delete;
    Incoming_Airplane(Incoming_Airplane&&)                 = default;
    Incoming_Airplane& operator=(Incoming_Airplane&&) = default;

    ~Incoming_Airplane() override { airplane_th_.join(); };

    void start_action() override;
    Airplane::Action get_action_type() const override;

   private:
    std::thread airplane_th_;

    void move_to_passenger_area();
    void move_off_runway();
    void land();
};