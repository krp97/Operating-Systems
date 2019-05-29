#include "../include/flight_generator.hpp"

Flight_Generator::Flight_Generator(Window& win, Control_Tower& ct,
                                   float out_in_ratio,
                                   std::chrono::milliseconds frequency)
    : win_ {win}, ct_ {ct}, out_in_ratio_ {out_in_ratio}, frequency_ {frequency}
{
    generate_loop();
}

void Flight_Generator::generate_loop()
{
    ct_.create_flight(std::make_unique<Outgoing_Airplane>(
        std::chrono::milliseconds(50), win_));
    ct_.create_flight(std::make_unique<Outgoing_Airplane>(
        std::chrono::milliseconds(50), win_));
    /*
    while (!ct_.should_shutdown())
    {
        int outgoing_planes = 1, incoming_planes = 1;
        if (out_in_ratio_ > 1)
            outgoing_planes = static_cast<int>(out_in_ratio_);
        else
            incoming_planes = static_cast<int>(1 / out_in_ratio_);

        for (; outgoing_planes > 0; --outgoing_planes)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            ct_.create_flight(std::make_unique<Outgoing_Airplane>(
                std::chrono::milliseconds(frequency_), win_, win_.HANGAR_OUT));
        }
        for (; incoming_planes > 0; --incoming_planes)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2000));
            ct_.create_flight(std::make_unique<Incoming_Airplane>(
                std::chrono::milliseconds(frequency_), win_, win_.HANGAR_OUT));
        }
    }*/
}