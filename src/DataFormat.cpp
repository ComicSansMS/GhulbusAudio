#include <gbAudio/DataFormat.hpp>

#include <ostream>

namespace GHULBUS_AUDIO_NAMESPACE
{

std::ostream& operator<<(std::ostream& os, Format format)
{
    switch (format) {
    case Format::Unknown:   os << "Unknown format"; break;
    case Format::Mono8:     os << "8-bit Mono"; break;
    case Format::Stereo8:   os << "8-bit Stereo"; break;
    case Format::Mono16:    os << "16-bit Mono"; break;
    case Format::Stereo16:  os << "16-bit Stereo"; break;
    }
    return os;
}

}
