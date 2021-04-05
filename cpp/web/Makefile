OBJS := common.o tcp.o http.o
SRCS := $(wildcard example/*.cpp)
EXAMPLE := $(patsubst example/%.cpp, %, $(SRCS))
INCLUDE_DIR	:= $(shell pwd)
CPPFLAGS = -g -Wall -Wextra -Werror -I$(INCLUDE_DIR)

all:$(OBJS) $(EXAMPLE)

%.o: %.cpp %.h common.hpp
	g++ $(CPPFLAGS) -c -o $@ $<

%: example/%.cpp $(OBJS)
	g++ $(CPPFLAGS) -o $@ $< $(OBJS) 

clean:
	rm -f *.o $(EXAMPLE)
