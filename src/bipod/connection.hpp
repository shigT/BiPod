#ifndef BIPOD_CONNECTION
#define BIPOD_CONNECTION


#include "ipc_managed_segment.hpp"
#include "detail/traits/are_in.hpp"

namespace bipod {

namespace{

using boost::optional;
template<class Field>
using pair_t = std::pair<Field, optional<typename Field::type>>;

} // end namespace

template<class T, class, class>
class connection {
	static_assert(sizeof(T) == 0, "Invalid template argument.");
};

template<class ResourceType, class... ReadPermissions, class... WritePermissions>
class connection<ResourceType, std::tuple<ReadPermissions...>, std::tuple<WritePermissions...>> {

public:
	template<class... Args>
	connection(Args... args) : resource_(args...) {}

	template<class... Fields, class = std::enable_if<detail::are_in<std::tuple<Fields...>, std::tuple<WritePermissions...>>::value>::type>
	void Set(typename Fields::type... vals) {
		resource_.Set<Fields...>(std::make_tuple(Gen<Fields>(vals)...));
	}

	template<class... Fields, class = typename std::enable_if<detail::are_in<std::tuple<Fields...>, std::tuple<WritePermissions...>>::value>::type>
	void Set(std::tuple<Fields...> val) {
		resource_.Set(val);
	}
	
	void Set(std::tuple<> val) {
		// nop
	}

	template<class... Fields, class = typename std::enable_if<detail::are_in<std::tuple<Fields...>, std::tuple<ReadPermissions...>>::value>::type>
	std::tuple<Fields...> Get() {
		return resource_.Get<Fields...>();
	}

private:

	template<class Field>
	Field Gen(typename Field::type val) {
		Field f;
		f.value = val;
		return f;
	}

	ResourceType resource_;
};

} // end namespace bipod

#endif //BIPOD_CONNECTION
