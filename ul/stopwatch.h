#pragma once

#include <cassert>
#include <chrono>

namespace ul {

using std::chrono::duration;
using std::chrono::high_resolution_clock;

// Matlab-like tic/toc:
//
//     double elapsed = 0.0;
//     auto t0 = tic();
//     ...
//     elapsed += toc(t0);

inline high_resolution_clock::time_point tic()
{
    return high_resolution_clock::now();
}

inline double toc(high_resolution_clock::time_point t0)
{
    return duration<double>(tic() - t0).count();
}

// Stopwatch for more serious measurements
//
//     Stopwatch sw(true);
//     ...
//     lap_time = sw.elapsed();
//     ...
//     sw.stop();
//     ...
//     sw.start(); //resume
//     ...
//     sw.stop();
//     time_of_two_sections = sw.elapsed();
template <class Clock = high_resolution_clock>
class Stopwatch
{
public:
    using clock = Clock;
    using time_point = typename clock::time_point;
    using duration = typename clock::duration;
    using duration_dbl = std::chrono::duration<double>;

    explicit Stopwatch(bool start = false)
        : running_since(time_point::max()), elapsed_duration(duration::zero())
    {
        if (start)
            this->start();
    }

    bool running() const { return running_since != time_point::max(); }

    // elapsed time in seconds. Includes both previous measurement intervals and
    // the current one (even if it's still running)
    double elapsed() const
    {
        auto result = elapsed_duration;
        if (running())
            result += clock::now() - running_since;
        return duration_dbl(result).count();
    }

    // Starts/resumes counting the time (does not reset). It's an error to call
    // this on a running stopwatch.
    void start()
    {
        assert(!running());
        running_since = clock::now();
    }

    // Stops counting the time. It's an error to call this on a stopped
    // stopwatch. Returns elapsed time.
    double stop()
    {
        elapsed_duration += clock::now() - running_since;
        assert(running());  // call check only after registering the time
        running_since = time_point::max();
        return duration_dbl(elapsed_duration).count();
    }

    // Starts counting the time, zeroing out the time accumulated so far.
    // Returns the previously elapsed time.
    double restart()
    {
        double result = elapsed();
        elapsed_duration = duration::zero();
        running_since = clock::now();
        return result;
    }

    // Zeroes out time accumulated so far. Also stops watch if running.
    void reset()
    {
        elapsed_duration = duration::zero();
        running_since = time_point::max();
    }

private:
    time_point running_since;
    duration elapsed_duration;
};

}  // namespace ul
