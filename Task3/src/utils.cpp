#include <functional>

namespace utils {
inline std::function<size_t(size_t)> get_operator_for_sign(int val)
{
    if (val > 0)
        return [](size_t value) -> size_t { return value - 1; };
    else
        return [](size_t value) -> size_t { return value + 1; };
}
}  // namespace utils