#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_QUEUED_SOURCE_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_QUEUED_SOURCE_HPP

/** @file
*
* @brief Queued Source.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/
#include <gbAudio/config.hpp>

#include <gbAudio/AudioFwd.hpp>
#include <gbAudio/Playable.hpp>

#include <functional>

namespace GHULBUS_AUDIO_NAMESPACE
{

/** A multi-buffer queued source.
 * This source maintains a queue of buffers for playback.
 * When a buffer has been exhausted, playback continues seamlessly from the next buffer in the queue.
 */
class QueuedSource : public Playable
{
public:
    /** Action to be performed by the QueuedSource upon return of BufferProcessedCallback.
     */
    enum class BufferAction {
        Reinsert,       ///< Reinsert the buffer into the source's queue.
        Drop            ///< Drop the buffer from the source's queue.
    };
    /** Callback invoked by pump() upon removing a buffer that has finished playing from the queue.
     * The \em buffer parameter contains a reference to the buffer that was removed from the queue.
     * The callee is free to fill the buffer with new data and have it re-inserted at the back of the queue
     * by returning BufferAction::Reinsert. Alternatively, when returning BufferAction::Drop the source
     * will discard the buffer and no longer access it. It is safe to delete the buffer from within the
     * callback when returning BufferAction::Drop.
     */
    using BufferProcessedCallback = std::function<BufferAction(Buffer& buffer)>;
public:
    virtual ~QueuedSource() = default;

    QueuedSource() = default;
    QueuedSource(QueuedSource const&) = delete;
    QueuedSource& operator=(QueuedSource const&) = delete;

    /** Enqueue a single buffer at the end of the queue.
     */
    virtual void enqueueBuffer(Buffer& buffer) = 0;

    /** Enqueue a single buffer at the end of the queue.
     * @param[in] cb Callback that will be invoked by pump() if the respective buffer has been completely processed.
     */
    virtual void enqueueBuffer(Buffer& buffer, BufferProcessedCallback cb) = 0;

    /** Queue pump function.
     * Invoking this function will pump the buffer queue, that is, it will remove any buffers that have finished
     * playing from the queue and invoke their respective \ref BufferProcessedCallback functions, if present.
     * Note that pumping is *not* required to advance playback from one buffer to the next.
     */
    virtual void pump() = 0;

    /** Clear the queue and detach all buffers.
     * @pre getPlayState() == State::Stopped.
     */
    virtual void clearQueue() = 0;
};

}
#endif
