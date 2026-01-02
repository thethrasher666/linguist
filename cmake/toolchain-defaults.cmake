#
# Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
#

# Set the minimum MacOS version.
set(CMAKE_OSX_DEPLOYMENT_TARGET "15" CACHE STRING "Minimum OS X deployment version")


# Generate values from the current date that can be used in file configuration macros.
string(TIMESTAMP BUILD_YEAR  "%Y")
string(TIMESTAMP BUILD_MONTH "%m")
string(TIMESTAMP BUILD_DAY   "%d")
string(TIMESTAMP BUILD_TIME  "%H:%M:%S")
