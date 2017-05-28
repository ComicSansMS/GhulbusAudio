#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_FWD_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_FWD_HPP

/** @file
*
* @brief Forward declarations for gbAudio.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/

#include <gbAudio/config.hpp>
#include <memory>

namespace GHULBUS_AUDIO_NAMESPACE
{
    class AudioDevice;
    typedef std::unique_ptr<AudioDevice> AudioDevicePtr;

    class Buffer;
    typedef std::unique_ptr<Buffer> BufferPtr;
}
#endif
