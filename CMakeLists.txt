cmake_minimum_required(VERSION 3.3)

project(GhulbusAudio)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if($ENV{TRAVIS})
    if($ENV{TRAVIS} STREQUAL "true")
        message(STATUS "Building on Travis-CI.")
        set(IS_TRAVIS_BUILD TRUE)
    endif()
endif()

option(GB_GENERATE_COVERAGE_INFO "If set, line coverage info will be generated from debug test runs." OFF)
if(GB_GENERATE_COVERAGE_INFO)
    if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
        # this is an undocumented option for changing the output extension from *.cpp.ext
        # to *.ext; As there is no reasonable way to obtain the list of object files
        # this seems to be the best option to generate the naming gcov expects.
        set(CMAKE_CXX_OUTPUT_EXTENSION_REPLACE 1)
        set(GCOV_COMMAND gcov CACHE STRING "Command to invoke to run gcov.")
    endif()
endif()

add_subdirectory(external)

###############################################################################
## Library gbAudio
###############################################################################

set(GB_AUDIO_VERSION_MAJOR 0)
set(GB_AUDIO_VERSION_MINOR 1)
set(GB_AUDIO_VERSION_PATCH 0)
set(GB_AUDIO_VERSION "${GB_AUDIO_VERSION_MAJOR}.${GB_AUDIO_VERSION_MINOR}.${GB_AUDIO_VERSION_PATCH}")

set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

set(GHULBUS_BASE_ROOT "" CACHE PATH "GhulbusBase installation root directory.")
if(GHULBUS_BASE_ROOT)
    list(APPEND CMAKE_PREFIX_PATH ${GHULBUS_BASE_ROOT}/cmake)
endif()
find_package(gbBase REQUIRED)

