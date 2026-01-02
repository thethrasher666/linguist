//
// Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
//

#include <linguist/translator.hxx>

#include <algorithm>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <catch2/catch_test_macros.hpp>

static const auto test_data_file = std::filesystem::path(TEST_DATA_FILE);

static auto read_file(const std::filesystem::path& path) -> std::string
{
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

TEST_CASE("translator detects system locale")
{
    std::string system_locale = linguist::translator::detect_system_locale();
    REQUIRE(!system_locale.empty());
    REQUIRE(system_locale.find('-') != std::string::npos);
}

TEST_CASE("translator loads translations from JSON string")
{
    const std::string json = R"({
        "test.key": {
            "en-US": "Test",
            "fr-FR": "Essai"
        }
    })";

    linguist::translator translator;
    REQUIRE(translator.load_from_string(json));
}

TEST_CASE("translator returns available locales")
{
    linguist::translator translator;
    const auto json = read_file(test_data_file);
    REQUIRE(translator.load_from_string(json));

    auto locales = translator.get_available_locales();
    REQUIRE(locales.size() == 3);
    REQUIRE(std::find(locales.begin(), locales.end(), "en-US") != locales.end());
    REQUIRE(std::find(locales.begin(), locales.end(), "fr-FR") != locales.end());
    REQUIRE(std::find(locales.begin(), locales.end(), "es-ES") != locales.end());
}

TEST_CASE("translator sets and gets locale")
{
    linguist::translator translator;
    translator.set_locale("fr-FR");
    REQUIRE(translator.get_locale() == "fr-FR");
}

TEST_CASE("translator translates using current locale")
{
    linguist::translator translator;
    const auto json = read_file(test_data_file);
    REQUIRE(translator.load_from_string(json));

    SECTION("en-US translations")
    {
        translator.set_locale("en-US");
        REQUIRE(translator.translate("home.title") == "Home");
        REQUIRE(translator.translate("home.subtitle") == "Welcome back");
        REQUIRE(translator.translate("button.save") == "Save");
    }

    SECTION("fr-FR translations")
    {
        translator.set_locale("fr-FR");
        REQUIRE(translator.translate("home.title") == "Accueil");
        REQUIRE(translator.translate("home.subtitle") == "Bon retour");
        REQUIRE(translator.translate("button.save") == "Enregistrer");
    }

    SECTION("es-ES translations")
    {
        translator.set_locale("es-ES");
        REQUIRE(translator.translate("home.title") == "Inicio");
        REQUIRE(translator.translate("home.subtitle") == "Bienvenido de nuevo");
        REQUIRE(translator.translate("button.save") == "Guardar");
    }
}

TEST_CASE("translator returns nullopt for missing keys")
{
    linguist::translator translator;
    const auto json = read_file(test_data_file);
    REQUIRE(translator.load_from_string(json));
    translator.set_locale("en-US");

    REQUIRE(!translator.translate("nonexistent.key").has_value());
}

TEST_CASE("translator uses fallback string")
{
    linguist::translator translator;
    const auto json = read_file(test_data_file);
    REQUIRE(translator.load_from_string(json));
    translator.set_locale("en-US");

    REQUIRE(translator.translate("nonexistent.key", "Fallback Text") == "Fallback Text");
}

TEST_CASE("translator falls back to base language")
{
    const std::string json = R"({
        "test.key": {
            "en": "English"
        }
    })";

    linguist::translator translator;
    REQUIRE(translator.load_from_string(json));
    translator.set_locale("en-US");

    REQUIRE(translator.translate("test.key") == "English");
}

TEST_CASE("translator translates for specific locale")
{
    linguist::translator translator;
    const auto json = read_file(test_data_file);
    REQUIRE(translator.load_from_string(json));

    auto translation = translator.translate("button.save", "fr-FR", true);
    REQUIRE(translation.has_value());
    REQUIRE(*translation == "Enregistrer");
}

TEST_CASE("translator checks if translation exists")
{
    linguist::translator translator;
    const auto json = read_file(test_data_file);
    REQUIRE(translator.load_from_string(json));
    translator.set_locale("en-US");

    REQUIRE(translator.has_translation("home.title"));
    REQUIRE(!translator.has_translation("unknown.key"));
}

TEST_CASE("translator returns first available translation as fallback")
{
    const std::string json = R"({
        "test.key": {
            "fr-FR": "Français"
        }
    })";

    linguist::translator translator;
    REQUIRE(translator.load_from_string(json));
    translator.set_locale("de-DE");

    // Should return first available (fr-FR) when locale not found
    auto translation = translator.translate("test.key");
    REQUIRE(translation.has_value());
    REQUIRE(*translation == "Français");
}
