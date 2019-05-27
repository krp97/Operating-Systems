#pragma once

#include <chrono>
#include <functional>
#include <thread>
#include "../src/utils.cpp"
#include "priority.hpp"
#include "route.hpp"
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
    Airplane(std::chrono::milliseconds speed, Window& win, Route route,
             Priority p)
        : speed_ {speed},
          win_ {win},
          route_ {route},
          priority_ {p},
          position_ {route.start_} {};

    virtual ~Airplane() = default;

    // Take off or Landing, depending on the base class implementation.
    virtual void start_action()            = 0;  // Better name ?
    virtual Action get_action_type() const = 0;

    bool Airplane::operator>(const Airplane& b) const
    {
        return this->priority_.get_priority() > b.priority_.get_priority();
    };

   protected:
    std::chrono::milliseconds speed_;
    Window& win_;
    std::pair<size_t, size_t> position_;
    Priority priority_;
    Route route_;
};