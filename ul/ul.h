#pragma once

#include <functional>
#include <iterator>
#include <utility>

#include "ul/preproc.h"

#define BE(X) std::begin(X), std::end(X)
#define FOR(VAR, FROM, OP_TO) for (auto VAR = (FROM); VAR OP_TO; ++(VAR))
#define FORBACK(VAR, FROM, OP_TO) for (auto VAR = (FROM); VAR OP_TO; --(VAR))
#define FORBE(VAR, X) for (auto(VAR) = begin(X); (VAR) != end(X); ++(VAR))

#define ON_SCOPE_EXIT                                \
    auto UL_CAT(ul_scope_exit_task__, __COUNTER__) = \
        ::ul::detail::dummy_type_for_on_scope_exit{} << [&]

namespace ul {

struct scope_exit_task
{
    scope_exit_task() = default;
    scope_exit_task(const scope_exit_task&) = delete;
    explicit scope_exit_task(std::function<void()>&& f) : task(std::move(f)) {}
    scope_exit_task(scope_exit_task&& x) : task(move(x.task))
    {
        x.task = nullptr;
    }
    ~scope_exit_task()
    {
        if (task)
            task();
    }

    void operator=(std::function<void()>&& f) { task = move(f); }
    void operator=(std::nullptr_t) { task = nullptr; }

private:
    std::function<void()> task;
};

namespace detail {
struct dummy_type_for_on_scope_exit
{};
inline scope_exit_task operator<<(dummy_type_for_on_scope_exit,
                                  std::function<void()>&& f)
{
    return scope_exit_task(std::move(f));
}
}  // namespace detail

}  // namespace ul
