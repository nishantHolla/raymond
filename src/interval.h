#ifndef INTERVAL_H_
#define INTERVAL_H_

// ==============================
// Interval class
// ==============================

class Interval {
  public:
    double min, max; // range of the interval

    /*
     * Constructs the interval object with -infinity and +infinity as the range if no argument is passed.
     */
    Interval() :
      min(+infinity),
      max(-infinity) {
      }

    /*
     * Constructs the interval object with the passed min and max range.
     */
    Interval(double min, double max) :
      min(min),
      max(max) {
      }

    /*
     * Constructs the interval object tightly enclosing the two given intervals.
     */
    Interval(const Interval &a, const Interval &b) {
      min = std::fmin(a.min, b.min);
      max = std::fmax(a.max, b.max);
    }

    /*
     * Returns the size of the interval
     */
    double size() const {
      return max - min;
    }

    /*
     * Checks if a given point is part of the current interval. x belongs to [min, max]
     */
    bool contains(double x) const {
      return min <= x && x <= max;
    }

    /*
     * Checks if a given point is inside the current interval. ie. x belongs to (min, max)
     */
    bool surrounds(double x) const {
      return min < x && x < max;
    }

    /*
     * Clamps the given point within the current interval.
     */
    double clamp(double x) const {
      if (x < min) {
        return min;
      }

      if (x > max) {
        return max;
      }

      return x;
    }

    /*
     * Expand current interval by gven delta amount and return new interval.
     */
    Interval expand(double delta) const {
      double padding = delta / 2;
      return Interval(min - padding, max + padding);
    }

    static const Interval empty; // Interval that contains no points
    static const Interval universe; // Interval that contains all the points
};

// Initializing empty and universe interval

const Interval Interval::empty = Interval(+infinity, -infinity);
const Interval Interval::universe = Interval(-infinity, +infinity);

#endif //!INTERVAL_H_
