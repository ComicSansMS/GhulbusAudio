#include <gbAudio/impl/Buffer_OAL.hpp>

#include <gbAudio/impl/Error_OAL.hpp>
#include <gbAudio/Exceptions.hpp>

#include <gbBase/Exception.hpp>
#include <gbBase/Log.hpp>

namespace GHULBUS_AUDIO_NAMESPACE
{
namespace
{

ALenum translateDataFormat(Format fmt)
{
    switch(fmt)
    {
    case Format::Mono8:    return AL_FORMAT_MONO8;
    case Format::Stereo8:  return AL_FORMAT_STEREO8;
    case Format::Mono16:   return AL_FORMAT_MONO16;
    case Format::Stereo16: return AL_FORMAT_STEREO16;
    }
    GHULBUS_UNREACHABLE();
}

}

namespace impl
{

Buffer_OAL::Buffer_OAL(ALuint buffer_id)
    :m_buffer(buffer_id)
{
}

Buffer_OAL::~Buffer_OAL()
{
    ErrorMonitor_OAL monitor;
    GHULBUS_LOG(Debug, "Deleting buffer #" << m_buffer << ".");
    alDeleteBuffers(1, &m_buffer);
}

void Buffer_OAL::setData(DataVariant const& data)
{
    ErrorMonitor_OAL monitor;
    auto const bytesize = DataOp::getNumberOfSamples(data) * DataOp::getSampleSize(data);
    if (bytesize > std::numeric_limits<ALsizei>::max()) {
        GHULBUS_THROW(GHULBUS_BASE_NAMESPACE::Exceptions::InvalidArgument(),
                      "Data is too big to fit into a single buffer.");
    }
    GHULBUS_LOG(Debug, "Writing " << bytesize << " bytes to buffer #" << m_buffer << ".");
    alBufferData(m_buffer, translateDataFormat(DataOp::getFormat(data)), DataOp::getRawData(data),
                 static_cast<ALsizei>(bytesize), DataOp::getSamplingFrequency(data));
    if (monitor.checkError()) {
        GHULBUS_THROW(Exceptions::OpenALError(), "Error writing data to buffer.");
    }
}

ALuint Buffer_OAL::getBufferId_OAL()
{
    return m_buffer;
}

}
}
