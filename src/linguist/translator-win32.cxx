//
// Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
//

#include "linguist/translator.hxx"

#include <windows.h>

namespace linguist
{
    auto translator::detect_system_locale() -> std::string
    {
        wchar_t locale_name[LOCALE_NAME_MAX_LENGTH] = { 0 };
        if (GetUserDefaultLocaleName(locale_name, ARRAYSIZE(locale_name)) > 0)
        {
            char buffer[LOCALE_NAME_MAX_LENGTH] = { 0 };
            size_t converted{};
            if (wcstombs_s(&converted, buffer, locale_name, ARRAYSIZE(buffer)) == 0)
            {
                return std::string(buffer);
            }
        }

        return "en-US"; // Default fallback
    }

} // namespace linguist
