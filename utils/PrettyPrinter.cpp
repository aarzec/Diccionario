#include <iostream>
#include <string>
#include <locale>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "ConsoleColor.h"
#include "PrettyPrinter.h"
#include "Utilidades.h"

const bool DEBUG = false;

PrettyPrinter::PrettyPrinter() {
    std::locale::global(std::locale(""));
}

void PrettyPrinter::print(const std::wstring& message, MessageType type, bool clearConsole, bool pause) {
    std::wstring colorCode;
    std::wstring title;

    switch (type) {
    case INFO:
        colorCode = ConsoleColor::BLUE;
        title = L" 🔔 Info ";
        break;
    case WARNING:
        colorCode = ConsoleColor::YELLOW;
        title = L" 🔔 Advertencia ";
        break;
    case SUCCESS:
        colorCode = ConsoleColor::GREEN;
        title = L" ✅ Éxito ";
        break;
    case PPERROR:
        colorCode = ConsoleColor::RED;
        title = L" ❌ Error ";
        break;
    case PPDEBUG: {
        if (DEBUG) {
            std::wcout << ConsoleColor::GRAY << L"Debug: " << message << ConsoleColor::RESET << std::endl;
        }
        return;
    }
    }

    std::wstring resetCode = L"\033[0m";
    std::wstring borderTop = L"╔" + std::wstring(message.length() + 2, L'═') + L"╗";
    std::wstring borderMid = L"╠" + std::wstring(message.length() + 2, L'═') + L"╣";
    std::wstring borderBottom = L"╚" + std::wstring(message.length() + 2, L'═') + L"╝";
    std::wstring emptyLine = L"║" + std::wstring(message.length() + 2, L' ') + L"║";
    std::wstring messageLine = L"║ " + ConsoleColor::WHITE + message + colorCode + L" ║";

    std::wcout << colorCode;
    std::wcout << borderTop << std::endl;
    std::wcout << L"║" << title << std::wstring(message.length() + 1 - title.length(), L' ') << L"║" << std::endl;
    std::wcout << borderMid << std::endl;
    std::wcout << emptyLine << std::endl;
    std::wcout << messageLine << std::endl;
    std::wcout << emptyLine << std::endl;
    std::wcout << borderBottom << std::endl;
    std::wcout << resetCode;

    if (pause) {
        Utilidades::consolePause();
    }

    if (clearConsole) {
        Utilidades::clearConsole();
    }
}

