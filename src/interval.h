#ifndef INTERVAL_H
#define INTERVAL_H

class interval
{
    public:
        double min, max;

        interval(): min(+infinity), max(-infinity) {}

        interval(double min, double max): min(min), max(max) {}

        double size() const
        {
            return max-min;
        }

        bool constains(double n) const
        {
            return min <= n && n <= max;
        }

        bool surrounds(double n) const
        {
            return min < n && n < max;
        }

        double clamp(double n) const
        {
            if(n < min) return min;
            if(n > max) return max;
            return n;
        }

        static const interval empty, universe;
};

const interval interval::empty      = interval(+infinity, -infinity);
const interval interval::universe   = interval(-infinity, +infinity);

#endif