set(GB_AUDIO_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)
set(GB_AUDIO_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(GB_AUDIO_TEST_DIR ${PROJECT_SOURCE_DIR}/test)

set(GB_AUDIO_SOURCE_FILES
    ${GB_AUDIO_SOURCE_DIR}/audio.cpp
)

set(GB_AUDIO_HEADER_FILES
    ${GB_AUDIO_INCLUDE_DIR}/audio.hpp
)

set(GB_AUDIO_TEST_SOURCES
    ${GB_AUDIO_TEST_DIR}/TestAudio.cpp
)

add_library(gbAudio ${GB_AUDIO_SOURCE_FILES} ${GB_AUDIO_HEADER_FILES})
target_include_directories(gbAudio PUBLIC $<BUILD_INTERFACE:${GB_AUDIO_INCLUDE_DIR}>)
target_include_directories(gbAudio PUBLIC $<BUILD_INTERFACE:${Boost_INCLUDE_DIR}>)
target_include_directories(gbAudio PRIVATE $<BUILD_INTERFACE:${OPENAL_INCLUDE_DIR}>)
target_compile_definitions(gbAudio PUBLIC BOOST_ALL_NO_LIB)
target_link_libraries(gbAudio PUBLIC Threads::Threads)
target_link_libraries(gbAudio PUBLIC Ghulbus::gbBase)
target_link_libraries(gbAudio PRIVATE ${OPENAL_LIBRARY})

target_compile_definitions(gbAudio PUBLIC $<$<CONFIG:Debug>:GHULBUS_CONFIG_ASSERT_LEVEL_DEBUG>)
target_compile_options(gbAudio PRIVATE $<$<CXX_COMPILER_ID:MSVC>:/W4>)
target_compile_options(gbAudio PRIVATE
    $<$<OR:$<CXX_COMPILER_ID:AppleClang>,$<CXX_COMPILER_ID:Clang>>:-pedantic -Wall>)
target_compile_options(gbAudio PRIVATE $<$<CXX_COMPILER_ID:GNU>:-pedantic -Wall>)

option(BUILD_SHARED_LIBS "If checked, generate dynamic instead of static libraries." OFF)
include(GenerateExportHeader)
generate_export_header(gbAudio
    EXPORT_FILE_NAME ${PROJECT_BINARY_DIR}/generated/gbAudio/gbAudio_Export.hpp
    PREFIX_NAME GHULBUS_LIBRARY_
)
set(GB_AUDIO_GENERATED_HEADER_FILES ${PROJECT_BINARY_DIR}/generated/gbAudio/gbAudio_Export.hpp)
target_include_directories(gbAudio PUBLIC $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/generated>)
if(BUILD_SHARED_LIBS)
    set_property(TARGET gbAudio PROPERTY CXX_VISIBILITY_PRESET hidden)
    set_property(TARGET gbAudio PROPERTY VISIBILITY_INLINES_HIDDEN ON)
endif()

set_property(TARGET gbAudio PROPERTY VERSION ${GB_AUDIO_VERSION})
set_property(TARGET gbAudio PROPERTY SOVERSION ${GB_AUDIO_VERSION_MAJOR})
set_property(TARGET gbAudio PROPERTY DEBUG_POSTFIX d)

###############################################################################
## Testing gbAudio
###############################################################################

option(GB_BUILD_TESTS "Determines whether to build tests." ON)
if(GB_BUILD_TESTS)
    enable_testing()

    if(NOT TARGET Catch)
        include(ExternalProject)
        if(IS_TRAVIS_BUILD)     # on travis, use git for fetching instead of wget
            set(GB_FETCH_EXTERNAL_CATCH
                GIT_REPOSITORY https://github.com/philsquared/Catch.git
                GIT_TAG 0c1c9fa9229c7358ce16c9910004a39290b20f5f)
        elseif(WIN32)
            set(GB_FETCH_EXTERNAL_CATCH
                URL https://github.com/philsquared/Catch/archive/v1.2.1-develop.12.zip
                URL_HASH MD5=cda228922a1c9248364c99a3ff9cd9fa)
        else()
            set(GB_FETCH_EXTERNAL_CATCH
                URL https://github.com/philsquared/Catch/archive/v1.2.1-develop.12.tar.gz
                URL_HASH MD5=a8dfb7be899a6e7fb30bd55d53426122)
        endif()
        ExternalProject_Add(Catch-External
            PREFIX ${CMAKE_BINARY_DIR}/external/Catch
            ${GB_FETCH_EXTERNAL_CATCH}
            CONFIGURE_COMMAND ""
            BUILD_COMMAND ""
            INSTALL_COMMAND ${CMAKE_COMMAND} -E copy ${CMAKE_BINARY_DIR}/external/Catch/src/Catch-External/single_include/catch.hpp
                                                     ${CMAKE_BINARY_DIR}/external/Catch/include/catch.hpp
        )
        add_library(Catch INTERFACE)
        add_dependencies(Catch Catch-External)
        target_include_directories(Catch INTERFACE ${CMAKE_BINARY_DIR}/external/Catch/include)
    endif()

    add_executable(gbAudio_Test ${GB_AUDIO_TEST_SOURCES})
    target_link_libraries(gbAudio_Test gbAudio Catch)
    add_test(NAME TestAudio COMMAND gbAudio_Test)

    if(GB_GENERATE_COVERAGE_INFO AND (CMAKE_CXX_COMPILER_ID STREQUAL "GNU"))
        target_compile_options(gbAudio PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_libraries(gbAudio PRIVATE -fprofile-arcs -ftest-coverage)
        add_custom_target(gbAudio_TestCoverage
            COMMAND ${GCOV_COMMAND} -o CMakeFiles/gbAudio.dir/src ${GB_AUDIO_SOURCE_FILES}
            WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
            COMMENT "Collecting gcov coverage information"
        )
    endif()
endif()

###############################################################################
## Doxygen gbAudio
###############################################################################

option(GB_GENERATE_DOXYGEN_DOCUMENTATION "If checked, API documentation will be generated if Doxygen is available." ON)
if(GB_GENERATE_DOXYGEN_DOCUMENTATION)
    find_package(Doxygen)
    if(DOXYGEN_FOUND)
        set(GB_LIB_DOC_DIR ${PROJECT_BINARY_DIR}/doc)
        configure_file(${PROJECT_SOURCE_DIR}/doc/Doxyfile.in ${GB_LIB_DOC_DIR}/Doxyfile @ONLY)
        add_custom_target(doxygen-doc
            COMMAND ${DOXYGEN_EXECUTABLE}
            WORKING_DIRECTORY ${GB_LIB_DOC_DIR}
            COMMENT "Building doxygen documentation"
            SOURCES ${PROJECT_SOURCE_DIR}/doc/Doxyfile.in
        )
        source_group("" FILES ${PROJECT_SOURCE_DIR}/doc/Doxyfile.in)
    endif()
endif()


###############################################################################
## Packaging gbAudio
###############################################################################

set(GB_LIBRARY_NAMESPACE Ghulbus::)

install(TARGETS gbAudio EXPORT gbAudioTargets
    RUNTIME DESTINATION bin/$<CONFIG>
    LIBRARY DESTINATION lib/$<CONFIG>
    ARCHIVE DESTINATION lib/$<CONFIG>
    INCLUDES DESTINATION include
)
target_include_directories(gbAudio PUBLIC $<INSTALL_INTERFACE:include>)
install(FILES ${GB_AUDIO_HEADER_FILES} ${GB_AUDIO_GENERATED_HEADER_FILES} DESTINATION include/gbAudio)
if(MSVC AND BUILD_SHARED_LIBS)
    install(FILES $<TARGET_PDB_FILE:gbAudio> DESTINATION bin/Debug CONFIGURATIONS Debug)
    install(FILES $<TARGET_PDB_FILE:gbAudio> DESTINATION bin/RelWithDebInfo CONFIGURATIONS RelWithDebInfo)
endif()
install(FILES LICENSE DESTINATION ".")

if(GB_GENERATE_DOXYGEN_DOCUMENTATION)
    install(DIRECTORY ${GB_LIB_DOC_DIR}/html DESTINATION doc MESSAGE_NEVER)
endif()

include(CMakePackageConfigHelpers)
configure_package_config_file(${PROJECT_SOURCE_DIR}/cmake/gbAudioConfig.cmake.in
    ${PROJECT_BINARY_DIR}/cmake/gbAudioConfig.cmake
    INSTALL_DESTINATION cmake
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)
write_basic_package_version_file(
    ${PROJECT_BINARY_DIR}/cmake/gbAudioVersion.cmake
    VERSION ${GB_AUDIO_VERSION}
    COMPATIBILITY SameMajorVersion
)

configure_file(${PROJECT_SOURCE_DIR}/cmake/gbAudioMacros.cmake.in
    ${PROJECT_BINARY_DIR}/cmake/gbAudioMacros.cmake
    COPYONLY
)

# installation - build tree specific package config files
export(EXPORT gbAudioTargets FILE ${PROJECT_BINARY_DIR}/cmake/gbAudioTargets.cmake NAMESPACE ${GB_LIBRARY_NAMESPACE})
export(PACKAGE gbAudio)

# installation - relocatable package config files
install(EXPORT gbAudioTargets
    FILE gbAudioTargets.cmake
    NAMESPACE ${GB_LIBRARY_NAMESPACE}
    DESTINATION cmake
)
install(FILES
    ${PROJECT_BINARY_DIR}/cmake/gbAudioConfig.cmake
    ${PROJECT_BINARY_DIR}/cmake/gbAudioMacros.cmake
    ${PROJECT_BINARY_DIR}/cmake/gbAudioVersion.cmake
    DESTINATION cmake
)
