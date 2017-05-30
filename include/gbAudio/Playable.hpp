#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_PLAYABLE_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_PLAYABLE_HPP

/** @file
*
* @brief Playable interface.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/
#include <gbAudio/config.hpp>

namespace GHULBUS_AUDIO_NAMESPACE
{

/** Common interface for sources capable of playing sounds.
 */
class Playable
{
public:
    /** State of audio playback.
     */
    enum class State
    {
        Stopped,        ///< Stopped.
        Playing,        ///< Playing.
        Paused          ///< Paused.
    };
public:
    virtual ~Playable() = default;

    Playable() = default;
    Playable(Playable const&) = delete;
    Playable& operator=(Playable const&) = delete;

    /** Start playback.
     * If the source is already playing, calling Play() will cause it to restart from the beginning.
     * If the source is paused, it will resume playing where it left off.
     * Once playback has completed, the source will automatically transition to the Stopped state.
     */
    virtual void play() = 0;

    /** Pause playback.
     * Playback will be paused until the next call to Play().
     * Calling Pause on a source that is paused or stopped has no effect.
     */
    virtual void pause() = 0;

    /** Stop playback.
     * Playback will be stopped. On the next call to Play() playback will restart from the beginning.
     * Calling on a stopped source has no effect.
     */
    virtual void stop() = 0;

    /** Rewind playback.
     * If called on a playing or paused source, playback  will restart at the beginning.
     * Has no effect on a stopped source.
     */
    virtual void rewind() = 0;

    /** Retrieve the current play state.
     * @return Current state of playback.
     */
    virtual State getPlayState() const = 0;

    /** Toggle loop repeat.
     * @param[in] If true, the source will automatically rewind when reaching the end of audio data.
     */
    virtual void loop(bool do_loop) = 0;

    /** Check loop status.
     * @return true if loop repeat is enabled; false otherwise.
     */
    virtual bool isLooping() const = 0;
};

}
#endif
