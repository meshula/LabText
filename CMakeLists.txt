
cmake_minimum_required(VERSION 3.4)
project(LabText)
set(CMAKE_BUILD_TYPE Release)

set(LABTEXT_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(LOCAL_ROOT ${CMAKE_CURRENT_SOURCE_DIR}/../local)

# override cmake install prefix if it's not set,
# per: http://public.kitware.com/pipermail/cmake/2010-December/041135.html
IF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    SET(CMAKE_INSTALL_PREFIX ${LOCAL_ROOT} CACHE PATH "Install set to local" FORCE)
ENDIF(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)

set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/defaults
                      ${CMAKE_SOURCE_DIR}/cmake/modules
                      ${CMAKE_SOURCE_DIR}/cmake/macros)

include(CXXDefaults)
include(Utilities)
include(Packages)

add_library(LabText STATIC "")

if (MSVC_IDE)
    # hack to get around the "Debug" and "Release" directories cmake tries to add on Windows
    #set_target_properties(LabRender PROPERTIES PREFIX "../")
    set_target_properties(LabText PROPERTIES IMPORT_PREFIX "../")
endif()

target_include_directories(LabText PRIVATE "${LABTEXT_ROOT}/include")
target_include_directories(LabText PRIVATE "${LOCAL_ROOT}/include")
target_compile_definitions(LabText PRIVATE BUILDING_LABTEXT=1)

IF(WIN32)
    target_compile_definitions(LabText PUBLIC PLATFORM_WINDOWS=1)
elseif(APPLE)
    target_compile_definitions(LabText PUBLIC PLATFORM_MACOS=1)
else()
    target_compile_definitions(LabText PUBLIC PLATFORM_LINUX=1)
endif()

set_target_properties(LabText PROPERTIES OUTPUT_NAME_DEBUG LabTextd)

set_target_properties(LabText
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install (TARGETS LabText
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin)

source_file(src/LabText/config.h)
source_file(src/LabText/dict.cpp)
source_file(src/LabText/dict.h)
source_file(src/LabText/Dictionary.cpp)
source_file(src/LabText/Dictionary.h)
source_file(src/LabText/ftoa.cpp)
source_file(src/LabText/ftoa.h)
source_file(src/LabText/TextScanner.hpp)
source_file(src/LabText/TextScanner.cpp)
source_file(src/LabText/TextScanner.h)
source_file(src/LabText/TextScannerLib.cpp)
source_file(src/LabText/zmalloc.cpp)
source_file(src/LabText/zmalloc.h)

install(
    FILES
        src/LabText/TextScanner.h
        src/LabText/TextScanner.hpp

    DESTINATION
        include/LabText)
