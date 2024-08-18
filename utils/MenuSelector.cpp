#include <iostream>
#include <vector>
#include <stddef.h>
#include <limits>
#include <string>
#include "../lib/StrLib.h"
#include "PrettyPrinter.h"
#include "TermInput.h"
#include "Utilidades.h"
#include "ConsoleColor.h"
#include "MenuSelector.h"

MenuSelector::MenuSelector(const std::wstring& title, const std::vector<std::wstring>& options) : title(title), options(options) {}

unsigned long MenuSelector::showMenu() {
    unsigned long choice = -1;
    while (true) {
        std::wcout << ConsoleColor::BG_GREEN << ConsoleColor::BLACK << "   " << title << "   " << ConsoleColor::RESET << "\n\n";

        for (unsigned long i = 0; i < options.size(); ++i) {
            std::wcout << ConsoleColor::GREEN << (i + 1) << L". " << ConsoleColor::YELLOW << options[i] << ConsoleColor::RESET << std::endl;
        }
        std::wcout << "\n" << ConsoleColor::BLUE << L"Elija una opción: " << ConsoleColor::RESET;
        choice = ingresarEntero();

        if (choice < 1 || choice > options.size()) {
            printer.print(L"Opción inválida. Vuelva a intentar", PrettyPrinter::PPERROR);
            continue;
        }
        return choice - 1;
    }
}

