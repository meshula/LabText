
include_guard()

cmake_minimum_required (VERSION 3.11)
project(LabText)

set(LABTEXT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(PUBLIC_HEADERS
    include/LabText/LabText.h
)

set(PRIVATE_HEADERS
    src/LabText/TextScanner.hpp
    src/LabText/TextScannerTime.hpp
    src/LabText/config.h
    src/LabText/dict.h
    src/LabText/Dictionary.h
    src/LabText/ftoa.h
)

set(CPPFILES
    src/LabText/dict.cpp
    src/LabText/Dictionary.cpp
    src/LabText/ftoa.cpp
    src/LabText/TextScanner.cpp
    src/LabText/LabTextODR.cpp
)

add_library(LabText STATIC ${PUBLIC_HEADERS} ${PRIVATE_HEADERS} ${CPPFILES})
target_include_directories(LabText PUBLIC ${LABTEXT_ROOT}/include)
set_target_properties(
    LabText
    PROPERTIES
        FOLDER "LabText"
        POSITION_INDEPENDENT_CODE ON
        PUBLIC_HEADER "${PUBLIC_HEADERS}"
        OUTPUT_NAME "LabText"
        OUTPUT_NAME_DEBUG "LabText_d"
        INTERFACE_INCLUDE_DIRECTORIES ${LABTEXT_ROOT}/include
)

add_library(Lab::Text ALIAS LabText)

IF(WIN32)
    target_compile_definitions(LabText PUBLIC PLATFORM_WINDOWS=1)
elseif(APPLE)
    target_compile_definitions(LabText PUBLIC PLATFORM_MACOS=1)
else()
    target_compile_definitions(LabText PUBLIC PLATFORM_LINUX=1)
endif()

configure_file(cmake/LabTextConfig.cmake.in
  "${PROJECT_BINARY_DIR}/LabTextConfig.cmake" @ONLY)
install(FILES
  "${PROJECT_BINARY_DIR}/LabTextConfig.cmake"
  DESTINATION "${CMAKE_INSTALL_PREFIX}"
)
