#include <csetjmp>
#include <csignal>
#include <cstring>  // strsignal
#include <iostream>

// sigsetjmp
// siglongjmp

void segmentation_fault() { *(int *)0x01 = 0; }

void signal_handler(int signal) { std::cout << "Signal " << strsignal(signal) << " received, ignore it." << std::endl; }

void signal_handler2(int signal) {
    std::cout << "Signal " << strsignal(signal) << " received, throw something..." << std::endl;
    throw 0;
}

std::jmp_buf jump_buffer;

void signal_handler3(int signal) {
    std::cout << "Signal " << strsignal(signal) << " received, longjmp..." << std::endl;
    std::longjmp(jump_buffer, 1);
}

void signal_handler4(int signal) {
    std::cout << "signal_handler4" << std::endl;
    // std::signal(signal, SIG_DFL);  // set default handler
    std::signal(signal, SIG_IGN);  // ignore signal
}

int main() {
    std::signal(SIGSEGV, signal_handler);
    std::raise(SIGSEGV);

    // loop
    // segmentation_fault();

    // can not catch
    // try {
    //     std::signal(SIGSEGV, signal_handler2);
    //     segmentation_fault();
    // } catch (...) {
    //     std::cout << "Catch something." << std::endl;
    // }

#if 0
    std::signal(SIGSEGV, signal_handler3);
    if (setjmp(jump_buffer) == 0) {
        segmentation_fault();
    } else {
        std::cout << "return by longjmp" << std::endl;
    }

    std::signal(SIGSEGV, signal_handler4);
    // no loop
    segmentation_fault();
#endif
}
