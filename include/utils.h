//
// Created by csq on 2/7/23.
//

#ifndef META_UTILS_H
#define META_UTILS_H

#include <type_traits>

template <typename T>
using RemConstRef = std::remove_cv_t<std::remove_reference_t<T>>;

#endif // META_UTILS_H
