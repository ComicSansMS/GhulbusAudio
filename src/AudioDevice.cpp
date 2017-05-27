#include <gbAudio/AudioDevice.hpp>

#include <gbAudio/Exceptions.hpp>

#include <gbAudio/impl/AudioDevice_OAL.hpp>

#include <gbBase/Assert.hpp>
#include <gbBase/UnusedVariable.hpp>

#include <al.h>
#include <alc.h>

namespace GHULBUS_AUDIO_NAMESPACE
{
/* static */
AudioDevicePtr AudioDevice::create()
{
    return AudioDevice::create(AudioBackend::Default);
}

/* static */
AudioDevicePtr AudioDevice::create(AudioBackend audio_backend)
{
    return AudioDevice::create(audio_backend, AudioDevice::DeviceIdentifier{""});
}

/* static */
AudioDevicePtr AudioDevice::create(AudioBackend audio_backend, AudioDevice::DeviceIdentifier const& device_id)
{
    switch(audio_backend)
    {
    case AudioBackend::Default:
    case AudioBackend::OpenAL:
        return std::make_unique<impl::AudioDevice_OAL>(device_id);
    default: break;
    }
    GHULBUS_UNREACHABLE_MESSAGE("Invalid audio backend.");
}

/* static */
std::vector<AudioDevice::DeviceIdentifier> AudioDevice::enumerateDevices(AudioBackend audio_backend)
{
    switch(audio_backend)
    {
    case AudioBackend::Default:
    case AudioBackend::OpenAL:
        return impl::AudioDevice_OAL::enumerateDevices_OAL();
    default: break;
    }
    GHULBUS_UNREACHABLE_MESSAGE("Invalid audio backend.");
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
