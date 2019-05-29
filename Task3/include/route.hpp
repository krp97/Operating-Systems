
#include <stdlib.h>
#include <utility>

class Route
{
   private:
    using Point = std::pair<size_t, size_t>;

   public:
    Point start_;
    Point passenger_area_;
    Point runway_start_;
    Point end_;
    Route() = default;
    Route(Point start, Point passenger_area, Point runway_start, Point end)
        : start_ {start},
          passenger_area_ {passenger_area},
          runway_start_ {runway_start},
          end_ {end}
    {
    }

    Route(const Route&) = default;
    Route& operator=(const Route&) = default;
};