all: src/prop.cpp
	clang++ -std=c++14 -stdlib=libc++ src/prop.cpp