#include "../include/outgoing_airplane.hpp"

Outgoing_Airplane::Outgoing_Airplane(std::chrono::milliseconds speed,
                                     Window& win)
    : Airplane(speed, win, Priority(utils::random_int(1, 5)))
{
    airplane_th_ = std::thread(&Outgoing_Airplane::start_action, this);
}

void Outgoing_Airplane::start_action()
{
    std::unique_lock<std::mutex> lk(priority_mtx_);
    priority_cv_.wait(lk, [&]() {
        if (!first_move_.load())
            priority_.increase_priority();
        return first_move_.load();
    });

    move_to_passenger_area();
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    finished_first_.store(true);

    while (!second_move_.load())
        std::this_thread::sleep_for(std::chrono::milliseconds(20));

    take_off();
}

void Outgoing_Airplane::move_to_passenger_area()
{
    move_horizontally(position_, route_.passenger_area_);
    win_.occupy_pa(route_.passenger_area_);
}

void Outgoing_Airplane::move_to_runway()
{
    win_.free_pa(route_.passenger_area_);
    win_.occupy_runway(route_.runway_start_);
    move_horizontally(position_, route_.runway_start_);
}

void Outgoing_Airplane::take_off()
{
    move_to_runway();
    move_vertically(position_, route_.end_);
    win_.clear_pos(position_);
    finished_second_.store(true);
    win_.free_runway(route_.runway_start_);
}

Airplane::Action Outgoing_Airplane::get_action_type() const
{
    return Airplane::Action::OUTGOING;
}