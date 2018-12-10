all:dp_alloc
dp_alloc:dp_alloc.cpp os.h memNode.h
	g++ -g $^ -o $@
