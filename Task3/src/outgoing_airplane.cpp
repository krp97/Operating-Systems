#include "../include/outgoing_airplane.hpp"

Outgoing_Airplane::Outgoing_Airplane(std::chrono::milliseconds speed,
                                     Window& win, Route route)
    : Airplane(speed, win, route, Priority(utils::random_int(1, 5)))
{
    start_action();
}

void Outgoing_Airplane::start_action()
{
    airplane_th_ = std::thread(&Outgoing_Airplane::take_off, this);
}

void Outgoing_Airplane::move_to_passenger_area()
{
    move_horizontally(position_, route_.passenger_area_);
    win_.light_up_lower_pa(win_.BLUE);
}

void Outgoing_Airplane::move_to_runway()
{
    win_.light_up_lower_pa(win_.WHITE);
    move_horizontally(position_, route_.runway_start_);
}

void Outgoing_Airplane::take_off()
{
    while (!can_move_to_pa_.load())
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    move_to_passenger_area();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    while (!can_move_to_runway_.load())
        std::this_thread::sleep_for(std::chrono::milliseconds(50));

    move_to_runway();
    move_vertically(position_, route_.end_);
    win_.clear_pos(position_);
    finished_action_.store(true);
}

Airplane::Action Outgoing_Airplane::get_action_type() const
{
    return Airplane::Action::OUTGOING;
}