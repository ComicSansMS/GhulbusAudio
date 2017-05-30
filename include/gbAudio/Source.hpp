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

#include <functional>

namespace GHULBUS_AUDIO_NAMESPACE
{

/** A simple single-buffer source.
 */
class Source : public Playable
{
public:
    Source() = default;
    Source(Source const&) = delete;
    Source& operator=(Source const&) = delete;

    virtual ~Source() = default;

    /** Bind a buffer for playback.
     * The same buffer may be bound to multiple sources.
     * The buffer must not be destroyed while it is still bound to any source.
     * Destroying the source will automatically detach the buffer.
     * @pre getPlayState() == State::Stopped
     */
    virtual void bindBuffer(Buffer& buffer) = 0;

    /** Detach the bound buffer, if any.
     * @pre getPlayState() == State::Stopped
     */
    virtual void detachBuffer() = 0;
};

}
#endif
