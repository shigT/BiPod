#ifndef BIPOD_DETAIL_TRAIT_ARE_UNIQUE_HPP
#define BIPOD_DETAIL_TRAIT_ARE_UNIQUE_HPP

#include "is_in.hpp"

namespace bipod {
namespace detail {

template<class T, class = void, class = void>
struct are_unique{
	static_assert(sizeof(T)==0, "Invalid template argument; a std::tuple is required.");
};

template<class Head, class ...Tail>
struct are_unique<
	std::tuple<Head, Tail...>
	, typename std::enable_if<is_in<Head, std::tuple<Tail...>>::value>::type
		> : std::false_type{ };

template<class Head, class ...Tail>
struct are_unique<
	std::tuple<Head, Tail...>
	, void
	, typename std::enable_if<!is_in<Head, std::tuple<Tail...>>::value>::type
		> : are_unique<std::tuple<Tail...>> { };

template<>
struct are_unique<std::tuple<>> : std::true_type{ };

} // namespace detail
} // namespace bipod

#endif //BIPOD_DETAIL_TRAIT_ARE_UNIQUE_HPP