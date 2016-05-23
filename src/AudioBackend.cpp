#include <gbAudio/AudioBackend.hpp>

#include <ostream>

namespace GHULBUS_AUDIO_NAMESPACE
{
std::ostream& operator<<(std::ostream& os, AudioBackend audio_backend)
{
    switch(audio_backend)
    {
    case AudioBackend::Default: os << "Default";         break;
    case AudioBackend::OpenAL:  os << "OpenAL";          break;
    default:                    os << "Unknown backend"; break;
    }
    return os;
}
}
