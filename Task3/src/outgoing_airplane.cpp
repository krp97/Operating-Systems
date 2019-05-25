#include "../include/outgoing_airplane.hpp"

Outgoing_Airplane::Outgoing_Airplane(std::chrono::milliseconds speed,
                                     const Window& win,
                                     std::pair<size_t, size_t> pos)
    : Airplane(speed, win, pos)
{
    airplane_th_ = std::thread(&Outgoing_Airplane::start_action, this);
}

void Outgoing_Airplane::start_action() {}

Airplane::Airplane_Type Outgoing_Airplane::get_action_type() const
{
    return Airplane::Airplane_Type::OUTGOING;
}