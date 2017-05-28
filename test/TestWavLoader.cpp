#include <gbAudio/WavLoader.hpp>

#include <catch.hpp>

#include <sstream>

TEST_CASE("WavLoader")
{
    using namespace GHULBUS_AUDIO_NAMESPACE;

    SECTION("Wav File Loading")
    {
        char wav_file[] = { '\x52', '\x49', '\x46', '\x46', '\xE0', '\x5C', '\xF8', '\x02', '\x57', '\x41',
                            '\x56', '\x45', '\x66', '\x6D', '\x74', '\x20', '\x10', '\x00', '\x00', '\x00',
                            '\x01', '\x00', '\x02', '\x00', '\x44', '\xAC', '\x00', '\x00', '\x10', '\xB1',
                            '\x02', '\x00', '\x04', '\x00', '\x10', '\x00', '\x64', '\x61', '\x74', '\x61',
                            // size
                            '\x18', '\x00', '\x00', '\x00',
                            // data
                            '\x01', '\x00', '\x02', '\x00',
                            '\x03', '\x00', '\x04', '\x00',
                            '\x05', '\x00', '\x06', '\x00',
                            '\x07', '\x00', '\x08', '\x00',
                            '\x09', '\x00', '\x0A', '\x00',
                            '\x0B', '\x00', '\x0C', '\x00'
                            };
        std::stringstream sstr;
        sstr.write(wav_file, sizeof(wav_file));
        WavLoader loader;
        loader.openWav(sstr);
        CHECK(loader.getCurrentReadPosition() == 0);
        CHECK(loader.getSamplingFrequency() == 44100);
        CHECK(loader.getFormat() == Format::Stereo16);
        CHECK(loader.getDataSize() == 24);

        {
            auto data_variant = loader.readChunk(4);
            auto& data = std::get<DataStereo16Bit>(data_variant);
            CHECK(data.getFormat() == Format::Stereo16);
            CHECK(data.getNumberOfSamples() == 1);
            CHECK(data.getSamplingFrequency() == 44100);
            CHECK(data[0].left == 1);
            CHECK(data[0].right == 2);

            CHECK(loader.getCurrentReadPosition() == 4);
        }

        {
            auto data_variant = loader.readChunk(8);
            auto& data = std::get<DataStereo16Bit>(data_variant);
            CHECK(data.getFormat() == Format::Stereo16);
            CHECK(data.getNumberOfSamples() == 2);
            CHECK(data.getSamplingFrequency() == 44100);
            CHECK(data[0].left == 3);
            CHECK(data[0].right == 4);
            CHECK(data[1].left == 5);
            CHECK(data[1].right == 6);

            CHECK(loader.getCurrentReadPosition() == 12);
        }

        {
            auto data_variant = loader.readAll();
            auto& data = std::get<DataStereo16Bit>(data_variant);
            CHECK(data.getFormat() == Format::Stereo16);
            CHECK(data.getNumberOfSamples() == 3);
            CHECK(data.getSamplingFrequency() == 44100);
            CHECK(data[0].left == 7);
            CHECK(data[0].right == 8);
            CHECK(data[1].left == 9);
            CHECK(data[1].right == 10);
            CHECK(data[2].left == 11);
            CHECK(data[2].right == 12);

            CHECK(loader.getCurrentReadPosition() == loader.getDataSize());
        }
    }

}
