#include <gbAudio/Audio.hpp>

#include <gbBase/Log.hpp>

namespace GHULBUS_AUDIO_NAMESPACE
{

void initializeAudio()
{
    GHULBUS_BASE_NAMESPACE::Log::initializeLogging();
}

void shutdownAudio()
{
    GHULBUS_BASE_NAMESPACE::Log::shutdownLogging();
}
}
