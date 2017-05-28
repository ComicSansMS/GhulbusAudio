#include <gbAudio/impl/Error_OAL.hpp>

namespace GHULBUS_AUDIO_NAMESPACE
{
namespace impl
{

char const* translateErrorCode_OAL(ALenum err)
{
    switch (err) {
    case AL_NO_ERROR:          return "No error";
    case AL_INVALID_NAME:      return "Invalid name";
    case AL_INVALID_ENUM:      return "Invalid enum";
    case AL_INVALID_VALUE:     return "Invalid value";
    case AL_INVALID_OPERATION: return "Invalid operation";
    case AL_OUT_OF_MEMORY:     return "Out of memory";
    default:                   return "Unknown error";
    };
}

}
}
