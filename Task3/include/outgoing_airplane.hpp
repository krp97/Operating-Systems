#pragma once

#include "airplane.hpp"
#include "window.hpp"

class Outgoing_Airplane : public Airplane
{
   public:
    Outgoing_Airplane() = delete;
    Outgoing_Airplane(std::chrono::milliseconds speed, Window& win,
                      std::pair<size_t, size_t> pos);
    Outgoing_Airplane(const Outgoing_Airplane&) = delete;
    Outgoing_Airplane& operator=(const Outgoing_Airplane&) = delete;
    Outgoing_Airplane(Outgoing_Airplane&&)                 = default;
    Outgoing_Airplane& operator=(Outgoing_Airplane&&) = default;

    ~Outgoing_Airplane() override { airplane_th_.join(); };

    void start_action() override;
    Airplane::Action get_action_type() const override;

   private:
    std::thread airplane_th_;

    void move_horizontally(std::pair<size_t, size_t>& prev,
                           const std::pair<size_t, size_t> next) override;
    void move_vertically(std::pair<size_t, size_t>& prev,
                         const std::pair<size_t, size_t> next) override;
    void move_to_passenger_area();
    void take_off();
};