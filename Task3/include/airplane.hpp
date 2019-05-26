#pragma once

#include <chrono>
#include <functional>
#include <thread>
#include "../src/utils.cpp"
#include "window.hpp"

class Airplane
{
   public:
    enum class Action
    {
        OUTGOING,
        INCOMING
    };

    Airplane() = delete;
    Airplane(std::chrono::milliseconds speed, Window& win,
             std::pair<size_t, size_t> pos)
        : speed_ {speed}, win_ {win}, position_ {pos} {};

    virtual ~Airplane() = default;

    // Take off or Landing, depending on the base class implementation.
    virtual void start_action()            = 0;  // Better name ?
    virtual Action get_action_type() const = 0;

   protected:
    std::chrono::milliseconds speed_;
    Window& win_;
    std::pair<size_t, size_t> position_;

    void move_horizontally(std::pair<size_t, size_t>& prev,
                           const std::pair<size_t, size_t> next)
    {
        int x_diff      = prev.first - next.first;
        auto func       = utils::get_operator_for_sign(x_diff);
        auto iterations = abs(x_diff);
        for (int moves = 1; moves <= iterations; ++moves)
        {
            win_.move_on_screen(prev, {func(prev.first), prev.second});
            prev.first = func(prev.first);
            std::this_thread::sleep_for(std::chrono::milliseconds(speed_));
        }
    }

    void move_vertically(std::pair<size_t, size_t>& prev,
                         const std::pair<size_t, size_t> next)
    {
        int y_diff      = prev.second - next.second;
        auto func       = utils::get_operator_for_sign(y_diff);
        auto iterations = abs(y_diff);
        for (int moves = 1; moves <= iterations; ++moves)
        {
            win_.move_on_screen(prev, {prev.first, func(prev.second)});
            prev.second = func(prev.second);
            std::this_thread::sleep_for(std::chrono::milliseconds(speed_));
        }
    }
};