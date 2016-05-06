include(ExternalProject)

if(IS_TRAVIS_BUILD)     # on travis, use git for fetching instead of wget
    set(GB_FETCH_EXTERNAL_OPENAL_SOFT
        GIT_REPOSITORY https://github.com/kcat/openal-soft.git
        GIT_TAG openal-soft-1.17.2)
else()
    set(GB_FETCH_EXTERNAL_OPENAL_SOFT
        URL https://github.com/kcat/openal-soft/archive/openal-soft-1.17.2.zip
        URL_HASH MD5=087449bf150dc4f3bc81e3052597115b)
endif()

ExternalProject_Add(OpenAL-Soft-External
    PREFIX ${CMAKE_BINARY_DIR}/external/OpenAL_Soft
    CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR> -DALSOFT_EXAMPLES=OFF
    ${GB_FETCH_EXTERNAL_OPENAL_SOFT}
)
