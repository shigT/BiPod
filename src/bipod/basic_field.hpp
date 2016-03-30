#ifndef BIPOD_BASIC_FIELD_HPP
#define BIPOD_BASIC_FIELD_HPP

#include <boost/optional.hpp>

namespace bipod {

template <class T>
struct basic_field{
	using type = T;
	boost::optional<T> value;
	basic_field() : value(boost::none) {};
	basic_field(T val) : value(val) {};
};

}// namespace bipod

#endif //BIPOD_BASIC_FIELD_HPP