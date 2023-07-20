#include "liph/io/poll.h"

#include <fcntl.h>
#include <sys/stat.h>

#include "gtest/gtest.h"

TEST(poll, DISABLED_poll) {
    liph::io::poll_base *poll = new liph::io::poll();
    liph::io::descriptor fd(::open("111", O_RDONLY));
    EXPECT_TRUE(poll->add(fd, liph::io::event_type::read, nullptr));
    std::vector<liph::io::poll_event> events;
    /* int r = poll->wait(events, -1);  // 10s */
    int r = poll->wait(events, 10 * 1000 * 1000);  // 10s
    if (r > 0) {
        char buffer[1024];
        for (auto& event : events) {
            EXPECT_EQ(event.type, liph::io::event_type::read);
            liph::io::descriptor fd(event.fd);
            auto size = fd.read(buffer, sizeof(buffer));
            std::cout << size << ": " << buffer;
        }
    } else {
        std::cout << "r=" << r << ",errno=" << errno << std::endl;
    }
    delete poll;
}
