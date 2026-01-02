#
# Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
#

# Define the target.
add_executable(linguist-embed-tool
    "main.cxx"
)

# Apply the default target settings.
set_target_defaults(linguist-embed-tool)

# Link the dependent libraries.
target_link_libraries(linguist-embed-tool
    PRIVATE
        $<BUILD_INTERFACE:nlohmann_json::nlohmann_json>
)

# Installation - export as a target for downstream use
include(GNUInstallDirs)

install(TARGETS linguist-embed-tool
    EXPORT LinguistTargets
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
