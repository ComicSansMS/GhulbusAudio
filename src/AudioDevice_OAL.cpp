#include <gbAudio/impl/AudioDevice_OAL.hpp>

#include <gbAudio/impl/Buffer_OAL.hpp>
#include <gbAudio/impl/Error_OAL.hpp>
#include <gbAudio/impl/Source_OAL.hpp>
#include <gbAudio/impl/QueuedSource_OAL.hpp>
#include <gbAudio/Exceptions.hpp>

#include <gbBase/Log.hpp>
#include <gbBase/UnusedVariable.hpp>

#include <al.h>
#include <alc.h>

#include <memory>

namespace GHULBUS_AUDIO_NAMESPACE
{
namespace impl
{
struct AudioDevice_OAL::OALImpl
{
    std::unique_ptr<ALCdevice, std::function<void(ALCdevice*)>> device;
    std::unique_ptr<ALCcontext, std::function<void(ALCcontext*)>> context;

    OALImpl(AudioDevice::DeviceIdentifier const& device_id)
    {
        auto dev = alcOpenDevice(device_id.name.empty() ? nullptr : device_id.name.c_str());
        if(!dev) {
            GHULBUS_THROW(Exceptions::OpenALError(), "Error creating OpenAL device \"" + device_id.name + "\".");
        }
        device = decltype(device)(dev, [](ALCdevice* dev) {
                auto res = alcCloseDevice(dev);
                if(res != ALC_TRUE) {
                    GHULBUS_LOG(Error, "Error closing OpenAL device. "
                        "This is usually caused by a leaked context or buffer.");
                }
            });

        auto ctx = alcCreateContext(device.get(), nullptr);
        if(!ctx) {
            GHULBUS_THROW(Exceptions::OpenALError(), "Error creating OpenAL context.");
        }
        context = decltype(context)(ctx, [](ALCcontext* ctx) {
                alcMakeContextCurrent(nullptr);
                alcDestroyContext(ctx);
            });
        if(alcMakeContextCurrent(ctx) != ALC_TRUE) {
            GHULBUS_THROW(Exceptions::OpenALError(), "Error activating created OpenAL context.");
        }
    }
};

AudioDevice_OAL::AudioDevice_OAL(AudioDevice::DeviceIdentifier const& device_id)
    :m_impl(std::make_unique<OALImpl>(device_id))
{
}

AudioDevice_OAL::~AudioDevice_OAL()
{
    /* needed for pimpl destruction */
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
        GHULBUS_THROW(Exceptions::OpenALError(), "Device enumeration not supported.");
    }
    ALCchar const* device_names =
#ifdef ALC_ALL_DEVICES_SPECIFIER
        (alcIsExtensionPresent(nullptr, "ALC_ENUMERATE_ALL_EXT") == ALC_TRUE) ?
            alcGetString(nullptr, ALC_ALL_DEVICES_SPECIFIER) :
#endif
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

BufferPtr AudioDevice_OAL::createBuffer()
{
    ErrorMonitor_OAL monitor;
    ALuint buffer_id;
    alGenBuffers(1, &buffer_id);
    if (monitor.checkError()) {
        GHULBUS_THROW(Exceptions::OpenALError(), "Buffer creation failed.");
    }
    GHULBUS_LOG(Debug, "Creating audio buffer #" << buffer_id << ".");
    return std::make_unique<impl::Buffer_OAL>(buffer_id);
}

SourcePtr AudioDevice_OAL::createSource()
{
    ErrorMonitor_OAL monitor;
    ALuint source_id;
    alGenSources(1, &source_id);
    if (monitor.checkError()) {
        GHULBUS_THROW(Exceptions::OpenALError(), "Source creation failed.");
    }
    GHULBUS_LOG(Debug, "Creating audio source #" << source_id << ".");
    return std::make_unique<impl::Source_OAL>(source_id);
}

QueuedSourcePtr AudioDevice_OAL::createQueuedSource()
{
    ErrorMonitor_OAL monitor;
    ALuint source_id;
    alGenSources(1, &source_id);
    if(monitor.checkError()) {
        GHULBUS_THROW(Exceptions::OpenALError(), "Source creation failed.");
    }
    GHULBUS_LOG(Debug, "Creating queued audio source #" << source_id << ".");
    return std::make_unique<impl::QueuedSource_OAL>(source_id);
}

}
}
