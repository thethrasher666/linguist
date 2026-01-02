//
// Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
//

#include "linguist/translator.hxx"

#include <CoreFoundation/CoreFoundation.h>

namespace linguist
{
    auto translator::detect_system_locale() -> std::string
    {
        CFLocaleRef locale = CFLocaleCopyCurrent();
        CFStringRef locale_id = (CFStringRef) CFLocaleGetValue(locale, kCFLocaleIdentifier);
        char buffer[128];
        CFStringGetCString(locale_id, buffer, sizeof(buffer), kCFStringEncodingUTF8);
        CFRelease(locale);

        // Convert from "en_US" to "en-US"
        std::string result(buffer);
        size_t pos = result.find('_');
        if (pos != std::string::npos)
        {
            result[pos] = '-';
        }
        return result;
    }

} // namespace linguist
