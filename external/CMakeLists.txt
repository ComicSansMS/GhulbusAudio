
option(GB_USE_OPENAL "Use OpenAL as the audio backend." ON)
option(GB_BUILD_OPENAL_SOFT "Build OpenAL Soft as part of GhulbusAudio instead of searching the system for binaries." OFF)

if(GB_USE_OPENAL)
    if(GB_BUILD_OPENAL_SOFT)
        add_subdirectory(openal_soft)
        set(OPENAL_INCLUDE_DIR ${CMAKE_BINARY_DIR}/external/OpenAL_Soft/include/AL PARENT_SCOPE)
        set(OPENAL_LIBRARY ${CMAKE_BINARY_DIR}/external/OpenAL_Soft/lib/OpenAL32.lib PARENT_SCOPE)
    endif()
    find_package(OpenAL REQUIRED)
endif()
