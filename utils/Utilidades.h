#ifndef UTILIDADES_H
#define UTILIDADES_H

#include <iostream>
#include <string>
#include <stddef.h>
#include <limits>

class Utilidades {
private:
    
public:
    static void consolePause();
    static void clearConsole();
    static std::wstring toWString(const std::string& str);
};

#endif
