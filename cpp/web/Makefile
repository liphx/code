OBJS := common.o tcp.o http.o
CPPFLAGS = -g -Wall -Wextra -Werror

SRCS := $(wildcard example/*.cpp)
EXAMPLE := $(patsubst example/%.cpp,%,$(SRCS))

all:$(OBJS) $(EXAMPLE)

%.o: %.cpp %.h common.hpp
	g++ $(CPPFLAGS) -c -o $@ $<

%: example/%.cpp $(OBJS)
	g++ $(CPPFLAGS) -o $@ $< $(OBJS)

clean:
	rm -f *.o $(EXAMPLE)
