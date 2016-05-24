#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_DEVICE_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_DEVICE_HPP

/** @file
*
* @brief Audio device.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/

#include <gbAudio/config.hpp>
#include <gbAudio/AudioBackend.hpp>

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
    struct DeviceIdentifier {
        std::string name;
    };
    enum class ChannelFormat {
        Mono = 0,
        Stereo,
        MC_Quad,
        MC_5_1,
        MC_6_1,
        MC_7_1
    };
public:
    AudioDevice() = default;
    AudioDevice(AudioDevice const&) = delete;
    AudioDevice& operator=(AudioDevice const&) = delete;

    virtual ~AudioDevice() {}

    virtual std::vector<ChannelFormat> getSupportedChannelFormats() const = 0;
    virtual AudioBackend getBackend() const = 0;

    GHULBUS_AUDIO_API static AudioDevicePtr create();
    GHULBUS_AUDIO_API static AudioDevicePtr create(AudioBackend audio_backend);
    GHULBUS_AUDIO_API static AudioDevicePtr create(AudioBackend audio_backend, DeviceIdentifier const& device_id);
    GHULBUS_AUDIO_API static std::vector<DeviceIdentifier> enumerateDevices(AudioBackend audio_backend);
};

GHULBUS_AUDIO_API std::ostream& operator<<(std::ostream& os, AudioDevice::ChannelFormat channel_format);
}
#endif
