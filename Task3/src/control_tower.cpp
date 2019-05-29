#include "../include/control_tower.hpp"

Control_Tower::Control_Tower() { shutdown_flag_.store(false); }

void Control_Tower::create_flight(std::unique_ptr<Airplane> flight)
{
    std::unique_lock<std::mutex> lk(flights_mtx_);
    flights_.push_back(std::move(flight));
    lk.unlock();
    flights_cv_.notify_one();
}

std::vector<std::unique_ptr<Airplane>>::iterator
Control_Tower::get_next_flight()
{
    std::lock_guard<std::mutex> lk(flights_mtx_);
    return std::max_element(flights_.begin(), flights_.end());
}

void Control_Tower::idle_func()
{
    while (!should_shutdown())
    {
        if (flights_.size() > 0)
        {
            auto airplane_it = get_next_flight();
            if (can_start_on_runway_1(airplane_it))
            {
                passenger_area_1_ = std::move(*airplane_it);
                flights_.erase(airplane_it);
                passenger_area_1_->allow_move_to_pa();
                right_runway_ = std::move(passenger_area_1_);
                right_runway_->allow_move_to_runway();
            }
        }
        else
        {
            std::unique_lock<std::mutex> lk(flights_mtx_);
            flights_cv_.wait(lk, [&]() { return flights_.size() > 0; });
        }
    }
}

bool Control_Tower::can_start_on_runway_1(
    std::vector<std::unique_ptr<Airplane>>::const_iterator flight)
{
    if (!passenger_area_1_)
    {
        if (right_runway_ &&
            (right_runway_->get_action_type() == (*flight)->get_action_type()))
        {
            return true;
        }
        else if (!right_runway_)
            return true;
    }
    return false;
}

bool Control_Tower::should_shutdown() const { return shutdown_flag_.load(); }