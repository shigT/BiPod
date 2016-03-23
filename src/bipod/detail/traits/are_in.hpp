#ifndef BIPOD_DETAIL_TRAIT_ARE_IN_HPP
#define BIPOD_DETAIL_TRAIT_ARE_IN_HPP

#include "is_in.hpp"

namespace bipod {
namespace detail {

template<class T, class, class=void, class=void>
struct are_in{
	static_assert(sizeof(T)==0, "Invalid template argument; two std::tuple(s) are required.");
};

template<class Subject_head, class... Subjects_tail, class... Indexes>
struct are_in<
	std::tuple<Subject_head, Subjects_tail...>
	, std::tuple<Indexes...>
	, typename std::enable_if<! is_in<Subject_head, std::tuple<Indexes...> >::value >::type
	> : std::false_type{ };

template<class Subject_head, class... Subjects_tail, class... Indexes>
struct are_in<
	std::tuple<Subject_head, Subjects_tail...>
	, std::tuple<Indexes...>
	, void
	, typename std::enable_if<is_in<Subject_head, std::tuple<Indexes...> >::value >::type
	> : are_in<std::tuple<Subjects_tail...>, std::tuple<Indexes...>>{ };

template<class... Indexes>
struct are_in<
	std::tuple<>
	, std::tuple<Indexes...>
	> : std::true_type { };

} // namespace detail
} // namespace bipod

#endif //BIPOD_DETAIL_TRAIT_ARE_IN_HPP