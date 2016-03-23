#ifndef BIPOD_DETAIL_FOR_EACH_TUPLE_HPP
#define BIPOD_DETAIL_FOR_EACH_TUPLE_HPP

#include <tuple>
#include <utility>

namespace bipod {
namespace detail {

template <template<class>class Functor, size_t Pos>
struct for_each_tuple_impl {
	template<class Tuple>
	void operator()(Tuple& t) {
		for_each_tuple_impl<Functor, Pos - 1>()(t);
		Functor<typename std::tuple_element<Pos, Tuple>::type>()(std::get<Pos>(t));
	}
	template<class Tuple, class... Args>
	void operator()(Tuple& t, Args... args) {
		for_each_tuple_impl<Functor, Pos - 1>()(t, args...);
		Functor<typename std::tuple_element<Pos, Tuple>::type>()(std::get<Pos>(t), args...);
	}
};

template <template<class>class Functor>
struct for_each_tuple_impl<Functor, 0> {
	template<class Tuple>
	void operator()(Tuple& t) {
		Functor<typename std::tuple_element<0, Tuple>::type>()(std::get<0>(t));
	}
	template<class Tuple, class... Args>
	void operator()(Tuple& t, Args... args) {
		Functor<typename std::tuple_element<0, Tuple>::type>()(std::get<0>(t), args...);
	}
};

template<template<class>class Functor, class Tuple>
void for_each_tuple(Tuple& t) {
	for_each_tuple_impl<Functor, std::tuple_size<Tuple>::value - 1>()(t);
}

template<template<class>class Functor, class Tuple, class... Args>
void for_each_tuple(Tuple& t, Args... args) {
	for_each_tuple_impl<Functor, std::tuple_size<Tuple>::value - 1>()(t, args...);
}

} // end namespace detail
} // end namespace bipod

#endif BIPOD_DETAIL_FOR_EACH_TUPLE_HPP