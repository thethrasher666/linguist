//
// Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
//

#include <linguist/translator.hxx>

#include <filesystem>
#include <fstream>
#include <catch2/catch_test_macros.hpp>

TEST_CASE("Embedding tool generates valid C++ code")
{
    const std::filesystem::path k_test_json = std::filesystem::temp_directory_path() / "test_embed.json";
    const std::filesystem::path k_test_output = std::filesystem::temp_directory_path() / "test_embed.cxx";

    // Create a test JSON file
    {
        std::ofstream out(k_test_json);
        out << R"({
    "test.key1": {
        "en-US": "Value 1",
        "fr-FR": "Valeur 1"
    },
    "test.key2": {
        "en-US": "Value 2",
        "fr-FR": "Valeur 2"
    }
})";
    }

    // Run the embedding tool
    std::string command = std::string(EMBED_TOOL_PATH) + " " + k_test_json.string() + " " + k_test_output.string();
    int32_t result = std::system(command.c_str());
    REQUIRE(result == 0);

    // Verify output file exists
    REQUIRE(std::filesystem::exists(k_test_output));

    // Read the generated file
    {
        std::ifstream in(k_test_output);
        std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

        // Verify content contains expected structure
        REQUIRE(content.find("namespace linguist") != std::string::npos);
        REQUIRE(content.find("get_embedded_translations") != std::string::npos);
        REQUIRE(content.find("test.key1") != std::string::npos);
        REQUIRE(content.find("test.key2") != std::string::npos);
        REQUIRE(content.find("Value 1") != std::string::npos);
        REQUIRE(content.find("Valeur 2") != std::string::npos);
    }

    // Cleanup
    std::filesystem::remove(k_test_json);
    std::filesystem::remove(k_test_output);
}

TEST_CASE("Embedding tool handles quotes correctly")
{
    const std::filesystem::path k_test_json = std::filesystem::temp_directory_path() / "test_quotes.json";
    const std::filesystem::path k_test_output = std::filesystem::temp_directory_path() / "test_quotes.cxx";

    // Create a test JSON with quotes
    {
        std::ofstream out(k_test_json);
        out << R"({
    "with.quotes": {
        "en-US": "Say \"Hello\" there"
    }
})";
    }

    // Run the embedding tool
    std::string command = std::string(EMBED_TOOL_PATH) + " " + k_test_json.string() + " " + k_test_output.string();
    int32_t result = std::system(command.c_str());
    REQUIRE(result == 0);

    // Read the generated file
    {
        std::ifstream in(k_test_output);
        std::string content((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());

        // Verify quotes are escaped
        REQUIRE(content.find("\\\"Hello\\\"") != std::string::npos);
    }

    // Cleanup
    std::filesystem::remove(k_test_json);
    std::filesystem::remove(k_test_output);
}

TEST_CASE("Embedding tool fails on invalid JSON")
{
    const std::filesystem::path k_test_json = std::filesystem::temp_directory_path() / "test_invalid.json";
    const std::filesystem::path k_test_output = std::filesystem::temp_directory_path() / "test_invalid.cxx";

    // Create an invalid JSON file
    {
        std::ofstream out(k_test_json);
        out << "{ invalid json content }";
    }

    // Run the embedding tool - should fail
    std::string command = std::string(EMBED_TOOL_PATH) + " " + k_test_json.string() + " " + k_test_output.string();
    int32_t result = std::system(command.c_str());
    REQUIRE(result != 0);

    // Cleanup
    std::filesystem::remove(k_test_json);
    if (std::filesystem::exists(k_test_output))
    {
        std::filesystem::remove(k_test_output);
    }
}

TEST_CASE("Embedding tool fails on missing input file")
{
    const std::filesystem::path k_test_json = std::filesystem::temp_directory_path() / "nonexistent.json";
    const std::filesystem::path k_test_output = std::filesystem::temp_directory_path() / "test_output.cxx";

    // Run the embedding tool with non-existent file - should fail
    std::string command = std::string(EMBED_TOOL_PATH) + " " + k_test_json.string() + " " + k_test_output.string();
    int32_t result = std::system(command.c_str());
    REQUIRE(result != 0);

    // Output file should not be created
    REQUIRE(!std::filesystem::exists(k_test_output));
}
