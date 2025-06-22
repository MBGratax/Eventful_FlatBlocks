#pragma once

#include "EFText.h"

namespace EventfulEngine{
    std::unordered_map<EFString, EFString> EFText::_macroTable{};

    EFString EFText::ColorCode(const EFTextColor color){
        switch (color){
            using enum EFTextColor;
        case Black: return "\033[30m";
        case Red: return "\033[31m";
        case Green: return "\033[32m";
        case Yellow: return "\033[33m";
        case Blue: return "\033[34m";
        case Magenta: return "\033[35m";
        case Cyan: return "\033[36m";
        case White: return "\033[37m";
        case BrightBlack: return "\033[90m";
        case BrightRed: return "\033[91m";
        case BrightGreen: return "\033[92m";
        case BrightYellow: return "\033[93m";
        case BrightBlue: return "\033[94m";
        case BrightMagenta: return "\033[95m";
        case BrightCyan: return "\033[96m";
        case BrightWhite: return "\033[97m";
        default: return "\033[0m";
        }
    }

    EFString EFText::ApplyColor(const std::string_view text, const EFTextColor color){
        return ColorCode(color) + EFString(text) + ColorCode(EFTextColor::TextColor);
    }

    void EFText::RegisterMacro(const std::string_view name, const std::string_view value){
        _macroTable[EFString(name)] = EFString(value);
    }

    EFString EFText::GetMacro(const std::string_view name){
        if (const auto macro = _macroTable.find(EFString(name)); macro != _macroTable.end()){
            return macro->second;
        }
        return {};
    }

    EFString EFText::ExpandMacros(std::string_view text){
        EFString result;
        result.reserve(text.size());
        const size_t size = text.size();
        size_t i = 0;
        while (i < size){
            if (text[i] == '$' && i + 1 < text.size() && text[i + 1] == '('){
                if (const size_t end = text.find(')', i + 2); end != std::string_view::npos){
                    const auto key = text.substr(i + 2, end - (i + 2));
                    result += GetMacro(key);
                    i = end + 1;
                    continue;
                }
            }
            result.push_back(text[i]);
            ++i;
        }
        return result;
    }

    void EFText::ClearMacros(){
        _macroTable.clear();
    }
} // EventfulEngine
