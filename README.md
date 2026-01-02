# Linguist

A lightweight C++ translation library for locale-based string management, similar in concept to Qt Linguist but with minimal dependencies.

## Features

- **Locale Detection** - Automatically detects system locale on Windows, macOS, and Linux
- **Simple API** - Easy-to-use interface for translation lookups
- **Fallback Support** - Gracefully handles missing translations with fallbacks
- **CMake Integration** - Easy to integrate into existing projects
- **Fast** - In-memory translations for quick lookups

## Quick Start

### JSON Format

Create a JSON file with your translations:

```json
{
    "home.title": {
        "en-US": "Home",
        "fr-FR": "Accueil",
        "es-ES": "Inicio"
    },
    "button.save": {
        "en-US": "Save",
        "fr-FR": "Enregistrer",
        "es-ES": "Guardar"
    }
}
```

### Basic Usage

```cpp
#include <linguist/translator.hxx>

auto main() -> int32_t
{
    // Automatically load embedded translations.
    linguist::translator translator;

    // OPTIONAL - Set a specific locale instead of the auto-detected system locale.
    translator.set_locale("fr-FR");

    // Get translation
    const auto text = translator.translate("home.title");
    if (text)
    {
        std::cout << *text << std::endl;
    }

    // With fallback
    const std::string saveButton = translator.translate("button.save", "Save");

    return 0;
}
```

## Building

### Prerequisites

- CMake 3.27 or higher
- C++20 compatible compiler

## Integration

### Using CMake FetchContent

Add Linguist to your project without installing:

```cmake
include(FetchContent)
FetchContent_Declare(
    Linguist
    GIT_REPOSITORY https://github.com/thethrasher666/linguist.git
    GIT_TAG        v1.0.0  # or main for latest
)

# Don't build Linguist's tests
set(BUILD_LINGUIST_TESTS OFF)
FetchContent_MakeAvailable(Linguist)

# Include the embedding function
include("${linguist_SOURCE_DIR}/share/cmake/LinguistEmbedTranslations.cmake")

# Embed translations at build time
embed_translation_file(
    INPUT_FILE "${CMAKE_SOURCE_DIR}/translations.json"
    OUTPUT_VARIABLE embedded_translations
)

add_executable(your_app
    main.cpp
    ${embedded_translations}
)
target_link_libraries(your_app PRIVATE linguist::translator)
```

## API Reference

### Core Methods

- `bool load_from_string(const std::string& json_content)` - Load translations from JSON string
- `void set_locale(const std::string& locale)` - Set current locale (e.g., "en-US")
- `std::optional<std::string> translate(const std::string& identifier)` - Get translation for current locale
- `std::string translate(const std::string& identifier, const std::string& fallback)` - Get translation with fallback
- `bool has_translation(const std::string& identifier)` - Check if translation exists

### CMake Functions

- `embed_translation_file(INPUT_FILE <json> OUTPUT_VARIABLE <var>)` - Generate embedded translation source file

### Locale Detection

The library automatically detects the system locale on:
- **Windows** - Uses `GetUserDefaultLocaleName`
- **macOS** - Uses CoreFoundation `CFLocaleCopyCurrent`
- **Linux** - Reads `LANG` environment variable

Locale codes follow the format: `language-COUNTRY` (e.g., `en-US`, `fr-FR`, `es-ES`)

### Fallback Behavior

When a translation is not found:
1. Tries exact locale match (e.g., `en-US`)
2. Falls back to base language (e.g., `en` from `en-US`)
3. Returns first available translation
4. Returns `std::nullopt` or provided fallback string

## Platform Support

- ✅ Windows (MSVC)
- ✅ macOS (Clang)
- ✅ Linux (GCC, Clang)

## Documentation

- [Architecture & Design Decisions](docs/ARCHITECTURE.md) - In-depth explanation of design choices, trade-offs, and comparisons

## License

MIT License - See [LICENSE](LICENSE) file for details

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## Roadmap

- [ ] Support for additional JSON libraries (nlohmann/json integration)
- [ ] Translation validation tools
