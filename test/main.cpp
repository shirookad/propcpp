#include <iostream>
#include <chrono>
#include <functional>

#include <prop.h>

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
		WILL_SET(newValue) {
			//std::cout << "will set from " << currentValue << " to " << newValue << std::endl;
		},
		DID_SET(oldValue) {
			//std::cout << "did set from " << oldValue << " to " << newValue << std::endl;
		}
	};
	
	prop::computed_property<int> comp {
		GET {
			return 5;
		}
	};
	
	prop::observable_property<std::string> strVal {
		WILL_SET(newValue) { std::cout << "Will set from " << strVal.get() << " to " << newValue << std::endl; },
		DID_SET(oldValue) { std::cout << "Did set from " << oldValue << " to " << strVal.get() << std::endl; }
	};
	
	prop::readonly_property<observerTest, int> intVal { 5 };
	
	observerTest() { intVal = 3; }
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
			t3.count.set(5);
		}
	});

	observerTest t4;
	auto test4nanos = measure_block([&t4]() {
		for (unsigned long long i = 0; i < 1000000; ++i) {
			t4.someOtherVal = 5;
		}
	});
	
	t4.strVal = "Hello";
	t4.strVal.set(t4.strVal.get().append(", World"));
	
	int a = (int)t4.intVal;
	
	std::cout << "test1: " << test1nanos << std::endl;
	std::cout << "test2: " << test2nanos << std::endl;
	std::cout << "test3: " << test3nanos << std::endl;
	std::cout << "test4: " << test4nanos << std::endl;
	
	return 0;	
}
