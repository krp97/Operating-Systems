#include "../include/outgoing_airplane.hpp"

Outgoing_Airplane::Outgoing_Airplane(std::chrono::milliseconds speed,
                                     Window& win, std::pair<size_t, size_t> pos)
    : Airplane(speed, win, pos)
{
    start_action();
}

void Outgoing_Airplane::start_action()
{
    airplane_th_ = std::thread(&Outgoing_Airplane::take_off, this);
}

void Outgoing_Airplane::move_to_passenger_area()
{
    move_horizontally(position_, {win_.PASSENGER_STOP, win_.LOWER_LANE_Y});
    win_.light_up_lower_pa(win_.BLUE);
}

void Outgoing_Airplane::move_to_runway()
{
    win_.light_up_lower_pa(win_.WHITE);
    move_horizontally(position_, win_.RIGHT_RUNWAY_START);
    move_vertically(position_, win_.RIGHT_RUNWAY_START);
}

void Outgoing_Airplane::take_off()
{
    move_to_passenger_area();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    move_to_runway();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    move_vertically(position_, win_.RIGHT_RUNWAY_END);
    win_.clear_pos(position_);
}

Airplane::Action Outgoing_Airplane::get_action_type() const
{
    return Airplane::Action::OUTGOING;
}