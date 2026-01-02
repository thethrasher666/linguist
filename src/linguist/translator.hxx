//
// Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
//

#pragma once

#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace linguist
{
    /// Generated function for embedded translations (defined by embed_translation_file)
    [[nodiscard]] auto get_embedded_translations() -> std::unordered_map<std::string, std::unordered_map<std::string, std::string>>;

    /// Lightweight translation library for locale-based string lookups
    ///
    /// translator loads translations from a JSON file and provides locale-aware
    /// string retrieval. The JSON format uses identifiers as keys, with locale
    /// codes mapping to translated strings.
    ///
    class translator
    {
    public:
        /// Construct a new translator object
        translator();

        /// Destroy the translator object
        ~translator() = default;

        /// Disable copy
        translator(const translator&) = delete;

        /// Disable copy
        translator& operator=(const translator&) = delete;

        /// Enable move
        translator(translator&&) noexcept = default;

        /// Enable move
        translator& operator=(translator&&) noexcept = default;

        /// Load translations from a JSON string
        ///
        /// \param json_content JSON content as a string
        /// \return true if loaded successfully
        /// \return false if loading failed
        [[nodiscard]] auto load_from_string(const std::string& json_content) -> bool;

        /// Set the current locale
        ///
        /// \param locale Locale code (e.g., "en-US", "fr-FR")
        void set_locale(const std::string& locale);

        /// Get the current locale
        ///
        /// \return Current locale code
        [[nodiscard]] auto get_locale() const -> const std::string&;

        /// Get translation for an identifier using current locale
        ///
        /// \param identifier Translation identifier/key
        /// \return Translated string if found
        ///
        [[nodiscard]] auto translate(const std::string& identifier) const -> std::optional<std::string>;

        /// Get translation for an identifier with fallback
        ///
        /// \param identifier Translation identifier/key
        /// \param fallback Fallback string if translation not found
        /// \return Translated string or fallback
        [[nodiscard]] auto translate(const std::string& identifier, const std::string& fallback) const -> std::string;

        /// Get translation for a specific locale
        ///
        /// \param identifier Translation identifier/key
        /// \param locale Locale code to use for this translation
        /// \return Translated string if found
        [[nodiscard]] auto translate(const std::string& identifier, const std::string& locale, bool) const -> std::optional<std::string>;

        /// Check if a translation exists for an identifier
        ///
        /// \param identifier Translation identifier/key
        /// \return true if translation exists for current locale
        /// \return false otherwise
        [[nodiscard]] auto has_translation(const std::string& identifier) const -> bool;

        /// Get all available locales
        ///
        /// \return List of locale codes
        [[nodiscard]] auto get_available_locales() const -> std::vector<std::string>;

        /// Detect system locale automatically
        ///
        /// \return Detected locale code (e.g., "en-US")
        [[nodiscard]] static auto detect_system_locale() -> std::string;

    private:
        /// Load translations from embedded data
        ///
        /// Loads translations that were embedded at build-time using embed_translation_file().
        /// This calls the generated get_embedded_translations() function.
        void load_embedded();

    private:
        std::string current_locale_;
        std::unordered_map<std::string, std::unordered_map<std::string, std::string>> translations_;
    };

} // namespace linguist
