#
# Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
#

# Build and run unit-tests.
option(BUILD_LINGUIST_TESTS "Build the testing tree." ON)

# Static analysis
if(WIN32 AND MSVC)
    set(BUILD_STATIC_ANALYSIS_MODE "VisualStudio" CACHE STRING "Enable static analysis.")
else()
    set(BUILD_STATIC_ANALYSIS_MODE "OFF" CACHE STRING "Enable static analysis.")
endif()
set_property(CACHE BUILD_STATIC_ANALYSIS_MODE PROPERTY STRINGS OFF VisualStudio)
mark_as_advanced(FORCE BUILD_STATIC_ANALYSIS_MODE)

# Build with address sanitizer.
option(BUILD_WITH_SANITIZERS "Build with address and undefined behavior sanitizers." OFF)
