#ifndef LIPH_COROUTINE_H_
#define LIPH_COROUTINE_H_

// C++ version of https://github.com/cloudwu/coroutine

#ifdef __APPLE__
#define _XOPEN_SOURCE
#endif
#include <ucontext.h>

#include <cstddef>
#include <cstring>
#include <functional>
#include <memory>
#include <vector>

namespace liph {

class coroutine;

class processor {
public:
    static const size_t STACKSIZE = 1024 * 1024;  // 1M

public:
    int add(std::shared_ptr<coroutine> co);  // return coroutine id, -1 on failure
    void resume(int id);
    void yield();
    int status(int id) const;

private:
    static void run(processor *p);

private:
    char stack_[STACKSIZE];
    ucontext_t main_;
    int running_id_{-1};
    std::vector<std::shared_ptr<coroutine>> co_list_;
};

class coroutine {
public:
    enum { DEAD = 0, READY = 1, RUNNING = 2, SUSPEND = 3 };

    friend class processor;

public:
    coroutine(std::function<void(void *)> entry, void *user_data) : entry_(entry), user_data_(user_data) {}

    int status() const { return status_; }

private:
    void save_stack(char *top);

private:
    std::function<void(void *)> entry_;
    void *user_data_{nullptr};
    ucontext_t ctx_;
    ptrdiff_t cap_{0};
    ptrdiff_t size_{0};
    int status_{READY};
    char *stack_{nullptr};
};

}  // namespace liph

#endif  // LIPH_COROUTINE_H_
