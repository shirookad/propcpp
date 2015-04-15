all: test/main.cpp
	g++ -std=c++14 -stdlib=libc++ -Iinclude test/main.cpp