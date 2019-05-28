#include "../include/control_tower.hpp"
Control_Tower::Control_Tower() : shutdown_flag_ {false} {}

void Control_Tower::create_flight(std::unique_ptr<Airplane> flight)
{
    flights_.push_back(std::move(flight));
}

void Control_Tower::schedule_flight() {}

bool Control_Tower::should_shutdown() const { return shutdown_flag_.load(); }