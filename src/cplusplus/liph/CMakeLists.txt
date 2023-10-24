add_library(liph
    coroutine.cpp
    crypto/md5.cpp
    csv.cpp
    file.cpp
    flags.cpp
    io/poll.cpp
    json.cpp
    net/http.cpp
    net/requests.cpp
    net/socket.cpp
    net/tcp.cpp
    numeric/big_integer.cpp
    numeric/random.cpp
    process.cpp
    string.cpp
    terminal.cpp
)

target_include_directories(liph PUBLIC "${PROJECT_SOURCE_DIR}")
