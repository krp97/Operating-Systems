#pragma once
#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <future>
#include <memory>
#include <queue>
#include "airplane.hpp"

class Control_Tower
{
   public:
    Control_Tower(Window& win);
    Control_Tower(Control_Tower&) = delete;
    Control_Tower& operator=(const Control_Tower&) = delete;
    Control_Tower(Control_Tower&&)                 = default;
    Control_Tower& operator=(Control_Tower&&) = default;
    ~Control_Tower();

    void create_flight(std::unique_ptr<Airplane> flight);
    void break_runway();

    void idle_func();

    bool should_shutdown() const;
    void shutdown() { shutdown_flag_.store(true); };
    bool should_wait() const;

   private:
    Window& win_;
    std::vector<std::unique_ptr<Airplane>> flights_;
    std::mutex flights_mtx_;

    std::atomic_bool shutdown_flag_;
    std::atomic_bool left_runw_broken_;
    std::atomic_bool right_runw_broken_;

    std::future<void> runway_fix_;
    std::atomic_bool during_fix_;

    std::unique_ptr<Airplane> passenger_area_1_;
    std::unique_ptr<Airplane> passenger_area_2_;
    std::unique_ptr<Airplane> right_runway_;
    std::unique_ptr<Airplane> left_runway_;

    void schedule_flight();

    void schedule_outgoing_flight(
        std::vector<std::unique_ptr<Airplane>>::iterator flight);
    void schedule_incoming_flight(
        std::vector<std::unique_ptr<Airplane>>::iterator flight);

    bool can_start_on_runway(
        const std::unique_ptr<Airplane>& passenger_area,
        const std::unique_ptr<Airplane>& runway,
        std::vector<std::unique_ptr<Airplane>>::const_iterator flight);

    bool can_land_on_runway(
        const std::unique_ptr<Airplane>& passenger_area,
        const std::unique_ptr<Airplane>& runway,
        std::vector<std::unique_ptr<Airplane>>::const_iterator flight);

    void move_active_flights();

    void cleanup_finished_flights();
    void cleanup_flight_at(std::unique_ptr<Airplane>& checkpoint,
                           Airplane::Action action_type);
    void move_outgoing_flights(std::unique_ptr<Airplane>& passenger_area,
                               std::unique_ptr<Airplane>& runway);
    void move_incoming_flights(std::unique_ptr<Airplane>& passenger_area,
                               std::unique_ptr<Airplane>& runway);

    std::vector<std::unique_ptr<Airplane>>::iterator get_next_flight();

    void bump_priorities(const std::unique_ptr<Airplane>& skip_flight);
    void break_left_runway();
    void break_right_runway();

    void check_runways_state();
    void fix_broken_runways();
    void fix_runway(std::atomic_bool& runway_flag, const short runway_v_pos,
                    std::atomic_bool& shutdown_flag_);

    void shutdown_checkpoint(std::unique_ptr<Airplane>& flight);
};