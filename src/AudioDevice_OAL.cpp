#include <gbAudio/impl/AudioDevice_OAL.hpp>

#include <gbAudio/Exceptions.hpp>

#include <gbBase/UnusedVariable.hpp>

#include <al.h>
#include <alc.h>

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
    std::vector<ChannelFormat> ret;
    ret.push_back(ChannelFormat::Mono);
    ret.push_back(ChannelFormat::Stereo);
    if(alGetEnumValue("AL_FORMAT_QUAD16"))  { ret.push_back(ChannelFormat::MC_Quad); }
    if(alGetEnumValue("AL_FORMAT_51CHN16")) { ret.push_back(ChannelFormat::MC_5_1); }
    if(alGetEnumValue("AL_FORMAT_61CHN16")) { ret.push_back(ChannelFormat::MC_6_1); }
    if(alGetEnumValue("AL_FORMAT_71CHN16")) { ret.push_back(ChannelFormat::MC_7_1); }
    return ret;
}

AudioBackend AudioDevice_OAL::getBackend() const
{
    return AudioBackend::OpenAL;
}

/* static */ std::vector<AudioDevice::DeviceIdentifier> AudioDevice_OAL::enumerateDevices_OAL()
{
    if(alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT") != ALC_TRUE) {
        GHULBUS_THROW(Exceptions::IOError(), "Device enumeration not supported.");
    }
    ALCchar const* device_names = 
        (alcIsExtensionPresent(nullptr, "ALC_ENUMERATE_ALL_EXT") == ALC_TRUE) ?
        alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER) :
        alcGetString(nullptr, ALC_DEVICE_SPECIFIER);
    // device names is a concatenation of zero terminated strings;
    // the end of the list is designated by an empty string.
    std::vector<DeviceIdentifier> ret;
    for(;;) {
        DeviceIdentifier devid;
        devid.name = device_names;
        if(devid.name.empty()) { break; }
        ret.push_back(devid);
        device_names += ret.back().name.length() + 1;
    }
    return ret;
}
}
}
