#include <gbAudio/Audio.hpp>
#include <gbAudio/AudioDevice.hpp>

#include <catch.hpp>

#include <algorithm>

TEST_CASE("AudioDevice")
{
    using GHULBUS_AUDIO_NAMESPACE::AudioDevice;
    using GHULBUS_AUDIO_NAMESPACE::AudioBackend;

    GHULBUS_AUDIO_NAMESPACE::initializeAudio();

    SECTION("Device enumeration")
    {
        auto device_list = AudioDevice::enumerateDevices(AudioBackend::OpenAL);
        CHECK(!device_list.empty());
        CHECK(std::none_of(begin(device_list), end(device_list), [](auto dev_id) { return dev_id.name.empty(); }));
    }

    SECTION("Device creation")
    {
        auto dev = AudioDevice::create(AudioBackend::Default);
        REQUIRE(dev);
        CHECK(dev->getBackend() == AudioBackend::OpenAL);
    }

    SECTION("Channel format enumeration")
    {
        auto dev = AudioDevice::create(AudioBackend::Default);
        REQUIRE(dev);

        auto const channel_formats = dev->getSupportedChannelFormats();
        // mono and stereo must always be supported
        CHECK(std::find(begin(channel_formats), end(channel_formats), AudioDevice::ChannelFormat::Mono)
                != end(channel_formats));
        CHECK(std::find(begin(channel_formats), end(channel_formats), AudioDevice::ChannelFormat::Stereo)
            != end(channel_formats));
        // all formats must be valid values of the AudioDevice::ChannelFormat enum
        CHECK(std::all_of(begin(channel_formats), end(channel_formats), [](auto fmt) {
                return (fmt >= AudioDevice::ChannelFormat::Mono) && (fmt <= AudioDevice::ChannelFormat::MC_7_1);
            }));
        // no duplicates in the list
        auto unique_list = channel_formats;
        std::sort(begin(unique_list), end(unique_list));
        unique_list.erase(std::unique(begin(unique_list), end(unique_list)), end(unique_list));
        CHECK(unique_list.size() == channel_formats.size());
    }

    GHULBUS_AUDIO_NAMESPACE::shutdownAudio();
}
