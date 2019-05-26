#include "../include/control_tower.hpp"
Control_Tower::Control_Tower() : shutdown_flag_ {false} {}

void Control_Tower::create_flight(std::unique_ptr<Airplane> flight)
{
    if (flight->get_action_type() == Airplane::Action::INCOMING)
        incoming_q.push(std::move(flight));
    else
        outgoing_q.push(std::move(flight));
}

bool Control_Tower::should_shutdown() const { return shutdown_flag_.load(); }