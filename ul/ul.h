#pragma once

#include <deque>
#include <functional>
#include <iterator>
#include <string>
#include <type_traits>
#include <unordered_map>
#include <utility>

#include "ul/preproc.h"

#define BE(X) std::begin(X), std::end(X)
#define FOR(VAR, FROM, OP_TO) for (auto VAR = (FROM); VAR OP_TO; ++(VAR))
#define FORBACK(VAR, FROM, OP_TO) for (auto VAR = (FROM); VAR OP_TO; --(VAR))
#define FORBE(VAR, X) for (auto VAR = begin(X); (VAR) != end(X); ++(VAR))

// Creates zero-overhead scope exit object
// Usage example: ON_SCOPE_EXIT { fclose(); };
#define ON_SCOPE_EXIT                                \
    auto UL_CAT(ul_scope_exit_task__, __COUNTER__) = \
        ::ul::detail::DummyTypeForOnScopeExit{} << [&]() -> void

namespace ul {

// Takes a lambda in ctor, stores in std::function<void()>, execute in dtor.
// Can be modified along lifetime.
struct ScopeExitTask
{
    ScopeExitTask() = default;
    ScopeExitTask(const ScopeExitTask&) = delete;
    explicit ScopeExitTask(std::function<void()>&& f) : task(std::move(f)) {}
    ScopeExitTask(ScopeExitTask&& x) : task(move(x.task)) { x.task = nullptr; }
    ~ScopeExitTask()
    {
        if (task)
            task();
    }

    void operator=(std::function<void()>&& f) { task = move(f); }
    void operator=(std::nullptr_t) { task = nullptr; }

private:
    std::function<void()> task;
};

namespace size_op {

template <class T,
          typename = std::enable_if_t<
              std::is_member_function_pointer_v<decltype(&T::size)>>>
int operator~(const T& v)
{
    return (int)v.size();
}

}  // namespace size_op

namespace detail {

struct DummyTypeForOnScopeExit
{};

template <class F>
struct TemplatedScopeExitTask
{
    explicit TemplatedScopeExitTask(F&& f) : f(f) {}
    TemplatedScopeExitTask(const TemplatedScopeExitTask&) = delete;
    ~TemplatedScopeExitTask() { f(); }

private:
    F f;
};

template <class F>
TemplatedScopeExitTask<typename std::decay<F>::type> operator<<(
    DummyTypeForOnScopeExit,
    F&& f)
{
    return TemplatedScopeExitTask<typename std::decay<F>::type>(
        std::forward<F>(f));
}
}  // namespace detail

}  // namespace ul
