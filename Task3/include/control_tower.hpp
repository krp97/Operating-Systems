#pragma once
#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <memory>
#include <queue>
#include "airplane.hpp"

class Control_Tower
{
   public:
    Control_Tower();
    Control_Tower(Control_Tower&) = delete;
    Control_Tower& operator=(const Control_Tower&) = delete;
    Control_Tower(Control_Tower&&)                 = default;
    Control_Tower& operator=(Control_Tower&&) = default;
    ~Control_Tower()                          = default;

    void idle_func();
    void create_flight(std::unique_ptr<Airplane> flight);
    bool should_shutdown() const;

   private:
    std::vector<std::unique_ptr<Airplane>> flights_;
    std::atomic_bool shutdown_flag_;

    std::mutex flights_mtx_;
    std::condition_variable flights_cv_;

    std::unique_ptr<Airplane> passenger_area_1_;
    std::unique_ptr<Airplane> right_runway_;

    bool can_start_on_runway_1(
        std::vector<std::unique_ptr<Airplane>>::const_iterator flight);

    std::vector<std::unique_ptr<Airplane>>::iterator get_next_flight();
};