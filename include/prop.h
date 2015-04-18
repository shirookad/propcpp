
#ifndef __PROPCPP_PROP_H__
#define __PROPCPP_PROP_H__

#include <functional>

#define WILL_SET(new_value_name) [this](auto new_value_name)
#define DID_SET(old_value_name) [this](auto old_value_name)
#define GET [this]()

namespace prop {
	template <class T>
	class property {
		using param_t = typename std::conditional<std::is_fundamental<T>::value, T, const T&>::type;
		
		T m_value;
	
	public:
		constexpr inline property() {  }
		constexpr inline property(param_t value) : m_value(value) {  }
		
		constexpr explicit inline operator param_t () const { return m_value; }
		
		inline T& set(param_t value) { return m_value = value; }
		constexpr inline param_t get() const { return m_value; }
		
		constexpr property(const property<T> &) = delete;
		constexpr property(property<T>&&) = delete;
		constexpr property& operator = (const property<T> &) = delete;	
	};

	template <class T>
	class observable_property {
		using observable_property_fnc_t = std::function<void (const T &)>;
		using param_t = typename std::conditional<std::is_fundamental<T>::value, T, const T&>::type;
		
		T m_value;
		const observable_property_fnc_t willSet, didSet;
	
	public:
		constexpr inline observable_property(const observable_property_fnc_t &_willSet, const observable_property_fnc_t &_didSet) :
			willSet(_willSet), didSet(_didSet) {  }
		
		constexpr explicit inline operator param_t () const { return m_value; }

		inline T& set(const param_t value) {
			willSet(value);
			T oldValue = std::move(m_value);
			m_value = value;
			didSet(oldValue);
			return m_value;
		}
		constexpr inline param_t get() const { return m_value; }
		
		constexpr observable_property(const observable_property<T> &) = delete;
		constexpr observable_property(observable_property<T>&&) = delete;
		constexpr observable_property& operator = (const observable_property<T> &) = delete;	
	};
	
	template <class T>
	class computed_property {
		using computed_property_fnc_t = std::function<T ()>;		
		
		const computed_property_fnc_t computeFunction;
	
	public:
		constexpr inline computed_property(const computed_property_fnc_t &_computeFunction) :
			computeFunction(_computeFunction) {  }
		
		constexpr inline operator T () const { return std::move(computeFunction()); }
		constexpr inline T get() const { return std::move(computeFunction()); }
		
		constexpr computed_property(const computed_property<T> &) = delete;
		constexpr computed_property(computed_property<T>&&) = delete;
		constexpr computed_property& operator = (const computed_property<T> &) = delete;	
	};
	
	template <class O, class T>
	class readonly_property {
		using param_t = typename std::conditional<std::is_fundamental<T>::value, T, const T&>::type;
		
		T m_value;
		
		inline T& set(const T& value) { return m_value = value; }
		
		friend O;
		
	public:
		constexpr inline readonly_property(param_t value) : m_value(value) {  }
		
		constexpr explicit inline operator param_t () const { return m_value; }

		constexpr inline param_t get() const { return m_value; }
		
		constexpr readonly_property(const readonly_property<O, T>&) = delete;
		constexpr readonly_property(readonly_property<O, T>&&) = delete;
		constexpr readonly_property<O, T>& operator = (const readonly_property<O, T>&) = delete;
	};
}

#endif

