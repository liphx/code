#include "liph/coroutine.h"

#ifdef OS_LINUX

#include <cassert>

namespace liph {

int processor::add(std::shared_ptr<coroutine> co) {
    if (co == nullptr) return -1;

    for (int i = 0; i < static_cast<int>(co_list_.size()); i++) {
        if (co_list_[i] == nullptr) {
            co_list_[i] = co;
            return i;
        }
    }

    co_list_.emplace_back(co);
    return co_list_.size() - 1;
}

void processor::run(processor *p) {
    auto& co = p->co_list_[p->running_id_];
    co->entry_(co->user_data_);
    co = nullptr;
    p->running_id_ = -1;
}

void processor::resume(int id) {
    if (id < 0 || (size_t)id >= co_list_.size()) return;

    auto co = co_list_[id].get();
    if (co == nullptr) return;

    switch (co->status()) {
    case coroutine::READY:
        getcontext(&co->ctx_);
        co->ctx_.uc_stack.ss_sp = stack_;
        co->ctx_.uc_stack.ss_size = STACKSIZE;
        co->ctx_.uc_link = &main_;
        running_id_ = id;
        co->status_ = coroutine::RUNNING;
        makecontext(&co->ctx_, (void (*)()) & processor::run, 1, this);
        swapcontext(&main_, &co->ctx_);
        break;
    case coroutine::SUSPEND:
        memcpy(stack_ + STACKSIZE - co->size_, co->stack_, co->size_);
        running_id_ = id;
        co->status_ = coroutine::RUNNING;
        swapcontext(&main_, &co->ctx_);
        break;
    default:
        assert(0);
    }
}

void coroutine::save_stack(char *top) {
    char dummy = 0;
    assert((size_t)(top - &dummy) <= processor::STACKSIZE);
    if (cap_ < top - &dummy) {
        free(stack_);
        cap_ = top - &dummy;
        stack_ = (char *)malloc(cap_);
    }
    size_ = top - &dummy;
    memcpy(stack_, &dummy, size_);
}

void processor::yield() {
    if (running_id_ < 0) return;

    auto co = co_list_[running_id_].get();
    assert((char *)&co > stack_);

    co->save_stack(stack_ + STACKSIZE);
    co->status_ = coroutine::SUSPEND;
    running_id_ = -1;
    swapcontext(&co->ctx_, &main_);
}

int processor::status(int id) const {
    assert(id >= 0 && (size_t)id < co_list_.size());

    if (co_list_[id] == nullptr) return coroutine::DEAD;

    return co_list_[id]->status_;
}

}  // namespace liph

#endif
