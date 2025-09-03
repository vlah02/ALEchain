#pragma once
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>
#include <atomic>
#include <stdexcept>

class HostTerminal {
public:
    HostTerminal(const HostTerminal&) = delete;
    HostTerminal& operator=(const HostTerminal&) = delete;

    HostTerminal() {
        bool expected=false;
        if (!active_.compare_exchange_strong(expected,true))
            throw std::runtime_error("HostTerminal already active");
        if (tcgetattr(STDIN_FILENO, &orig_) != 0) { active_.store(false); throw std::runtime_error("tcgetattr failed"); }
        termios t = orig_; t.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    }
    ~HostTerminal() { tcsetattr(STDIN_FILENO, TCSANOW, &orig_); active_.store(false); }

    static int read_char_nonblock() {
        unsigned char c;
        ssize_t n = ::read(STDIN_FILENO, &c, 1);
        return (n == 1) ? static_cast<int>(c) : EOF;
    }
private:
    termios orig_{};
    static inline std::atomic<bool> active_{false};
};
