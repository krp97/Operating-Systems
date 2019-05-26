#pragma once

#include <chrono>
#include <thread>
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

    virtual void move_horizontally(std::pair<size_t, size_t>& prev,
                                          const std::pair<size_t, size_t> next) = 0;
    virtual void move_vertically(std::pair<size_t, size_t>& prev,
                                 const std::pair<size_t, size_t> next) = 0;
};