#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_BUFFER_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_BUFFER_HPP

/** @file
*
* @brief Audio buffer.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/

#include <gbAudio/config.hpp>
#include <gbAudio/AudioFwd.hpp>
#include <gbAudio/Data.hpp>

namespace GHULBUS_AUDIO_NAMESPACE
{
    class Buffer {
    public:
        Buffer() = default;
        Buffer(Buffer const&) = delete;
        Buffer& operator=(Buffer const&) = delete;

        virtual ~Buffer() = default;

        virtual void setData(DataVariant const& data) = 0;
    };
}
#endif
