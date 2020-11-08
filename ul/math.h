#pragma once

// Simple mathematical functions that could be part of standard library
#include <cassert>
#include <cmath>
#include <vector>

#include "ul/check.h"
#include "ul/config.h"
#include "ul/span.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846264338327950288
#endif

namespace ul {

template <class X, class L, class U>
bool within_co(const X& x, const L& lower, const U& upper)
{
    assert(lower <= upper);
    return lower <= x && x < upper;
}

template <class T>
bool is_zero(T x)
{
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wfloat-equal"
#endif
    return x == 0;
#ifdef __clang__
#pragma clang diagnostic pop
#endif
}

template <class T>
constexpr int sign(T x) noexcept
{
    return is_zero(x) ? 0 : (std::signbit(x) ? -1 : 1);
}

template <class T>
T square(T x)
{
    return x * x;
}

template <class T>
T sec(T x)  // secant
{
    return 1 / cos(x);
}

class Statistics
{
public:
    void reset();

    void add(double d)
    {
        if (UL_UNLIKELY(std::isnan(d))) {
            return;
        }
        ++count_;
        sum_ += d;
        sum2_ += square(d);
        if (UL_UNLIKELY(std::isnan(lower_)))
            lower_ = upper_ = d;
        else if (d < lower_)
            lower_ = d;
        else if (d > upper_)
            upper_ = d;
        bDirty = true;
    }

    double mean() const
    {
        update_if_needed();
        return mean_;
    }

    double std() const  // normalized with N
    {
        update_if_needed();
        return std_;
    }
    double std_sample() const  // normalized with N-1
    {
        update_if_needed();
        return std_sample_;
    }

    double var() const  // normalized with N
    {
        update_if_needed();
        return var_;
    }
    double var_sample() const  // normalized with N-1
    {
        update_if_needed();
        return var_sample_;
    }

    int count() const
    {
        CHECK(count_ <= INT_MAX);
        return static_cast<int>(count_);
    }
    int64_t count64() const { return count_; }
    double sum() const { return sum_; }
    double sum2() const { return sum2_; }
    double upper() const { return upper_; }
    double lower() const { return lower_; }
    void update() const;

private:
    void update_if_needed() const
    {
        if (bDirty)
            update();
    }
    double sum_ = 0.0, sum2_ = 0.0;
    int64_t count_ = 0;

    mutable bool bDirty = false;
    mutable double mean_ = NAN;
    mutable double lower_ = NAN, upper_ = NAN;
    mutable double std_, std_sample_, var_, var_sample_;
};

}  // namespace ul
