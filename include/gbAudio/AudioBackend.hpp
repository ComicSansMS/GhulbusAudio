#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_BACKEND_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_BACKEND_HPP

/** @file
*
* @brief Audio backend enum.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/

#include <gbAudio/config.hpp>

#include <iosfwd>

namespace GHULBUS_AUDIO_NAMESPACE
{
enum class AudioBackend {
    Default = 0,
    OpenAL
};

GHULBUS_AUDIO_API std::ostream& operator<<(std::ostream& os, AudioBackend audio_backend);
}
#endif
