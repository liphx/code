CPP     = g++ -g -std=c++20 -W -Wall -Wextra -Wno-ignored-qualifiers -Werror -Wfatal-errors \
              -I../bazel-cplusplus/external/thirdparty/googletest-1.14.0/googletest/include
LTL     = -iquote include
STD     = -Dltl=std
LDFLAGS = -lgtest -lgtest_main -L../bazel-bin/external/thirdparty/googletest-1.14.0
TESTS   = $(wildcard unittests/*_test.cpp)

.PHONY: all
all: test_all test_std_all

test_all: $(TESTS)
	$(CPP) $(LTL) -o $@ $^ $(LDFLAGS)

test_std_all: $(TESTS)
	$(CPP) $(STD) -o $@ $^ $(LDFLAGS)

%_test: unittests/%_test.cpp
	$(CPP) $(LTL) -o $@ $^ $(LDFLAGS)

%_std_test: unittests/%_test.cpp
	$(CPP) $(STD) -o $@ $^ $(LDFLAGS)

.PHONY: clean
clean:
	rm -rf *.o *_test *.dSYM a.out test_all test_std_all

.PHONY: format
format:
	clang-format -i unittests/* include/detail/*
