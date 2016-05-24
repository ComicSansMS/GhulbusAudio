#include <gbAudio/AudioDevice.hpp>

#include <catch.hpp>

TEST_CASE("AudioDevice")
{
    using GHULBUS_AUDIO_NAMESPACE::AudioDevice;
    using GHULBUS_AUDIO_NAMESPACE::AudioBackend;

    auto device_list = AudioDevice::enumerateDevices(AudioBackend::OpenAL);
    CHECK(!device_list.empty());
    CHECK(std::none_of(begin(device_list), end(device_list), [](auto dev_id) { return dev_id.name.empty(); }));

    auto dev = AudioDevice::create(AudioBackend::Default);
    REQUIRE(dev);
    CHECK(dev->getBackend() == AudioBackend::OpenAL);
}
