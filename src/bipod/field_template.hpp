#ifndef BIPOD_FIELD_TEMPLATE_HPP
#define BIPOD_FIELD_TEMPLATE_HPP

#include <boost/optional.hpp>

namespace bipod {

template <class T>
struct field_template{
	using type = T;
	boost::optional<T> value;
	field_template() : value(boost::none) {};
	field_template(T val) : value(val) {};
};

}// namespace bipod

#endif //BIPOD_FIELD_TEMPLATE_HPP