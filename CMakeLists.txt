
include_guard()

cmake_minimum_required (VERSION 3.19)
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

install(TARGETS LabText
    EXPORT LabText
    LIBRARY DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    ARCHIVE DESTINATION "${CMAKE_INSTALL_PREFIX}/lib"
    RUNTIME DESTINATION "${CMAKE_INSTALL_PREFIX}/bin"
    PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_PREFIX}/include")

install(FILES
  "${PROJECT_BINARY_DIR}/LabTextConfig.cmake" DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake"
)

add_executable(TestSexpr TestSexpr.cpp)
target_link_libraries(TestSexpr Lab::Text)

add_executable(Landru Landru.cpp)
target_link_libraries(Landru Lab::Text)
