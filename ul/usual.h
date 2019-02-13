#pragma once

#include <array>
#include <cassert>
#include <deque>
#include <memory>
#include <string>
#include <tuple>
#include <vector>

#include "ul/check.h"
#include "ul/inlinevector.h"
#include "ul/preproc.h"
#include "ul/string_par.h"
#include "ul/ul.h"

namespace ul {
template <class T>
using up = std::unique_ptr<T>;

using std::array;
using std::deque;
using std::make_pair;
using std::make_tuple;
using std::make_unique;
using std::move;
using std::pair;
using std::string;
using std::tie;
using std::tuple;
using std::vector;

template <typename T>
using A2 = array<T, 2>;
template <typename T>
using A3 = array<T, 3>;
template <typename T>
using A4 = array<T, 4>;
using AF2 = A2<float>;
using AF3 = A3<float>;
using AF4 = A4<float>;
using AD2 = A2<double>;
using AD3 = A3<double>;
using AD4 = A4<double>;
using AI2 = A2<int>;
using AI3 = A3<int>;
using AI4 = A4<int>;
using VI = vector<int>;
using VF = vector<float>;
using VD = vector<double>;

template <class T>
int operator~(const std::vector<T>& v)
{
    return (int)v.size();
}

template <class T>
int operator~(const std::deque<T>& v)
{
    return (int)v.size();
}

}  // namespace ul
