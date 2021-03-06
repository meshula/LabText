
include_guard()

cmake_minimum_required (VERSION 3.11)
project(LabText C CXX)

set(LABTEXT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})

set(PUBLIC_HEADERS
    include/LabText/LabText.h
)

set(CPPFILES
    LabTextODR.cpp
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

configure_file(LabTextConfig.cmake.in "${PROJECT_BINARY_DIR}/LabTextConfig.cmake" @ONLY)

install(FILES
  "${PROJECT_BINARY_DIR}/LabTextConfig.cmake"
  DESTINATION "${CMAKE_INSTALL_PREFIX}"
)

