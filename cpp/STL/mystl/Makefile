SRCS := $(wildcard *.hpp)
TEST_SRCS := $(wildcard *.cpp)
OBJS := $(patsubst %cpp,%o,$(TEST_SRCS))
CPPFLAGS = -g -Wall -Wextra -Werror

all:test

test_%.o: test_%.cpp %.hpp
	g++ $(CPPFLAGS) -c -o $@ $<

test:$(OBJS)
	g++ $(CPPFLAGS) -o $@ $^ -lgtest -lgtest_main -lpthread

clean:
	rm *.o test
