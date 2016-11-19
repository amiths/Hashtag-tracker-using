all : hashtagcounter
hashtagcounter: main.cpp fib_heap.cpp fib_heap.h
	g++ -std=c++11 main.cpp fib_heap.cpp fib_heap.h -o hashtagcounter
clean:
	rm hashtagcounter