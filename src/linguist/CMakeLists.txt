#
# Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
#

# Define the target.
add_library(linguist_translator
    "translator.cxx"
    $<$<PLATFORM_ID:Darwin>:translator-darwin.mm>
    $<$<PLATFORM_ID:Linux>:translator-linux.cxx>
    $<$<PLATFORM_ID:Windows>:translator-win32.cxx>
)

# Create the alias target.
add_library(linguist::translator ALIAS linguist_translator)

# Apply the default target settings.
set_target_defaults(linguist_translator)

# Set the target include directories.
target_include_directories(linguist_translator
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# Link the dependent libraries.
target_link_libraries(linguist_translator
    PRIVATE
        $<BUILD_INTERFACE:nlohmann_json::nlohmann_json>
)

# Set the linker options.
target_link_options(linguist_translator
    PUBLIC
        $<$<PLATFORM_ID:Darwin>:-framework CoreFoundation>
)

# Installation
include(GNUInstallDirs)

install(TARGETS linguist_translator
    EXPORT LinguistTargets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Install public headers
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hxx"
)
