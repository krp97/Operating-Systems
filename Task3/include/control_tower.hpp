#pragma once
#include <atomic>
#include <memory>
#include <queue>
#include "airplane.hpp"

class Control_Tower
{
   public:
    Control_Tower();
    Control_Tower(const Control_Tower&) = delete;
    Control_Tower& operator=(const Control_Tower&) = delete;
    Control_Tower(Control_Tower&&)                 = default;
    Control_Tower& operator=(Control_Tower&&) = default;
    ~Control_Tower();

    void create_flight(std::unique_ptr<Airplane> flight);

    bool should_shutdown() const;

   private:
    std::queue<std::unique_ptr<Airplane>> incoming_q;
    std::queue<std::unique_ptr<Airplane>> outgoing_q;
    std::atomic_bool shutdown_flag_;
};