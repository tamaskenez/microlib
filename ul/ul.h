#pragma once

#include <iterator>

#define BE(X) begin(X), end(X)
#define FOR(VAR, FROM, TO) for (auto VAR = (FROM); VAR < (TO); ++VAR)
#define FORNEQ(VAR, FROM, TO) for (auto VAR = (FROM); VAR != (TO); ++VAR)
#define FORLEQ(VAR, FROM, TO) for (auto VAR = (FROM); VAR <= (TO); ++VAR)
#define FORGEQ(VAR, FROM, TO) for (auto VAR = (FROM); VAR >= (TO); --VAR)
#define FORBE(VAR, X) for (auto VAR = begin(X); VAR != end(X); ++VAR)

namespace ul {
}
