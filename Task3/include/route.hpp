
#include <stdlib.h>
#include <utility>

class Route
{
   private:
    using Point = std::pair<size_t, size_t>;

   public:
    const Point start_;
    const Point passenger_area_;
    const Point runway_start_;
    const Point end_;

    Route(Point start, Point passenger_area, Point runway_start, Point end)
        : start_ {start},
          passenger_area_ {passenger_area},
          runway_start_ {runway_start},
          end_ {end}
    {
    }
};