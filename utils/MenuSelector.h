#ifndef MENUSELECTOR_H
#define MENUSELECTOR_H

#include <vector>
#include <string>
#include "PrettyPrinter.h"

class MenuSelector {
    private:
        std::wstring title;
        std::vector<std::wstring> options;
        PrettyPrinter printer;

    public:
        MenuSelector(const std::wstring& title, const std::vector<std::wstring>& options);
        unsigned long showMenu();
    };

#endif
