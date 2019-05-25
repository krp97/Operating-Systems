#include "airplane.hpp"
#include "window.hpp"

class Outgoing_Airplane : public Airplane
{
   public:
    Outgoing_Airplane() = delete;
    Outgoing_Airplane(std::chrono::milliseconds speed, const Window& win,
                      std::pair<size_t, size_t> pos);
    Outgoing_Airplane(const Outgoing_Airplane&) = delete;
    Outgoing_Airplane& operator=(const Outgoing_Airplane&) = delete;
    Outgoing_Airplane(Outgoing_Airplane&&)                 = default;
    Outgoing_Airplane& operator=(Outgoing_Airplane&&) = default;
    ~Outgoing_Airplane();

    void start_action() override;
    Airplane::Airplane_Type get_action_type() const override;

   private:
};