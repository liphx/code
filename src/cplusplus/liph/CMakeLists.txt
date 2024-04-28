add_library(liph
    coroutine.cpp
    crypto/md5.cpp
    csv.cpp
    file.cpp
    io/poll.cpp
    json.cpp
    net/requests.cpp
    net/socket.cpp
    numeric/big_integer.cpp
    numeric/random.cpp
    string.cpp
)

target_include_directories(liph PUBLIC "${PROJECT_SOURCE_DIR}")
