#include <iostream>
#include <chrono>
#include <functional>
#include <ctime>

#define WILLSET [this](auto newValue)
#define DIDSET [this](auto oldValue)
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

class test1 {
public:
	int count;
};

class test2 {
	int _count;
	
public:
	inline int &setCount(const int &count) { return _count = count; }
	constexpr inline int count() const { return _count; }
};

class test3 {
public:
	prop::property<int> count;
};

inline auto measure_block(const std::function<void ()> &f) {
	std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
	start = std::chrono::high_resolution_clock::now();
	f();
	end = std::chrono::high_resolution_clock::now();
	return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

class observerTest {
public:
	prop::observable_property<int> someOtherVal {
		WILLSET {
			//std::cout << "will set from " << currentValue << " to " << newValue << std::endl;
		},
		DIDSET {
			//std::cout << "did set from " << oldValue << " to " << newValue << std::endl;
		}
	};
	
	prop::computed_property<int> comp {
		GET {
			return 5;
		}
	};
};

int main(int argc, char *argv[]) {
	test1 t1;
	auto test1nanos = measure_block([&t1]() {
		for (unsigned long long i = 0; i < 1000000; ++i) {
			t1.count = 5;
		}
	});
	
	test2 t2;
	auto test2nanos = measure_block([&t2]() {
		for (unsigned long long i = 0; i < 1000000; ++i) {
			t2.setCount(5);
		}
	});

	test3 t3;
	auto test3nanos = measure_block([&t3]() {
		for (unsigned long long i = 0; i < 1000000; ++i) {
			t3.count = 5;
		}
	});

	observerTest t4;
	auto test4nanos = measure_block([&t4]() {
		for (unsigned long long i = 0; i < 1000000; ++i) {
			t4.someOtherVal = 5;
		}
	});
	
	std::cout << "test1: " << test1nanos << std::endl;
	std::cout << "test2: " << test2nanos << std::endl;
	std::cout << "test3: " << test3nanos << std::endl;
	std::cout << "test4: " << test4nanos << std::endl;
	
	return 0;	
}