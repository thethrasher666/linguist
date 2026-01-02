#
# Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
#

# Set the minimum CMake version.
cmake_minimum_required(VERSION 3.27.0 FATAL_ERROR)

# Include pre-project definitions.
include("toolchain-defaults")

# Define the project.
project(linguist
    VERSION         1.0.0.0
    DESCRIPTION     "Linguist - A simple localisation library for C++"
    HOMEPAGE_URL    "https://www.pixel-slayer.com/"
    LANGUAGES       CXX
)

set(CMAKE_INSTALL_MESSAGE LAZY)

# Include the dependent modules.
include("CTest")
include("options")
include("packages")
include("targets")

# Add the source directory.
add_subdirectory("src")

# Enable testing.
if(BUILD_LINGUIST_TESTS)
    add_subdirectory("tests")
endif()

# Installation
include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

# Install the CMake helper function
install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/share/cmake/LinguistEmbedTranslations.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Linguist
)

# Configure package version file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/linguistConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion
)

# Configure package config file
configure_package_config_file(
    "${CMAKE_CURRENT_SOURCE_DIR}/cmake/linguistConfig.cmake.in"
    "${CMAKE_CURRENT_BINARY_DIR}/linguistConfig.cmake"
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Linguist
)

# Install CMake config files
install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/linguistConfig.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/linguistConfigVersion.cmake"
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Linguist
)

# Install documentation
install(FILES
    "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)

# Export targets
install(EXPORT LinguistTargets
    FILE LinguistTargets.cmake
    NAMESPACE linguist::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/Linguist
)
