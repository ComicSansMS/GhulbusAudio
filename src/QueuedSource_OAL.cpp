#include <gbAudio/impl/QueuedSource_OAL.hpp>

#include <gbAudio/impl/Buffer_OAL.hpp>
#include <gbAudio/impl/Error_OAL.hpp>

#include <gbAudio/Exceptions.hpp>

#include <gbBase/Assert.hpp>
#include <gbBase/Exception.hpp>
#include <gbBase/Log.hpp>


namespace GHULBUS_AUDIO_NAMESPACE
{
namespace impl
{

QueuedSource_OAL::QueuedSource_OAL(ALuint source_id)
    :m_source(source_id), m_userRequestedPlayState(State::Stopped)
{
}

QueuedSource_OAL::~QueuedSource_OAL()
{
    ErrorMonitor_OAL monitor;
    GHULBUS_LOG(Debug, "Deleting queued source #" << m_source << ".");
    alDeleteSources(1, &m_source);
}

void QueuedSource_OAL::play()
{
    GHULBUS_LOG(Trace, "Playing queued source #" << m_source << ".");
    ErrorMonitor_OAL monitor;
    alSourcePlay(m_source);
    m_userRequestedPlayState = State::Playing;
}

void QueuedSource_OAL::pause()
{
    GHULBUS_LOG(Trace, "Pausing queued source #" << m_source << ".");
    ErrorMonitor_OAL monitor;
    alSourcePause(m_source);
    m_userRequestedPlayState = State::Paused;
}

void QueuedSource_OAL::stop()
{
    GHULBUS_LOG(Trace, "Stopping queued source #" << m_source << ".");
    ErrorMonitor_OAL monitor;
    alSourceStop(m_source);
    m_userRequestedPlayState = State::Stopped;
}

void QueuedSource_OAL::rewind()
{
    GHULBUS_LOG(Trace, "Rewinding queued source #" << m_source << ".");
    ErrorMonitor_OAL monitor;
    alSourceRewind(m_source);
}

Playable::State QueuedSource_OAL::getPlayState() const
{
    ALint state;
    alGetSourcei(m_source, AL_SOURCE_STATE, &state);
    switch(state)
    {
    case AL_PLAYING: return State::Playing;
    case AL_PAUSED:  return State::Paused;
    default:         return State::Stopped;
    }
}

void QueuedSource_OAL::loop(bool do_loop)
{
    ErrorMonitor_OAL monitor;
    alSourcei(m_source, AL_LOOPING, (do_loop) ? AL_TRUE : AL_FALSE);
}

bool QueuedSource_OAL::isLooping() const
{
    ALint ret;
    ErrorMonitor_OAL monitor;
    alGetSourcei(m_source, AL_LOOPING, &ret);
    return (ret == AL_TRUE);
}

void QueuedSource_OAL::enqueueBuffer(Buffer& buffer)
{
    enqueueBuffer(buffer, [](Buffer&) { return BufferAction::Drop; });
}

void QueuedSource_OAL::enqueueBuffer(Buffer& buffer, BufferProcessedCallback cb)
{
    auto& oal_buffer = dynamic_cast<Buffer_OAL&>(buffer);
    ALuint const buffer_id = oal_buffer.getBufferId_OAL();
    m_queue.emplace_back(&oal_buffer, std::move(cb));
    ErrorMonitor_OAL monitor;
    alSourceQueueBuffers(m_source, 1, &buffer_id);
    if(monitor.checkError()) {
        m_queue.pop_back();
        GHULBUS_THROW(Exceptions::OpenALError(), "Error when queuing buffers to source.");
    }
}

void QueuedSource_OAL::pump()
{
    ALint buffers_processed;
    ErrorMonitor_OAL monitor;
    alGetSourcei(m_source, AL_BUFFERS_PROCESSED, &buffers_processed);
    if(buffers_processed == 0) { return; }
    std::vector<ALuint> free_buffers(buffers_processed);
    alSourceUnqueueBuffers(m_source, buffers_processed, free_buffers.data());
    if(monitor.checkError()) {
        GHULBUS_THROW(Exceptions::OpenALError(), "Error while unqueuing buffers from source.");
    }

    std::vector<ALuint> reinserted_buffers;
    reinserted_buffers.reserve(free_buffers.size());        // we assume that reinsertion is the typical case
    auto it = begin(free_buffers);
    while(it != end(free_buffers)) {
        auto const qe = m_queue.front();
        GHULBUS_ASSERT(qe.buffer->getBufferId_OAL() == *it);
        auto const buffer_action = qe.cb(*qe.buffer);
        if(buffer_action == BufferAction::Reinsert) {
            m_queue.push_back(qe);
            ++it;
        } else {
            GHULBUS_ASSERT(buffer_action == BufferAction::Drop);
            it = free_buffers.erase(it);
        }
        m_queue.pop_front();
    }

    if(!free_buffers.empty()) {
        alSourceQueueBuffers(m_source, static_cast<ALsizei>(free_buffers.size()), free_buffers.data());
        if(monitor.checkError()) {
            GHULBUS_THROW(Exceptions::OpenALError(), "Error while re-queuing buffers to source.");
        }
        // If a source stopped playing because it ran out of buffers, reinserting buffers will resume playback
        if(m_userRequestedPlayState == State::Playing && getPlayState() == State::Stopped)
        {
            alSourcePlay(m_source);
        }
    }
}

void QueuedSource_OAL::clearQueue()
{
    GHULBUS_PRECONDITION(getPlayState() == State::Stopped);
    ErrorMonitor_OAL monitor;
    std::vector<ALuint> free_buffers(m_queue.size());
    alSourceUnqueueBuffers(m_source, static_cast<ALsizei>(m_queue.size()), free_buffers.data());
    if(monitor.checkError()) {
        GHULBUS_THROW(Exceptions::OpenALError(), "Error while clearing buffer queue on source.");
    }
}

}
}
