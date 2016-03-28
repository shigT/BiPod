#ifndef BIPOD_IPC_MANAGED_SEGMENT_HPP
#define BIPOD_IPC_MANAGED_SEGMENT_HPP

#include "detail/for_each_tuple.hpp"

#include <boost/interprocess/sync/sharable_lock.hpp>
#include <boost/interprocess/sync/named_sharable_mutex.hpp>


namespace bipod {

// tags
struct remove_on_destruct_t {};
struct leave_on_destruct_t {};


namespace ipc = boost::interprocess;

template <class ManagedMemorySegment, class on_destruct>
class ipc_managed_segment{
	using ipc_managed_segment_instance_t = ipc_managed_segment<ManagedMemorySegment, on_destruct>;
	using Mutex = ipc::named_sharable_mutex;

public:
	ipc_managed_segment(const std::string& name, size_t size)
		: segment_name_(std::string("bipod_ipc_managed_segment_").append(name))
		, mutex_name_(std::string("bipod_ipc_mutex_").append(name))
		, segment_(ipc::open_or_create, segment_name_.c_str(), size)
		, mutex_(ipc::open_or_create, mutex_name_.c_str())
		, remover_(*this)
	{ }

	template<class... Fields>
	std::tuple<Fields...> Get() {
		ipc::sharable_lock<ipc::named_sharable_mutex> lock(mutex_);
		return std::make_tuple(GetField<Fields>()...);
	}

	template<class... Fields>
	void Set(std::tuple<Fields...> val) {
		ipc::scoped_lock<ipc::named_sharable_mutex> lock(mutex_);
		detail::for_each_tuple <SetField> (val, &segment_);
	}

	template<class... Fields>
	void Dispatch(std::function<void(const typename Fields::type*...)> func) {
		ipc::scoped_lock<ipc::named_sharable_mutex> lock(mutex_);
		func(segment_.find<typename Fields::type>(Fields::name).first...);
	}

	template<class... Fields>
	void Dispatch(std::function<void(typename Fields::type*...)> func) {
		ipc::scoped_lock<ipc::named_sharable_mutex> lock(mutex_);
		func(segment_.find_or_construct<typename Fields::type>(Fields::name)()...);
	}

private:
	template <class Field>
	Field GetField() {
		if (auto ptr = segment_.find<typename Field::type>( Field::name ).first ) {
			Field f;
			f.value = *ptr;
			return f;
		}else{
			return Field();
		}
	}

	template<class Field>
	struct SetField {
		void operator()(Field f, ManagedMemorySegment* seg) {
			if (! f.value) { return; }
			if (auto res = seg->find_or_construct<typename Field::type>(Field::name)()) {
				*res = *(f.value);
			}
			else {
				throw std::exception("Out of segment");
			}
		}
	};


private:
	const std::string segment_name_;
	const std::string mutex_name_;
	ManagedMemorySegment segment_;
	Mutex mutex_;
	
	template<class Segment, class tag>
	struct remover {
		remover(ipc_managed_segment_instance_t&) {}
	};

	template<>
	struct remover<ipc::managed_shared_memory, remove_on_destruct_t> {
		remover(ipc_managed_segment_instance_t& parent) : parent_(parent) {}
		~remover() {
			ipc::shared_memory_object::remove(parent_.segment_name_.c_str());
			parent_.mutex_.remove(parent_.mutex_name_.c_str());
		}
	private:
		ipc_managed_segment_instance_t& parent_;
	};

	remover<ManagedMemorySegment, on_destruct> remover_;
	
};

} // end namespace bipod

#endif //BIPOD_IPC_MANAGED_SEGMENT_HPP