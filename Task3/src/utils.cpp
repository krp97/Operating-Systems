#include <functional>
#include <random>

namespace utils {

inline std::function<size_t(size_t)> get_operator_for_sign(int val)
{
    if (val > 0)
        return [](size_t value) -> size_t { return value - 1; };
    else
        return [](size_t value) -> size_t { return value + 1; };
}

inline int random_int(int a, int b)
{
    static std::default_random_engine generator;
    std::uniform_int_distribution<int> distribution(a, b);
    return distribution(generator);
}

}  // namespace utils