//
// Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
//

#include <fstream>
#include <iostream>
#include <string>
#include <nlohmann/json.hpp>

///
/// Build-time tool to generate embedded translation data from JSON
///
auto main(int32_t argc, char* argv[]) -> int32_t
{
    if (argc != 3)
    {
        std::cerr << "Usage: " << argv[0] << " <input.json> <output.cxx>\n";
        return 1;
    }

    const std::string input_file = argv[1];
    const std::string output_file = argv[2];

    try
    {
        // Read and parse JSON
        std::ifstream input(input_file);
        if (!input.is_open())
        {
            std::cerr << "Error: Cannot open input file: " << input_file << "\n";
            return 1;
        }

        nlohmann::json translations = nlohmann::json::parse(input);

        // Generate C++ source file
        std::ofstream output(output_file);
        if (!output.is_open())
        {
            std::cerr << "Error: Cannot open output file: " << output_file << "\n";
            return 1;
        }

        // Write header
        output << "//\n";
        output << "// Generated file - DO NOT EDIT\n";
        output << "// Generated from: " << input_file << "\n";
        output << "//\n\n";
        output << "#include <string>\n";
        output << "#include <unordered_map>\n\n";
        output << "namespace linguist\n";
        output << "{\n\n";

        // Write the embedded data structure
        output << "auto get_embedded_translations() -> std::unordered_map<std::string, std::unordered_map<std::string, std::string>>\n";
        output << "{\n";
        output << "    return {\n";

        // Generate entries
        for (auto& [key, locale_map] : translations.items())
        {
            output << "        {\"" << key << "\", {\n";

            for (auto& [locale, text] : locale_map.items())
            {
                // Escape quotes in the text
                std::string escaped_text = text.get<std::string>();
                size_t pos = 0;
                while ((pos = escaped_text.find('"', pos)) != std::string::npos)
                {
                    escaped_text.replace(pos, 1, "\\\"");
                    pos += 2;
                }

                output << "            {\"" << locale << "\", \"" << escaped_text << "\"},\n";
            }

            output << "        }},\n";
        }

        output << "    };\n";
        output << "}\n\n";
        output << "} // namespace linguist\n";

        std::cout << "Generated " << output_file << " from " << input_file << "\n";
        return 0;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }
}
