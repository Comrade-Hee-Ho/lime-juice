#pragma once

#include <string>

// decode a UTF-8 string into a sequence of Unicode codepoints
inline std::u32string utf8_to_codepoints(const std::string& text) {
    std::u32string result;
    size_t i = 0;

    while (i < text.size()) {
        char32_t cp;
        uint8_t b = static_cast<uint8_t>(text[i]);

        if (b < 0x80) {
            cp = b;
            i++;
        } else if ((b & 0xE0) == 0xC0) {
            cp = (b & 0x1F) << 6;
            cp |= (static_cast<uint8_t>(text[i + 1]) & 0x3F);
            i += 2;
        } else if ((b & 0xF0) == 0xE0) {
            cp = (b & 0x0F) << 12;
            cp |= (static_cast<uint8_t>(text[i + 1]) & 0x3F) << 6;
            cp |= (static_cast<uint8_t>(text[i + 2]) & 0x3F);
            i += 3;
        } else {
            cp = (b & 0x07) << 18;
            cp |= (static_cast<uint8_t>(text[i + 1]) & 0x3F) << 12;
            cp |= (static_cast<uint8_t>(text[i + 2]) & 0x3F) << 6;
            cp |= (static_cast<uint8_t>(text[i + 3]) & 0x3F);
            i += 4;
        }

        result.push_back(cp);
    }

    return result;
}

// encode a single Unicode codepoint as a UTF-8 string
inline std::string char32_to_utf8(char32_t c) {
    std::string result;

    if (c < 0x80) {
        result += static_cast<char>(c);
    } else if (c < 0x800) {
        result += static_cast<char>(0xC0 | (c >> 6));
        result += static_cast<char>(0x80 | (c & 0x3F));
    } else if (c < 0x10000) {
        result += static_cast<char>(0xE0 | (c >> 12));
        result += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (c & 0x3F));
    } else {
        result += static_cast<char>(0xF0 | (c >> 18));
        result += static_cast<char>(0x80 | ((c >> 12) & 0x3F));
        result += static_cast<char>(0x80 | ((c >> 6) & 0x3F));
        result += static_cast<char>(0x80 | (c & 0x3F));
    }

    return result;
}
