#include <gbAudio/AudioDevice.hpp>

#include <catch.hpp>

TEST_CASE("AudioDevice")
{
    using GHULBUS_AUDIO_NAMESPACE::AudioDevice;
    using GHULBUS_AUDIO_NAMESPACE::AudioBackend;

    auto dev = AudioDevice::create(AudioBackend::Default);
    REQUIRE(dev);
    CHECK(dev->getBackend() == AudioBackend::OpenAL);
}
