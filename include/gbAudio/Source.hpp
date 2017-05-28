#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_SOURCE_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_SOURCE_HPP

/** @file
*
* @brief Audio Source.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/
#include <gbAudio/config.hpp>

#include <gbAudio/AudioFwd.hpp>
#include <gbAudio/Playable.hpp>

namespace GHULBUS_AUDIO_NAMESPACE
{

class Source : public Playable
{
public:
    virtual ~Source() = default;

    Source() = default;
    Source(Source const&) = delete;
    Source& operator=(Source const&) = delete;

    virtual void bindBuffer(Buffer& buffer) = 0;
};

}
#endif
