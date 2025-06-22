#pragma once

#include <cxxopts.hpp>
#include "CoreTypes.h"

namespace EventfulEngine{

    using CommandLineOptions = cxxopts::ParseResult;

    class EFCORE_API EFCommandLine{
    public:
        /** Register an additional command line option to be parsed. */
        template <typename T>
        void AddOption(const EFString& opts, const EFString& desc);

        template <class T>
        void AddOption(const EFString& opts, const EFString& desc, const EFString& help);

        template <typename T>
        void AddOption(const EFString& opts, const EFString& desc, const EFString& help,
                       const EFString& defaultValue);

        /** Parse the command line and cache the result for later access. */
        void Parse();

        template <typename T>
        T EFCORE_API GetOption(const EFString& name);

        EFCommandLine Init(int argc, char** argv);

        /** Return the options parsed from the last call to Parse. */
        [[nodiscard]] const CommandLineOptions& GetOptions() const{ return _options; };

        /** Retrieve the raw command line used during Parse. */
        [[nodiscard]] const char* const* GetCommandLine() const{ return _argv.data(); };

        explicit EFCommandLine(const std::string_view& CommandlineName,
                               const std::string_view& CommandlineDesc) :
            _optionsParser(
                std::string(CommandlineName), std::string(CommandlineDesc)){
            _optionsParser.allow_unrecognised_options();
        };

        EFCommandLine Init(int argc, const char* argv[]);

        [[nodiscard]] int32 Argc() const noexcept{ return static_cast<int>(_argv.size()); }

    private:
        CommandLineOptions _options;
        std::vector<const char*> _argv;
        cxxopts::Options _optionsParser;
    };

    template <typename T>
    void EFCommandLine::AddOption(const EFString& opts, const EFString& desc){
        _optionsParser.add_options()(opts, desc, cxxopts::value<T>());
    }

    template <typename T>
    void EFCommandLine::AddOption(const EFString& opts, const EFString& desc, const EFString& help){
        _optionsParser.add_options()(opts, desc, cxxopts::value<T>(), help);
    }

    template <typename T>
    void EFCommandLine::AddOption(const EFString& opts, const EFString& desc, const EFString& help,
                                  const EFString& defaultValue){
        _optionsParser.add_options()(opts, desc, cxxopts::value<T>()->default_value(defaultValue), help);
    }

} // EventfulEngine
