#include "../include/incoming_airplane.hpp"

Incoming_Airplane::Incoming_Airplane(std::chrono::milliseconds speed,
                                     Window& win, std::pair<size_t, size_t> pos)
    : Airplane(speed, win, pos)
{
    airplane_th_ = std::thread(&Incoming_Airplane::start_action, this);
}

void Incoming_Airplane::start_action()
{
    land();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    move_off_runway();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    move_to_passenger_area();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    move_horizontally(position_, win_.HANGAR_OUT);
    win_.clear_pos(position_);
}

void Incoming_Airplane::move_to_passenger_area()
{
    move_horizontally(position_, {win_.PASSENGER_STOP, position_.second});
}

void Incoming_Airplane::move_off_runway()
{
    move_vertically(position_, {position_.first, win_.UPPER_LANE_Y});
}

void Incoming_Airplane::land()
{
    move_vertically(position_, win_.LEFT_RUNWAY_START);
}

Airplane::Action Incoming_Airplane::get_action_type() const
{
    return Airplane::Action::OUTGOING;
}