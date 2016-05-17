#ifndef BIPOD_DETAIL_SNAPSHOT_HPP
#define BIPOD_DETAIL_SNAPSHOT_HPP

#include "bipod/connection.hpp"

#include <boost/optional.hpp>
#include <boost/mpl/set.hpp>
#include <type_traits>
#include <tuple>

namespace bipod {

template<class T>
class snapshot {
	static_assert(sizeof(T) == 0, "Invalid template argument; excepted bipod::connection.");
};

template<class ResourceType, class... ReadPermissions, class... WritePermissions, class... ExecutePermissions>
class snapshot<bipod::connection<ResourceType, std::tuple<ReadPermissions...>, std::tuple<WritePermissions...>, std::tuple<ExecutePermissions...>>> {
	
	using ConnectionType = bipod::connection<ResourceType, std::tuple<ReadPermissions...>, std::tuple<WritePermissions...>, std::tuple<ExecutePermissions...>>;
	using inbox_t = std::tuple<ReadPermissions...>;
	using outbox_t = std::tuple<WritePermissions...>;
public:
	snapshot(ConnectionType& conn) 
		: conn_(conn)
		, inbox_(conn.Get<ReadPermissions...>())	{ }

	~snapshot() {
		conn_.Set(outbox_);
	}
	
	template<class Field, class = typename std::enable_if<detail::is_in<Field, std::tuple<WritePermissions...>>::value>::type>
	void Set(typename Field::type val) {
		std::get<Field>(outbox_).value = optional<typename Field::type>(val);
	}
	
	template<class Field>
	optional<typename Field::type> Get(typename std::enable_if<detail::is_in<Field, std::tuple<ReadPermissions...>>::value>::type* = nullptr) {
		return std::get<Field>(inbox_).value;
	}


	template<class... Fields>
	optional<std::tuple<Fields...>> Get(typename std::enable_if<(sizeof...(Fields) > 1) && detail::are_in<std::tuple<Fields...>, std::tuple<ReadPermissions...>>::value>::type* = nullptr) {
		if (are_true(std::get<Fields>(inbox_).value...)) {
			return boost::make_optional(std::make_tuple(std::get<Fields>(inbox_)...));
		}
		return boost::none;
	}

	/*
	template<class Field, class = typename std::enable_if<detail::is_in<Field, std::tuple<ReadPermissions...>>::value>::type>
	optional<typename Field::type> Get() {
		return std::get<Field>(inbox_).value;
	}

	
	template<class... Fields, class = typename std::enable_if<(sizeof...(Fields) > 1) && detail::are_in<std::tuple<Fields...>, std::tuple<ReadPermissions...>>::value>::type>
	optional<std::tuple<Fields...>> Get() {
		return boost::none;
	}
	*/
	
	
private:
	ConnectionType& conn_;
	const inbox_t inbox_;
	outbox_t outbox_;

	template<class Head>
	bool are_true(Head head) {
		return static_cast<bool>(head);
	}

	template<class Head, class... Tail>
	bool are_true(Head head, Tail... tail) {
		return static_cast<bool>(head) && are_true(tail...);
	}


};

} // end namespace bipod

#endif // BIPOD_DETAIL_SNAPSHOT_HPP