#include <string>
#include <codecvt>
#include <locale>
#include "StrLib.h"

std::wstring operator + (const std::wstring& wstr, const std::string& str) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return wstr + converter.from_bytes(str);
}

std::wstring operator + (const std::string& str, const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str) + wstr;
}

std::wostream& operator << (std::wostream& wos, const std::string& str) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wstr = converter.from_bytes(str);
    wos << wstr;
    return wos;
}

std::wostream& operator << (std::wostream& wos, const char* cstr) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
    std::wstring wstr = converter.from_bytes(cstr);
    wos << wstr;
    return wos;
}


