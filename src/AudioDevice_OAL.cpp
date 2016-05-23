#include <gbAudio/impl/AudioDevice_OAL.hpp>

#include <gbBase/UnusedVariable.hpp>

namespace GHULBUS_AUDIO_NAMESPACE
{
namespace impl
{
AudioDevice_OAL::AudioDevice_OAL(AudioDevice::DeviceIdentifier const& device_id)
{
    // @todo
    GHULBUS_UNUSED_VARIABLE(device_id);
}

AudioDevice_OAL::~AudioDevice_OAL()
{
    // @todo
}

std::vector<AudioDevice::ChannelFormat> AudioDevice_OAL::getSupportedChannelFormats() const
{
    // @todo
    return std::vector<ChannelFormat>();
}

AudioBackend AudioDevice_OAL::getBackend() const
{
    return AudioBackend::OpenAL;
}
}
}
