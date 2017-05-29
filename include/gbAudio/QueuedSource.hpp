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

class QueuedSource : public Playable
{
public:
    enum class BufferAction {
        Reinsert,
        Drop
    };
    using BufferProcessedCallback = std::function<BufferAction(Buffer& buffer)>;
public:
    virtual ~QueuedSource() = default;

    QueuedSource() = default;
    QueuedSource(QueuedSource const&) = delete;
    QueuedSource& operator=(QueuedSource const&) = delete;

    virtual void enqueueBuffer(Buffer& buffer) = 0;

    virtual void enqueueBuffer(Buffer& buffer, BufferProcessedCallback cb) = 0;

    virtual void pump() = 0;
};

}
#endif
