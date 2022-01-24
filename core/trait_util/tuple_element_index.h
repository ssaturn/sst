#pragma once

#include <tuple>


namespace sst::trait_util
{
    template <class T, class Tuple>
    struct tuple_element_index;

    template <class T, class... Types>
    struct tuple_element_index<T, std::tuple<T, Types...>> {
        static constexpr std::size_t value = 0;
    };

    template <class T, class U, class... Types>
    struct tuple_element_index<T, std::tuple<U, Types...>> {
        static const std::size_t value = 1 + tuple_element_index<T, std::tuple<Types...>>::value;
    };
}