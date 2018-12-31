
include_guard()

cmake_minimum_required (VERSION 3.11)
project(LabText)

set(LABTEXT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../LabCMake/scripts
                      ${CMAKE_CURRENT_SOURCE_DIR}/../LabCMake/modules)

include(Utilities)

lab_library(LabText
    TYPE STATIC
    ALIAS Lab::Text

    LIBRARIES

    INCLUDE_DIRS
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/src

    PUBLIC_HEADERS
        include/LabText/LabText.h

    PRIVATE_HEADERS
        src/LabText/TextScanner.hpp
        src/LabText/TextScannerTime.hpp
        src/LabText/config.h
        src/LabText/dict.h
        src/LabText/Dictionary.h
        src/LabText/ftoa.h

    CPPFILES
        src/LabText/dict.cpp
        src/LabText/Dictionary.cpp
        src/LabText/ftoa.cpp
        src/LabText/TextScanner.cpp
        src/LabText/TextScannerLib.cpp
)

IF(WIN32)
    target_compile_definitions(LabText PUBLIC PLATFORM_WINDOWS=1)
elseif(APPLE)
    target_compile_definitions(LabText PUBLIC PLATFORM_MACOS=1)
else()
    target_compile_definitions(LabText PUBLIC PLATFORM_LINUX=1)
endif()

