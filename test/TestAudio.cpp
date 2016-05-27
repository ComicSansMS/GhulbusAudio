#define CATCH_CONFIG_MAIN
#include <catch.hpp>

#include <gbAudio/Audio.hpp>

TEST_CASE("Initialization and shutdown")
{
    GHULBUS_AUDIO_NAMESPACE::initializeAudio();
    GHULBUS_AUDIO_NAMESPACE::shutdownAudio();
}
