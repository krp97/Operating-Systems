#include "../include/flight_generator.hpp"

Flight_Generator::Flight_Generator(Window& win, Control_Tower& ct,
                                   float out_in_ratio,
                                   std::chrono::milliseconds frequency)
    : win_ {win}, ct_ {ct}, out_in_ratio_ {out_in_ratio}, frequency_ {frequency}
{
    f_generator_th_ = std::thread(&Flight_Generator::generate_loop, this);
}

void Flight_Generator::generate_loop()
{
    while (!ct_.should_shutdown())
    {
        int outgoing_planes = 1, incoming_planes = 1;
        if (out_in_ratio_ > 1)
            outgoing_planes = static_cast<int>(out_in_ratio_);
        else
            incoming_planes = static_cast<int>(1 / out_in_ratio_);

        for (; outgoing_planes > 0 && !ct_.should_shutdown(); --outgoing_planes)
        {
            ct_.create_flight(std::make_unique<Outgoing_Airplane>(
                std::chrono::milliseconds(50), win_));
            wait();
        }
        for (; incoming_planes > 0 && !ct_.should_shutdown(); --incoming_planes)
        {
            ct_.create_flight(std::make_unique<Incoming_Airplane>(
                std::chrono::milliseconds(50), win_));
            wait();
        }
    }
}

void Flight_Generator::wait()
{
    for (int i = 0; i < 30 && !ct_.should_shutdown(); ++i)
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
}