#include <chrono>
#include <thread>
#include "window.hpp"
class Airplane
{
   public:
    Airplane() = delete;
    Airplane(std::chrono::milliseconds speed, const Window& win,
             std::pair<size_t, size_t> pos)
        : speed_ {speed}, win_ {win}, position_ {pos} {};
    enum class Airplane_Type
    {
        OUTGOING,
        INCOMING
    };

    // Take off or Landing, depending on the base class implementation.
    virtual void start_action()                   = 0;  // Better name ?
    virtual Airplane_Type get_action_type() const = 0;

   protected:
    std::chrono::milliseconds speed_;
    const Window& win_;
    std::thread airplane_th_;
    std::pair<size_t, size_t> position_;
};