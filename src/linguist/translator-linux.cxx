//
// Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
//

#include "linguist/translator.hxx"

namespace linguist
{
    auto translator::detect_system_locale() -> std::string
    {
        // Linux/Unix
        if (const char* lang = std::getenv("LANG"); lang)
        {
            std::string locale(lang);

            // Trim whitespace
            locale.erase(0, locale.find_first_not_of(" \t\n\r"));
            locale.erase(locale.find_last_not_of(" \t\n\r") + 1);

            // Extract locale from formats like "en_US.UTF-8"
            const size_t dot_position = locale.find('.');
            if (dot_position != std::string::npos)
            {
                locale = locale.substr(0, dot_position);
            }

            // Handle "C" or "POSIX" locales which are minimal/default
            if (locale.empty() || locale == "C" || locale == "POSIX")
            {
                return "en-US";
            }

            // Convert from "en_US" to "en-US"
            const size_t underscore_position = locale.find('_');
            if (underscore_position != std::string::npos)
            {
                locale[underscore_position] = '-';
            }

            return locale;
        }

        return "en-US"; // Default fallback
    }

} // namespace linguist
