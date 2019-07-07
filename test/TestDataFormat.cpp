#include <gbAudio/DataFormat.hpp>

#include <sstream>

#include <catch.hpp>

TEST_CASE("DataFormat")
{
    using namespace GHULBUS_AUDIO_NAMESPACE::DataFormat;
    using GHULBUS_AUDIO_NAMESPACE::Format;

    SECTION("Data Format Type To Enum")
    {
        constexpr auto vm8 = getDataFormat<Mono8>();
        CHECK(vm8 == Format::Mono8);
        constexpr auto vs8 = getDataFormat<Stereo8>();
        CHECK(vs8 == Format::Stereo8);
        constexpr auto vm16 = getDataFormat<Mono16>();
        CHECK(vm16 == Format::Mono16);
        constexpr auto vs16 = getDataFormat<Stereo16>();
        CHECK(vs16 == Format::Stereo16);
    }

    SECTION("Data Format To String")
    {
        for (auto const format : {Format::Unknown, Format::Mono8, Format::Stereo8, Format::Mono16, Format::Stereo16}) {
            std::stringstream sstr;
            sstr << format;
            CHECK(!sstr.str().empty());
        }
    }

}
