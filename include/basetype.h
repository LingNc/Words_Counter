#ifndef BASETYPE_H
#define BASETYPE_H

#include "pair.hpp"
#include "sqlist.hpp"
#include <string>

using WordItem = Pair<std::string, size_t>;
using FreqTable = SqList<WordItem>;

#endif