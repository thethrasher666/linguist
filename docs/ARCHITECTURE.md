# Linguist Architecture & Design Decisions

## Overview

Linguist is a lightweight, zero-configuration C++ translation library designed for applications that need locale-based string management without the complexity and dependencies of full-featured internationalization frameworks like Qt Linguist, gettext, or ICU.

## Design Philosophy

### Core Principles

1. **Minimal Dependencies** - Only requires nlohmann/json for parsing, which is widely adopted
2. **Zero Configuration** - Works out-of-the-box with automatic locale detection
3. **Build-Time Embedding** - Translations compiled into the binary for zero I/O overhead
4. **Simple Mental Model** - Straightforward key-value lookup with locale awareness
5. **Graceful Degradation** - Multiple fallback strategies for missing translations

## Key Design Decisions

### 1. JSON as Translation Format

**Decision:** Use JSON for translation data instead of custom formats (e.g., `.po`, `.ts`, `.properties`)

**Rationale:**
- **Universal Tooling** - Every text editor and IDE has JSON support
- **No Learning Curve** - Developers and translators already know JSON
- **Validation** - Built-in syntax validation from JSON parsers
- **Extensibility** - Easy to add metadata or nested structures in the future
- **Ecosystem** - Extensive tooling for JSON manipulation and validation

**Trade-offs:**
- ❌ More verbose than some alternatives (`.properties`)
- ❌ No built-in support for pluralization or gender (can be added via conventions)
- ✅ Human-readable and easy to diff in version control
- ✅ No need for specialized translation editors
- ✅ Can be generated/consumed by any language or tool

**Alternatives Considered:**
- **gettext (.po)** - Rejected: requires GNU toolchain, complex workflow
- **Qt Linguist (.ts)** - Rejected: XML overhead, Qt dependency
- **YAML** - Rejected: whitespace sensitivity, less universal support
- **Custom binary format** - Rejected: requires custom tooling, not human-readable

### 2. Build-Time Embedding

**Decision:** Embed translations into the binary at compile-time rather than loading files at runtime

**Rationale:**
- **Zero Runtime I/O** - No file system access, no parsing overhead at startup
- **Deployment Simplicity** - Single binary contains all translations
- **Performance** - Translations are already in memory, no lazy loading complexity
- **Security** - Translations cannot be tampered with or accidentally deleted
- **Deterministic** - No missing file errors or permission issues

**Trade-offs:**
- ❌ Larger binary size (typically negligible for translation data)
- ❌ Must recompile to update translations (acceptable for most desktop apps)
- ❌ Cannot swap translations without rebuilding
- ✅ Fastest possible lookup performance
- ✅ No deployment issues with missing translation files
- ✅ Works in restricted environments (sandboxed apps, embedded systems)

**When This Choice Doesn't Fit:**
- Server applications needing runtime translation updates
- Applications with extremely large translation datasets (MB+)
- User-generated or dynamic translation content

Runtime loading via `load_from_string()` is still available for these use cases.

### 3. In-Memory Hash Table Structure

**Decision:** Use `std::unordered_map<std::string, std::unordered_map<std::string, std::string>>` for storage

**Structure:**
```cpp
{
  "identifier" -> {
    "locale" -> "translated_string"
  }
}
```

**Rationale:**
- **O(1) Lookups** - Fast hash-based access for both identifier and locale
- **Standard Library** - No custom data structures, predictable behavior
- **Memory Efficiency** - Hash tables have reasonable memory overhead
- **Simplicity** - Easy to reason about and debug

**Trade-offs:**
- ❌ Higher memory usage than tree-based structures
- ❌ All translations loaded at once (no lazy loading per locale)
- ✅ Predictable O(1) performance
- ✅ No complexity in managing partial loads
- ✅ Cache-friendly for repeated lookups

### 4. std::optional for Error Handling

**Decision:** Return `std::optional<std::string>` instead of throwing exceptions

**Rationale:**
- **Expected Failures** - Missing translations are normal, not exceptional
- **Performance** - No exception overhead in the common case
- **Explicit Handling** - Callers must acknowledge the possibility of failure
- **Modern C++** - Idiomatic C++17+ approach

**Trade-offs:**
- ❌ Requires C++17 or later
- ❌ Callers must check for nullopt
- ✅ Zero overhead when translation exists
- ✅ Compiler warnings for unchecked optionals (with appropriate flags)
- ✅ Composable with monadic operations (.and_then, .or_else in C++23)

**Alternative API Provided:**
```cpp
// For convenience when you always want a string
std::string translate(const std::string& id, const std::string& fallback);
```

### 5. Sophisticated Fallback Strategy

**Decision:** Multi-level fallback chain for missing translations

**Fallback Order:**
1. Exact locale match (`en-US`)
2. Base language match (`en` from `en-US`)
3. First available translation (any locale)
4. `std::nullopt` or provided fallback

**Rationale:**
- **Graceful Degradation** - Show something rather than nothing
- **Partial Translations** - Works even if only some locales are complete
- **Development Friendly** - Doesn't break when adding new languages

**Example:**
```cpp
translator.set_locale("en-GB");
// "hello.world" has: en-US, fr-FR, es-ES
// 1. Try en-GB: not found
// 2. Try base "en": matches "en-US" ✓
// Returns: "Hello, World!"
```

**Trade-offs:**
- ❌ May return unexpected locale if not careful
- ❌ Adds slight complexity to lookup logic
- ✅ Resilient to incomplete translations
- ✅ Better user experience than missing strings

### 6. Automatic Locale Detection

**Decision:** Auto-detect system locale at construction with platform-specific APIs

**Platform Implementations:**
- **Windows** - `GetUserDefaultLocaleName()` (Win32 API)
- **macOS** - `CFLocaleCopyCurrent()` (CoreFoundation)
- **Linux** - `LANG` environment variable parsing

**Rationale:**
- **Zero Configuration** - Works immediately without setup
- **User Expectations** - Respects system preferences by default
- **Override Available** - Can still call `set_locale()` explicitly

**Trade-offs:**
- ❌ Platform-specific code required
- ❌ Slight startup overhead (negligible, <1ms)
- ✅ Correct default behavior for 99% of use cases
- ✅ No API key or configuration needed

**Locale Format Standardization:**
All platforms normalized to: `language-COUNTRY` (e.g., `en-US`, `fr-FR`)
