#ifndef CONSOLECOLOR_H
#define CONSOLECOLOR_H

#include <iostream>

class ConsoleColor {
public:
    static std::wstring BLACK;
    static std::wstring RED;
    static std::wstring GREEN;
    static std::wstring YELLOW;
    static std::wstring BLUE;
    static std::wstring MAGENTA;
    static std::wstring CYAN;
    static std::wstring WHITE;
    static std::wstring GRAY;

    static std::wstring BG_BLACK;
    static std::wstring BG_RED;
    static std::wstring BG_GREEN;
    static std::wstring BG_YELLOW;
    static std::wstring BG_BLUE;
    static std::wstring BG_MAGENTA;
    static std::wstring BG_CYAN;
    static std::wstring BG_WHITE;

    static std::wstring RESET;
};

#endif
