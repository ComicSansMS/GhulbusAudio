#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_IMPL_QUEUED_SOURCE_OAL_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_IMPL_QUEUED_SOURCE_OAL_HPP

/** @file
*
* @brief Queued Source implementation OpenAL.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/

#include <gbAudio/config.hpp>
#include <gbAudio/QueuedSource.hpp>

#include <al.h>

#include <deque>

namespace GHULBUS_AUDIO_NAMESPACE
{
namespace impl
{

class Buffer_OAL;

class QueuedSource_OAL : public QueuedSource {
private:
    ALuint m_source;
    struct QueueElement {
        Buffer_OAL* buffer;
        BufferProcessedCallback cb;

        QueueElement(Buffer_OAL* nbuffer, BufferProcessedCallback ncb) : buffer(nbuffer), cb(std::move(ncb)) {}
        QueueElement(QueueElement const&) = default;
        QueueElement& operator=(QueueElement const&) = default;
        QueueElement(QueueElement&&) = default;
        QueueElement& operator=(QueueElement&&) = default;
    };
    std::deque<QueueElement> m_queue;
    State m_userRequestedPlayState;
public:
    QueuedSource_OAL(ALuint source_id);

    ~QueuedSource_OAL() override;

    /** @name Implementation of QueuedSource
    * @{
    */

    /** @name Implementation of Playable
    * @{
    */
    void play() override;

    void pause() override;

    void stop() override;

    void rewind() override;

    State getPlayState() const override;

    void loop(bool do_loop) override;

    bool isLooping() const override;
    /// @}

    void enqueueBuffer(Buffer& buffer) override;

    void enqueueBuffer(Buffer& buffer, BufferProcessedCallback cb) override;

    void pump() override;

    void clearQueue() override;
    /// @}
};

}
}
#endif
