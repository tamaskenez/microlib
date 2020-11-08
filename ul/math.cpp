#include "ul/math.h"

namespace ul {

void Statistics::reset()
{
    count_ = 0;
    sum_ = sum2_ = 0.0;
    bDirty = false;
    lower_ = upper_ = NAN;
}

void Statistics::update() const
{
    if (count_ > 0) {
        mean_ = sum_ / count_;
        var_ = (count_ * sum2_ - square(sum_)) / (square(count_));
        std_ = sqrt(var_);
        if (count_ > 1) {
            var_sample_ =
                (count_ * sum2_ - square(sum_)) / (count_ * (count_ - 1));
            std_sample_ = sqrt(var_sample_);
        } else
            std_sample_ = var_sample_ = NAN;
    } else
        mean_ = std_ = std_sample_ = var_ = var_sample_ = NAN;

    bDirty = false;
}

}  // namespace ul
