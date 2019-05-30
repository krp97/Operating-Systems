#include "../include/incoming_airplane.hpp"
#include <iostream>
Incoming_Airplane::Incoming_Airplane(std::chrono::milliseconds speed,
                                     Window& win)
    : Airplane(speed, win, Priority(utils::random_int(3, 8)))
{
    airplane_th_ = std::thread(&Incoming_Airplane::start_action, this);
}

void Incoming_Airplane::start_action()
{
    while (!first_move_.load())
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    land();
    finished_first_.store(true);
    while (!second_move_.load())
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    move_to_passenger_area();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    win_.free_pa(route_.passenger_area_);
    move_horizontally(position_, route_.end_);
    win_.clear_pos(position_);
    finished_second_.store(true);
}

void Incoming_Airplane::move_to_passenger_area()
{
    win_.free_runway(route_.runway_start_);
    move_horizontally(position_, route_.passenger_area_);
    win_.occupy_pa(route_.passenger_area_);
}

void Incoming_Airplane::land()
{
    win_.occupy_runway(route_.runway_start_);
    move_vertically(position_, route_.runway_start_);
}

Airplane::Action Incoming_Airplane::get_action_type() const
{
    return Airplane::Action::INCOMING;
}