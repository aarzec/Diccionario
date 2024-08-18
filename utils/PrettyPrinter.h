#ifndef PRETTYPRINTER_H
#define PRETTYPRINTER_H

#include <iostream>
#include <string>
#include <locale>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "ConsoleColor.h"

class PrettyPrinter {
    public:
        enum MessageType {
            INFO,
            WARNING,
            SUCCESS,
            PPERROR,
            PPDEBUG
        };
        PrettyPrinter();
        static void print(const std::wstring& message, MessageType type, bool clearConsole = false, bool pause = true);

    private:
        bool clearConsole;
};

#endif
