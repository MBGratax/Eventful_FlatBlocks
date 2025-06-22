#pragma once

#include "CoreTypes.h"
#include "EFCoreModuleAPI.h"

#include <format>
#include <string_view>
#include <unordered_map>

namespace EventfulEngine{

    class EFCORE_API EFText{
    public:
        /** ANSI color identifiers used for colored console output. */
        enum class EFTextColor{
            TextColor,
            Black,
            Red,
            Green,
            Yellow,
            Blue,
            Magenta,
            Cyan,
            White,
            BrightBlack,
            BrightRed,
            BrightGreen,
            BrightYellow,
            BrightBlue,
            BrightMagenta,
            BrightCyan,
            BrightWhite
        };

        /** Format a string using std::format semantics. */
        template <typename... Args>
        static EFString Format(const std::string_view fmt, Args&&... args){
            return std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
        }

        template <typename... Args>
        static const char* Format(const char* fmt, Args&&... args){
            return std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...)).c_str();
        }

        /** Return the escape code for a color. */
        static EFString ColorCode(EFTextColor color);

        /** Wrap the provided text in the given color escape codes. */
        static EFString ApplyColor(std::string_view text, EFTextColor color);

        /** Register a macro that can later be expanded. */
        static void RegisterMacro(std::string_view name, std::string_view value);

        /** Lookup a macro value. Returns empty string if missing. */
        static EFString GetMacro(std::string_view name);

        /** Replace all occurrences of $(MACRO) with their values. */
        static EFString ExpandMacros(std::string_view text);

        /** Remove all registered macros. */
        static void ClearMacros();

    private:
        static std::unordered_map<EFString, EFString> _macroTable;
    };
}
