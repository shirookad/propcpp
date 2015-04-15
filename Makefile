all: test/main.cpp
	clang++ -std=c++14 -stdlib=libc++ -Iinclude test/main.cpp