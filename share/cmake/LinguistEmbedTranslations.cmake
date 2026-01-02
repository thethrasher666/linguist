#
# Copyright (c) 2025 Jamie Kenyon. All Rights Reserved.
#

#
# Embed translation JSON files into C++ source code
#
function(embed_translation_file)
    set(options "")
    set(oneValueArgs INPUT_FILE OUTPUT_VARIABLE)
    set(multiValueArgs "")
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    if(NOT ARG_INPUT_FILE)
        message(FATAL_ERROR "embed_translation_file: INPUT_FILE is required")
    endif()

    if(NOT ARG_OUTPUT_VARIABLE)
        message(FATAL_ERROR "embed_translation_file: OUTPUT_VARIABLE is required")
    endif()

    # Get absolute path for input
    if(NOT IS_ABSOLUTE "${ARG_INPUT_FILE}")
        set(ARG_INPUT_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${ARG_INPUT_FILE}")
    endif()

    # Auto-generate output filename in binary directory
    get_filename_component(INPUT_NAME "${ARG_INPUT_FILE}" NAME_WE)
    set(ARG_OUTPUT_FILE "${CMAKE_CURRENT_BINARY_DIR}/embedded_translations_${INPUT_NAME}.cxx")

    # Add custom command to generate the embedded file
    add_custom_command(
        OUTPUT "${ARG_OUTPUT_FILE}"
        COMMAND linguist-embed-tool "${ARG_INPUT_FILE}" "${ARG_OUTPUT_FILE}"
        DEPENDS linguist-embed-tool "${ARG_INPUT_FILE}"
        COMMENT "Embedding translations from ${ARG_INPUT_FILE}"
        VERBATIM
    )

    # Return the output file path to parent scope
    set(${ARG_OUTPUT_VARIABLE} "${ARG_OUTPUT_FILE}" PARENT_SCOPE)
endfunction()
