#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_IMPL_DEVICE_OAL_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_IMPL_DEVICE_OAL_HPP

/** @file
*
* @brief Audio device implementation OpenAL.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/

#include <gbAudio/config.hpp>
#include <gbAudio/AudioDevice.hpp>

namespace GHULBUS_AUDIO_NAMESPACE
{
namespace impl
{
class AudioDevice_OAL : public AudioDevice {
private:
    struct OALImpl;
    std::unique_ptr<OALImpl> m_impl;
public:
    AudioDevice_OAL(AudioDevice_OAL const&) = delete;
    AudioDevice_OAL& operator=(AudioDevice_OAL const&) = delete;

    AudioDevice_OAL(AudioDevice::DeviceIdentifier const& device_id);
    ~AudioDevice_OAL() override;

    /** @name Implementation of AudioDevice
     * @{
     */
    std::vector<ChannelFormat> getSupportedChannelFormats() const override;
    AudioBackend getBackend() const override;
    BufferPtr createBuffer() override;
    SourcePtr createSource() override;
    /// @}

    GHULBUS_AUDIO_API static std::vector<DeviceIdentifier> enumerateDevices_OAL();
};
}
}
#endif
