#include "../include/incoming_airplane.hpp"
#include "utils.cpp"

Incoming_Airplane::Incoming_Airplane(std::chrono::milliseconds speed,
                                     Window& win, Route route)
    : Airplane(speed, win, route, Priority(utils::random_int(3, 8)))
{
    airplane_th_ = std::thread(&Incoming_Airplane::start_action, this);
}

void Incoming_Airplane::start_action()
{
    land();
    move_off_runway();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    move_to_passenger_area();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    win_.light_up_upper_pa(win_.WHITE);

    win_.move_on_screen(position_, route_.end_);
    win_.clear_pos(position_);
}

void Incoming_Airplane::move_to_passenger_area()
{
    win_.move_on_screen(position_, route_.passenger_area_);
    win_.light_up_upper_pa(win_.BLUE);
}

void Incoming_Airplane::land()
{
    win_.move_on_screen(position_, route_.runway_start_);
}

Airplane::Action Incoming_Airplane::get_action_type() const
{
    return Airplane::Action::OUTGOING;
}