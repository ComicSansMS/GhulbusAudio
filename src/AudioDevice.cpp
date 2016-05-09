#include <gbAudio/AudioDevice.hpp>

#include <gbBase/Assert.hpp>

#include <al.h>
#include <alc.h>

namespace GHULBUS_AUDIO_NAMESPACE
{
class AudioDevice::AudioDeviceImpl
{
};

AudioDevice::AudioDevice()
    :AudioDevice(AudioBackend::Default)
{}

AudioDevice::AudioDevice(AudioBackend audio_backend)
    :AudioDevice(audio_backend, DeviceIdentifier{""})
{}

AudioDevice::AudioDevice(AudioBackend audio_backend, DeviceIdentifier const& device_id)
    :m_impl(std::make_unique<AudioDeviceImpl>())
{
}

AudioDevice::~AudioDevice()
{
}

auto AudioDevice::getSupportedChannelFormats() const -> std::vector<ChannelFormat>
{
    return std::vector<ChannelFormat>();
}

/* static */ auto AudioDevice::enumerateDevices(AudioBackend audio_backend) -> std::vector<DeviceIdentifier>
{
    return std::vector<DeviceIdentifier>();
}

std::ostream& operator<<(std::ostream& os, AudioDevice::ChannelFormat channel_format)
{
    switch(channel_format) {
    case AudioDevice::ChannelFormat::Mono:    os << "Mono"; break;
    case AudioDevice::ChannelFormat::Stereo:  os << "Stereo (2/2.1)"; break;
    case AudioDevice::ChannelFormat::MC_Quad: os << "Multi-Channel (4/4.1)"; break;
    case AudioDevice::ChannelFormat::MC_5_1:  os << "Multi-Channel (5.1)"; break;
    case AudioDevice::ChannelFormat::MC_6_1:  os << "Multi-Channel (6.1)"; break;
    case AudioDevice::ChannelFormat::MC_7_1:  os << "Multi-Channel (7.1)"; break;
    default: GHULBUS_UNREACHABLE();
    }
    return os;
}

}
