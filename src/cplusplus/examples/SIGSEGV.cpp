#include <string.h>  // strsignal

#include <csetjmp>
#include <csignal>
#include <iostream>

// sigsetjmp
// siglongjmp

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
    /* std::signal(signal, SIG_DFL);  // 信号处理函数被设为默认信号处理函数 */
    std::signal(signal, SIG_IGN);  // 忽略信号
}

int main() {
    std::signal(SIGSEGV, signal_handler);
    std::raise(SIGSEGV);

    // loop
    /* *(int *)0 = 0; */

    // can not catch
    /* try { */
    /*     std::signal(SIGSEGV, signal_handler2); */
    /*     *(int *)0 = 0; */
    /* } catch (...) { */
    /*     std::cout << "Catch something." << std::endl; */
    /* } */

#if 0
    std::signal(SIGSEGV, signal_handler3);
    if (setjmp(jump_buffer) == 0) {
        *(int *)0x01 = 0;
    } else {
        std::cout << "return by longjmp" << std::endl;
    }

    std::signal(SIGSEGV, signal_handler4);
    // no loop
    *(int *)0x01 = 0;
#endif
}
