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
        move_active_flights();
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
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
                schedule_incoming_flight(airplane_it);
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
        std::unique_lock<std::mutex> lk(flights_mtx_);
        passenger_area_2_ = std::move(*flight);
        flights_.erase(flight);
        lk.unlock();
        passenger_area_2_->allow_first_move();
    }
    else if (left_runw_perm)
    {
        (*flight)->set_route(Route(
            win_.HANGAR_OUT_UPPER, {win_.PASSENGER_STOP, win_.UPPER_LANE_Y},
            win_.LEFT_RUNWAY_START, win_.LEFT_RUNWAY_END));
        std::unique_lock<std::mutex> lk(flights_mtx_);
        passenger_area_1_ = std::move(*flight);
        flights_.erase(flight);
        lk.unlock();
        passenger_area_1_->allow_first_move();
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

void Control_Tower::schedule_incoming_flight(
    std::vector<std::unique_ptr<Airplane>>::iterator flight)
{
    bool right_runw_perm =
        can_land_on_runway(passenger_area_2_, right_runway_, flight);
    bool left_runw_perm =
        can_land_on_runway(passenger_area_1_, left_runway_, flight);
    if (right_runw_perm)
    {
        (*flight)->set_route(Route(
            win_.RIGHT_RUNWAY_END, {win_.PASSENGER_STOP, win_.LOWER_LANE_Y},
            win_.RIGHT_RUNWAY_START, win_.HANGAR_OUT_LOWER));
        std::unique_lock<std::mutex> lk(flights_mtx_);
        right_runway_ = std::move(*flight);
        flights_.erase(flight);
        lk.unlock();
        right_runway_->allow_first_move();
    }
    else if (left_runw_perm)
    {
        (*flight)->set_route(Route(
            win_.LEFT_RUNWAY_END, {win_.PASSENGER_STOP, win_.UPPER_LANE_Y},
            win_.LEFT_RUNWAY_START, win_.HANGAR_OUT_UPPER));
        std::unique_lock<std::mutex> lk(flights_mtx_);
        left_runway_ = std::move(*flight);
        flights_.erase(flight);
        lk.unlock();
        left_runway_->allow_first_move();
    }
}

bool Control_Tower::can_land_on_runway(
    const std::unique_ptr<Airplane>& passenger_area,
    const std::unique_ptr<Airplane>& runway,
    std::vector<std::unique_ptr<Airplane>>::const_iterator flight)
{
    if (!runway)
    {
        if (passenger_area &&
            (passenger_area->get_action_type() == (*flight)->get_action_type()))
        {
            return true;
        }
        else if (!passenger_area)
            return true;
    }
    return false;
}

void Control_Tower::move_active_flights()
{
    cleanup_finished_flights();
    move_outgoing_flights(passenger_area_1_, left_runway_);
    move_outgoing_flights(passenger_area_2_, right_runway_);
    move_incoming_flights(passenger_area_1_, left_runway_);
    move_incoming_flights(passenger_area_2_, right_runway_);
}

void Control_Tower::cleanup_finished_flights()
{
    cleanup_flight_at(passenger_area_1_, Airplane::Action::INCOMING);
    cleanup_flight_at(passenger_area_2_, Airplane::Action::INCOMING);
    cleanup_flight_at(left_runway_, Airplane::Action::OUTGOING);
    cleanup_flight_at(right_runway_, Airplane::Action::OUTGOING);
}

void Control_Tower::cleanup_flight_at(std::unique_ptr<Airplane>& checkpoint,
                                      Airplane::Action action_type)
{
    if (checkpoint)
        if (checkpoint->get_action_type() == action_type)
            if (checkpoint->has_finished_second_stage())
                checkpoint.reset(nullptr);
}

void Control_Tower::move_outgoing_flights(
    std::unique_ptr<Airplane>& passenger_area,
    std::unique_ptr<Airplane>& runway)
{
    if (passenger_area)
    {
        if (passenger_area->get_action_type() == Airplane::Action::OUTGOING)
        {
            if (!runway && passenger_area->has_finished_first_stage())
            {
                runway = std::move(passenger_area);
                runway->allow_second_move();
            }
        }
    }
}

void Control_Tower::move_incoming_flights(
    std::unique_ptr<Airplane>& passenger_area,
    std::unique_ptr<Airplane>& runway)
{
    if (runway)
    {
        if (runway->get_action_type() == Airplane::Action::INCOMING)
        {
            if (!passenger_area && runway->has_finished_first_stage())
            {
                passenger_area = std::move(runway);
                passenger_area->allow_second_move();
            }
        }
    }
}

bool Control_Tower::should_shutdown() const { return shutdown_flag_.load(); }