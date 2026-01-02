//
// Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
//

#include "linguist/translator.hxx"

#include <cstdlib>
#include <fstream>
#include <locale>
#include <sstream>
#include <nlohmann/json.hpp>

namespace linguist
{
    translator::translator() : current_locale_(detect_system_locale())
    {
        load_embedded();
    }

    auto translator::load_embedded() -> void
    {
        translations_ = get_embedded_translations();
    }

    auto translator::load_from_string(const std::string& json_content) -> bool
    {
        try
        {
            translations_ = nlohmann::json::parse(json_content);
            return !translations_.empty();
        }
        catch (...)
        {
            return false;
        }
    }

    void translator::set_locale(const std::string& locale)
    {
        current_locale_ = locale;
    }

    auto translator::get_locale() const -> const std::string&
    {
        return current_locale_;
    }

    auto translator::translate(const std::string& identifier) const -> std::optional<std::string>
    {
        auto it = translations_.find(identifier);
        if (it == translations_.end())
        {
            return std::nullopt;
        }

        const auto& locale_map = it->second;
        auto localeIt = locale_map.find(current_locale_);
        if (localeIt != locale_map.end())
        {
            return localeIt->second;
        }

        // Try fallback to base language (e.g., "en" from "en-US")
        const size_t dashPos = current_locale_.find('-');
        if (dashPos != std::string::npos)
        {
            std::string baseLocale = current_locale_.substr(0, dashPos);
            for (const auto& [locale, translation] : locale_map)
            {
                if (locale.find(baseLocale) == 0)
                {
                    return translation;
                }
            }
        }

        // Return first available translation as last resort
        if (!locale_map.empty())
        {
            return locale_map.begin()->second;
        }

        return std::nullopt;
    }

    auto translator::translate(const std::string& identifier, const std::string& fallback) const -> std::string
    {
        auto result = translate(identifier);
        return result.value_or(fallback);
    }

    auto translator::translate(const std::string& identifier, const std::string& locale, bool) const -> std::optional<std::string>
    {
        auto it = translations_.find(identifier);
        if (it == translations_.end())
        {
            return std::nullopt;
        }

        const auto& locale_map = it->second;
        auto localeIt = locale_map.find(locale);
        if (localeIt != locale_map.end())
        {
            return localeIt->second;
        }

        return std::nullopt;
    }

    auto translator::has_translation(const std::string& identifier) const -> bool
    {
        return translate(identifier).has_value();
    }

    auto translator::get_available_locales() const -> std::vector<std::string>
    {
        std::unordered_map<std::string, bool> localeSet;

        for (const auto& [identifier, locale_map] : translations_)
        {
            for (const auto& [locale, translation] : locale_map)
            {
                localeSet[locale] = true;
            }
        }

        std::vector<std::string> locales;
        locales.reserve(localeSet.size());
        for (const auto& [locale, _] : localeSet)
        {
            locales.push_back(locale);
        }

        return locales;
    }
} // namespace linguist
