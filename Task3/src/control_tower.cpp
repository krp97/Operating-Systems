#include "../include/control_tower.hpp"

Control_Tower::Control_Tower(Window& win)
    : win_ {win},
      shutdown_flag_ {false},
      left_runw_broken_ {false},
      right_runw_broken_ {false},
      during_fix_ {false}
{
}

void Control_Tower::create_flight(std::unique_ptr<Airplane> flight)
{
    std::unique_lock<std::mutex> lk(flights_mtx_);
    flights_.push_back(std::move(flight));
    lk.unlock();
}

std::vector<std::unique_ptr<Airplane>>::iterator
Control_Tower::get_next_flight()
{
    std::unique_lock<std::mutex> flights_lock(flights_mtx_);
    return std::max_element(flights_.begin(), flights_.end());
}

void Control_Tower::break_runway()
{
    if (!left_runw_broken_.load() && !right_runw_broken_.load())
    {
        int random_break = utils::random_int(1, 2);
        random_break == 1 ? break_left_runway() : break_right_runway();
    }
}

void Control_Tower::idle_func()
{
    refresh();
    win_.draw_foreground();
    while (!should_shutdown())
    {
        schedule_flight();
        move_active_flights();
        check_runways_state();
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
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
        can_start_on_runway(passenger_area_2_, right_runway_, flight) &&
        !right_runw_broken_;

    bool left_runw_perm =
        can_start_on_runway(passenger_area_1_, left_runway_, flight) &&
        !left_runw_broken_;

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
        bump_priorities(passenger_area_2_);
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
        bump_priorities(passenger_area_1_);
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
        can_land_on_runway(passenger_area_2_, right_runway_, flight) &&
        !right_runw_broken_;
    bool left_runw_perm =
        can_land_on_runway(passenger_area_1_, left_runway_, flight) &&
        !left_runw_broken_;

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
        bump_priorities(right_runway_);
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
        bump_priorities(left_runway_);
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

bool Control_Tower::should_wait() const
{
    bool clear_passenger_area = !passenger_area_1_ && passenger_area_2_;
    bool clear_runways        = !left_runway_ && !right_runway_;
    return clear_passenger_area && clear_runways && flights_.size() == 0;
}

void Control_Tower::bump_priorities(
    const std::unique_ptr<Airplane>& skip_flight)
{
    for (auto& flight : flights_)
    {
        if (flight != skip_flight)
            flight->bump_priority();
    }
}

void Control_Tower::break_left_runway()
{
    left_runw_broken_.store(true);
    win_.break_runway(win_.LEFT_RUNWAY_START);
}

void Control_Tower::break_right_runway()
{
    right_runw_broken_.store(true);
    win_.break_runway(win_.RIGHT_RUNWAY_START);
}

void Control_Tower::check_runways_state()
{
    if ((right_runw_broken_.load() || left_runw_broken_.load()) && !during_fix_)
        fix_broken_runways();
    if (runway_fix_.valid() && runway_fix_.wait_for(std::chrono::milliseconds(
                                   0)) == std::future_status::ready)
    {
        runway_fix_.get();
        during_fix_.store(false);
    }
}

void Control_Tower::fix_broken_runways()
{
    if (right_runw_broken_ && !passenger_area_2_ && !right_runway_)
    {
        during_fix_.store(true);
        runway_fix_ = std::async(std::launch::async, [&]() {
            fix_runway(right_runw_broken_, win_.RIGHT_RUNWAY_STAT,
                       shutdown_flag_);
        });
    }
    else if (left_runw_broken_ && !passenger_area_1_ && !left_runway_)
    {
        during_fix_.store(true);
        runway_fix_ = std::async(std::launch::async, [&]() {
            fix_runway(left_runw_broken_, win_.LEFT_RUNWAY_STAT,
                       shutdown_flag_);
        });
    }
}

void Control_Tower::fix_runway(std::atomic_bool& runway_flag,
                               const short runway_v_pos,
                               std::atomic_bool& shutdown_flag_)
{
    // Run once too much, so the loading bar disappears after 100%.
    for (int i = 0; i <= 16 && !shutdown_flag_.load(); ++i)
    {
        win_.update_loading_bar(runway_v_pos, i);
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    runway_flag.store(false);
}

void Control_Tower::shutdown_checkpoint(std::unique_ptr<Airplane>& flight)
{
    if (flight)
        flight->shutdown();
}

Control_Tower::~Control_Tower()
{
    if (runway_fix_.valid())
        runway_fix_.get();
    for (auto& flight : flights_) { flight->shutdown(); }

    shutdown_checkpoint(passenger_area_1_);
    shutdown_checkpoint(passenger_area_2_);
    shutdown_checkpoint(left_runway_);
    shutdown_checkpoint(right_runway_);

    passenger_area_1_.reset(nullptr);
    passenger_area_2_.reset(nullptr);
    right_runway_.reset(nullptr);
    left_runway_.reset(nullptr);
    flights_.clear();
}