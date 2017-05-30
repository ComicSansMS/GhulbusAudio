#include <gbAudio/impl/Source_OAL.hpp>

#include <gbAudio/Exceptions.hpp>

#include <gbAudio/impl/Buffer_OAL.hpp>
#include <gbAudio/impl/Error_OAL.hpp>

#include <gbBase/Assert.hpp>
#include <gbBase/Exception.hpp>
#include <gbBase/Log.hpp>

namespace GHULBUS_AUDIO_NAMESPACE
{
namespace impl
{

Source_OAL::Source_OAL(ALuint source_id)
    :m_source(source_id)
{
}

Source_OAL::~Source_OAL()
{
    ErrorMonitor_OAL monitor;
    GHULBUS_LOG(Debug, "Deleting source #" << m_source << ".");
    alDeleteSources(1, &m_source);
}

void Source_OAL::play()
{
    GHULBUS_LOG(Trace, "Playing source #" << m_source << ".");
    ErrorMonitor_OAL monitor;
    alSourcePlay(m_source);
}

void Source_OAL::pause()
{
    GHULBUS_LOG(Trace, "Pausing source #" << m_source << ".");
    ErrorMonitor_OAL monitor;
    alSourcePause(m_source);
}

void Source_OAL::stop()
{
    GHULBUS_LOG(Trace, "Stopping source #" << m_source << ".");
    ErrorMonitor_OAL monitor;
    alSourceStop(m_source);
}

void Source_OAL::rewind()
{
    GHULBUS_LOG(Trace, "Rewinding source #" << m_source << ".");
    ErrorMonitor_OAL monitor;
    alSourceRewind(m_source);
}

Playable::State Source_OAL::getPlayState() const
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

void Source_OAL::loop(bool do_loop)
{
    ErrorMonitor_OAL monitor;
    alSourcei(m_source, AL_LOOPING, (do_loop) ? AL_TRUE : AL_FALSE);
}

bool Source_OAL::isLooping() const
{
    ALint ret;
    ErrorMonitor_OAL monitor;
    alGetSourcei(m_source, AL_LOOPING, &ret);
    return (ret == AL_TRUE);
}

void Source_OAL::bindBuffer(Buffer& buffer)
{
    GHULBUS_PRECONDITION_MESSAGE(getPlayState() == State::Stopped,
                                 "Source must be stopped before rebinding");

    auto& oal_buffer = dynamic_cast<Buffer_OAL&>(buffer);
    GHULBUS_LOG(Trace, "Binding buffer #" << oal_buffer.getBufferId_OAL() << " to source #" << m_source << ".");
    ErrorMonitor_OAL monitor;
    alSourcei(m_source, AL_BUFFER, oal_buffer.getBufferId_OAL());
    if(monitor.checkError()) {
        GHULBUS_THROW(Exceptions::OpenALError(), "Unable to bind OpenAL buffer to source.");
    }
}

void Source_OAL::detachBuffer()
{
    GHULBUS_PRECONDITION_MESSAGE(getPlayState() == State::Stopped,
                                 "Source must be stopped before rebinding");
    ErrorMonitor_OAL monitor;
    alSourcei(m_source, AL_BUFFER, 0);
    if(monitor.checkError()) {
        GHULBUS_THROW(Exceptions::OpenALError(), "Unable detach OpenAL buffer from source.");
    }
}

}
}
