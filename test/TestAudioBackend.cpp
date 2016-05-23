#include <gbAudio/AudioBackend.hpp>

#include <sstream>

#include <catch.hpp>

TEST_CASE("AudioBackend")
{
    using GHULBUS_AUDIO_NAMESPACE::AudioBackend;

    for(auto const e : {AudioBackend::Default, AudioBackend::OpenAL, static_cast<AudioBackend>(9999)})
    {
        std::stringstream sstr;
        sstr << e;
        CHECK(!sstr.str().empty());
    }
}
