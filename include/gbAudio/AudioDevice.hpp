#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_DEVICE_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_DEVICE_HPP

/** @file
*
* @brief Audio device.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/

#include <gbAudio/config.hpp>

#include <iosfwd>
#include <memory>
#include <string>
#include <vector>

namespace GHULBUS_AUDIO_NAMESPACE
{

class AudioDevice;
typedef std::unique_ptr<AudioDevice> AudioDevicePtr;

class AudioDevice {
public:
    enum class AudioBackend {
        Default = 0,
        OpenAL
    };
    struct DeviceIdentifier {
        std::string Name;
    };
    enum class ChannelFormat {
        Mono = 0,
        Stereo,
        MC_Quad,
        MC_5_1,
        MC_6_1,
        MC_7_1
    };
private:
    class AudioDeviceImpl;
    std::unique_ptr<AudioDeviceImpl> m_impl;
public:
    AudioDevice(AudioDevice const&) = delete;
    AudioDevice& operator=(AudioDevice const&) = delete;

    AudioDevice();
    AudioDevice(AudioBackend audio_backend);
    AudioDevice(AudioBackend audio_backend, DeviceIdentifier const& device_id);
    ~AudioDevice();

    std::vector<ChannelFormat> getSupportedChannelFormats() const;

    static std::vector<DeviceIdentifier> enumerateDevices(AudioBackend audio_backend);
};

std::ostream& operator<<(std::ostream& os, AudioDevice::ChannelFormat channel_format);

}
#endif
