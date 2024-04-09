#pragma once
#include <string>
#include <Windows.h>
#include "Types.h"

class Utils
{
public:
    static String UTF8toUTF16(const std::string& utf8_string)
    {
        int utf16_length = MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, nullptr, 0);
        String utf16_string(utf16_length, 0);
        MultiByteToWideChar(CP_UTF8, 0, utf8_string.c_str(), -1, &utf16_string[0], utf16_length);
        return utf16_string;
    }

    static std::string UTF16toUTF8(const String& utf16_string)
    {
        int utf8_length = WideCharToMultiByte(CP_UTF8, 0, utf16_string.c_str(), -1, nullptr, 0, nullptr, nullptr);
        std::string utf8_string(utf8_length, 0);
        WideCharToMultiByte(CP_UTF8, 0, utf16_string.c_str(), -1, &utf8_string[0], utf8_length, nullptr, nullptr);
        return utf8_string;
    }
};

