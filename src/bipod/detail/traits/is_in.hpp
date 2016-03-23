#ifndef BIPOD_DETAIL_TRAITS_IS_IN_HPP
#define BIPOD_DETAIL_TRAITS_IS_IN_HPP

#include<iostream>
#include<tuple>
#include<type_traits>

namespace bipod {
namespace detail {

template <class T, class>
struct is_in{
	static_assert(sizeof(T)==0, "Invalid template argument; a class and tuple are required.");
};

template <class T>
struct is_in<T, std::tuple<>> : std::false_type{ };

template <class T, class... Tail>
struct is_in<T, std::tuple<T, Tail...>> : std::true_type{ };

template <class T, class Head, class... Tail>
struct is_in<T, std::tuple<Head, Tail...>> : is_in<T, std::tuple<Tail...>>{ };

} // end namespace detail
} // end namespace bipod

#endif // BIPOD_DETAIL_TRAITS_IS_IN_HPP