#include <iostream>
#include "ConsoleColor.h"

std::wstring ConsoleColor::BLACK   = L"\033[30m";
std::wstring ConsoleColor::RED     = L"\033[31m";
std::wstring ConsoleColor::GREEN   = L"\033[32m";
std::wstring ConsoleColor::YELLOW  = L"\033[33m";
std::wstring ConsoleColor::BLUE    = L"\033[34m";
std::wstring ConsoleColor::MAGENTA = L"\033[35m";
std::wstring ConsoleColor::CYAN    = L"\033[36m";
std::wstring ConsoleColor::WHITE   = L"\033[37m";
std::wstring ConsoleColor::GRAY    = L"\033[90m";

std::wstring ConsoleColor::BG_BLACK   = L"\033[40m";
std::wstring ConsoleColor::BG_RED     = L"\033[41m";
std::wstring ConsoleColor::BG_GREEN   = L"\033[42m";
std::wstring ConsoleColor::BG_YELLOW  = L"\033[43m";
std::wstring ConsoleColor::BG_BLUE    = L"\033[44m";
std::wstring ConsoleColor::BG_MAGENTA = L"\033[45m";
std::wstring ConsoleColor::BG_CYAN    = L"\033[46m";
std::wstring ConsoleColor::BG_WHITE   = L"\033[47m";

std::wstring ConsoleColor::RESET      = L"\033[0m";

