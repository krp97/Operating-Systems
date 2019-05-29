#include "../include/control_tower.hpp"

Control_Tower::Control_Tower(Window& win) : win_ {win}
{
    shutdown_flag_.store(false);
}

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
        schedule_flight();
    }
}

void Control_Tower::schedule_flight()
{
    if (flights_.size() > 0)
    {
        auto airplane_it = get_next_flight();
        switch ((*airplane_it)->get_action_type())
        {
            case Airplane::Action::OUTGOING:
            {
                schedule_outgoing_flight(airplane_it);
                break;
            }
            case Airplane::Action::INCOMING:
            {
                // schedule_incoming_flight(airplane_it);
                break;
            }
        }
    }
}

void Control_Tower::schedule_outgoing_flight(
    std::vector<std::unique_ptr<Airplane>>::iterator flight)
{
    bool right_runw_perm =
        can_start_on_runway(passenger_area_2_, right_runway_, flight);
    bool left_runw_perm =
        can_start_on_runway(passenger_area_1_, left_runway_, flight);

    if (right_runw_perm)
    {
        (*flight)->set_route(Route(
            win_.HANGAR_OUT_LOWER, {win_.PASSENGER_STOP, win_.LOWER_LANE_Y},
            win_.RIGHT_RUNWAY_START, win_.RIGHT_RUNWAY_END));
        passenger_area_2_ = std::move(*flight);
        flights_.erase(flight);
        passenger_area_2_->allow_move_to_pa();
    }
    else if (left_runw_perm)
    {
        (*flight)->set_route(Route(
            win_.HANGAR_OUT_UPPER, {win_.PASSENGER_STOP, win_.UPPER_LANE_Y},
            win_.LEFT_RUNWAY_START, win_.LEFT_RUNWAY_END));
        passenger_area_1_ = std::move(*flight);
        flights_.erase(flight);
        passenger_area_1_->allow_move_to_pa();
    }
}

bool Control_Tower::can_start_on_runway(
    const std::unique_ptr<Airplane>& passenger_area,
    const std::unique_ptr<Airplane>& runway,
    std::vector<std::unique_ptr<Airplane>>::const_iterator flight)
{
    if (!passenger_area)
    {
        if (runway &&
            (runway->get_action_type() == (*flight)->get_action_type()))
        {
            return true;
        }
        else if (!runway)
            return true;
    }
    return false;
}

bool Control_Tower::can_land_on_runway(
    std::unique_ptr<Airplane> passenger_area, std::unique_ptr<Airplane> runway,
    std::vector<std::unique_ptr<Airplane>>::const_iterator flight)
{
    return false;
}

bool Control_Tower::should_shutdown() const { return shutdown_flag_.load(); }