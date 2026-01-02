#
# Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
#

# Include the dependent modules.
include(FetchContent)

# JSON library.
FetchContent_Declare(nlohmann_json
    GIT_REPOSITORY  https://github.com/nlohmann/json.git
    GIT_TAG         v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

# Testing framework.
FetchContent_Declare(Catch2
  GIT_REPOSITORY    https://github.com/catchorg/Catch2.git
  GIT_TAG           v3.12.0
)
FetchContent_MakeAvailable(Catch2)
