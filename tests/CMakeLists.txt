#
# Copyright (c) 2026 Jamie Kenyon. All Rights Reserved.
#

if(NOT BUILD_TESTING)
    return()
endif()

# Create embedded translation file.
include("LinguistEmbedTranslations")
embed_translation_file(
    INPUT_FILE      "${CMAKE_CURRENT_SOURCE_DIR}/sample.json"
    OUTPUT_VARIABLE embedded_translation_file
)

# Define the target.
add_executable(sti-tests
    "test-basic.cxx"
    "test-embedding.cxx"
    ${embedded_translation_file}
)

# Apply the default target settings.
set_target_defaults(sti-tests)

# Add compile definitions.
target_compile_definitions(sti-tests
    PRIVATE
        EMBED_TOOL_PATH="$<TARGET_FILE:linguist-embed-tool>"
        TEST_DATA_FILE="${CMAKE_CURRENT_SOURCE_DIR}/sample.json"
)

# Link the dependent libraries.
target_link_libraries(sti-tests
    PRIVATE
        linguist::translator
        Catch2::Catch2WithMain
)

# Discover the tests.
include("Catch")
catch_discover_tests(sti-tests)
