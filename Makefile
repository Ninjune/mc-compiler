CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -g
SRCS = $(wildcard src/*.cpp)
HDRS = $(wildcard src/*.h)

mcc: $(SRCS) $(HDRS)
	$(CXX) $(CXXFLAGS) -o $@ $(SRCS)

#.PHONY: test clean
.PHONY: clean
#test: mcc
#	bash run_tests.sh ./mcc ../../public-tests

clean:
	rm -f mcc
