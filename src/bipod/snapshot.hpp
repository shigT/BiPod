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
	
	template<class Field>
	void Set(typename Field::type val) {
		std::get<Field>(outbox_).value = optional<typename Field::type>(val);
	}
	
	template<class Field>
	optional<typename Field::type> Get() {
		return std::get<Field>(inbox_).value;
	}
	
private:
	ConnectionType& const conn_;
	const inbox_t inbox_;
	outbox_t outbox_;
	
};

} // end namespace bipod

#endif // BIPOD_DETAIL_SNAPSHOT_HPP