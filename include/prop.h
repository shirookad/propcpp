
#ifndef __PROPCPP_PROP_H__
#define __PROPCPP_PROP_H__

#include <functional>

#define WILL_SET(new_value_name) [this](auto new_value_name)
#define DID_SET(old_value_name) [this](auto old_value_name)
#define GET [this]()

namespace prop {
	enum visibility : int {
		vprivate = 0,
		vprotected = 1,
		vpublic = 2
	};
	
	template <class T>
	class property {
		T value;
	
	public:
		constexpr inline property() {  }
		constexpr inline property(const T &val) : value(val) {  }
		
		constexpr inline T &operator = (const T &newValue) { return value = newValue; }
		constexpr inline operator T () const { return value; }
		
		constexpr property(const property<T> &) = delete;
		constexpr property& operator = (const property<T> &) = delete;	
	};
	
	template <class T>
	class observable_property {
		T value;
		std::function<void (const T &)> willSet, didSet;
	
	public:
		constexpr inline observable_property(const std::function<void (const T &)> &_willSet, const std::function<void (const T &)> &_didSet) :
			willSet(_willSet), didSet(_didSet) {  }
		
		constexpr inline T &operator = (const T &newValue) {
			willSet(newValue);
			T oldValue { value };
			value = newValue;
			didSet(oldValue);
			return value;
		}
		constexpr inline operator T () const { return value; }

		constexpr observable_property(const observable_property<T> &) = delete;
		constexpr observable_property& operator = (const observable_property<T> &) = delete;	
	};
	
	template <class T>
	class computed_property {
		std::function<T ()> computeFunction;
	
	public:
		constexpr inline computed_property(const std::function<T ()> &_computeFunction) :
			computeFunction(_computeFunction) {  }
		
		constexpr inline operator T () const { return computeFunction(); }
		
		constexpr computed_property(const computed_property<T> &) = delete;
		constexpr computed_property& operator = (const computed_property<T> &) = delete;	
	};
}

#endif
