#pragma once
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstdio>

class HostTerminal {
public:
    HostTerminal() {
        tcgetattr(STDIN_FILENO, &orig_);
        termios t = orig_;
        t.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &t);
        int flags = fcntl(STDIN_FILENO, F_GETFL, 0);
        fcntl(STDIN_FILENO, F_SETFL, flags | O_NONBLOCK);
    }
    ~HostTerminal() { tcsetattr(STDIN_FILENO, TCSANOW, &orig_); }

    static int read_char_nonblock() { return ::getchar(); }

private:
    termios orig_{};
};
