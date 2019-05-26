#include "../include/outgoing_airplane.hpp"

Outgoing_Airplane::Outgoing_Airplane(std::chrono::milliseconds speed,
                                     Window& win, std::pair<size_t, size_t> pos)
    : Airplane(speed, win, pos)
{
    airplane_th_ = std::thread(&Outgoing_Airplane::start_action, this);
}

void Outgoing_Airplane::start_action()
{
    move_to_passenger_area();
    // Fill some progress bar here
    // move_to_runway();
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
    move_horizontally(position_, win_.RIGHT_RUNWAY_START);
    move_vertically(position_, win_.RIGHT_RUNWAY_START);

    // move from the hangar to the passenger area
    // wait at the passenger area
    // move from the passenger area to the runway start
    // Take off
}

void Outgoing_Airplane::move_to_passenger_area()
{
    move_horizontally(position_, {win_.PASSENGER_STOP, win_.LOWER_LANE_Y});
}

void Outgoing_Airplane::move_horizontally(std::pair<size_t, size_t>& prev,
                                          const std::pair<size_t, size_t> next)
{
    int x_diff = abs(prev.first - next.first);
    for (int moves = 1; moves <= x_diff; ++moves)
    {
        win_.move_on_screen(prev, {prev.first + 1, prev.second});
        ++prev.first;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

void Outgoing_Airplane::move_vertically(std::pair<size_t, size_t>& prev,
                                        const std::pair<size_t, size_t> next)
{
    int y_diff = abs(prev.second - next.second);
    for (int moves = 1; moves <= y_diff; ++moves)
    {
        win_.move_on_screen(prev, {prev.first, prev.second - 1});
        --prev.second;
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }
}

Airplane::Action Outgoing_Airplane::get_action_type() const
{
    return Airplane::Action::OUTGOING;
}