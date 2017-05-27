#include <gbAudio/Data.hpp>

#include <gbBase/Assert.hpp>

#include <catch.hpp>

#include <algorithm>
#include <sstream>

TEST_CASE("Data")
{
    using namespace GHULBUS_AUDIO_NAMESPACE;

    SECTION("Construction")
    {
        Data<DataFormat::Mono8> d(12345);
        constexpr auto format = d.getFormat();
        CHECK(format == Format::Mono8);
        CHECK(d.getSamplingFrequency() == 12345);
        constexpr auto sample_size = d.getSampleSize();
        CHECK(sample_size == 1);
        CHECK(d.getNumberOfSamples() == 0);
        CHECK(d.getTotalLength() == std::chrono::seconds(0));
    }

    SECTION("Storing Data")
    {
        DataStereo16Bit d(44100);
        CHECK(d.getSamplingFrequency() == 44100);
        d.resize(5);
        d[0].left  = 0;
        d[0].right = 1;
        d[1].left  = 2;
        d[1].right = 3;
        d[2].left  = 4;
        d[2].right = 5;
        d[3].left  = 6;
        d[3].right = 7;
        d[4].left  = 8;
        d[4].right = 9;
        CHECK(d.getNumberOfSamples() == 5);
        char const* raw_ptr = &d.getRawData()[3 * d.getSampleSize()];
        DataFormat::Stereo16 dd;
        std::memcpy(&dd, raw_ptr, sizeof(dd));
        CHECK(dd.left == 6);
        CHECK(dd.right == 7);
    }

    SECTION("Appending Data")
    {
        DataStereo16Bit d(44100);
        d.resize(3);
        d[0].left  = 0;
        d[0].right = 1;
        d[1].left  = 2;
        d[1].right = 3;
        d[2].left  = 4;
        d[2].right = 5;
        DataStereo16Bit d2(44100);
        d2.resize(2);
        d2[0].left  = 6;
        d2[0].right = 7;
        d2[1].left  = 8;
        d2[1].right = 9;
        d.append(d2);
        CHECK(d.getNumberOfSamples() == 5);
        for (std::size_t i = 0; i < d.getNumberOfSamples(); ++i) {
            CHECK(d[i].left == i*2);
            CHECK(d[i].right == i*2 + 1);
        }
    }

    SECTION("getTotalLength")
    {
        DataMono8Bit d(44100);
        CHECK(d.getTotalLength() == std::chrono::milliseconds(0));
        d.resize(44100 / 2);
        CHECK(d.getTotalLength() == std::chrono::milliseconds(500));
    }

    SECTION("Data Variant")
    {
        std::vector<DataVariant> datas;
        {
            DataStereo16Bit d(1);
            d.resize(10);
            datas.push_back(std::move(d));
        }
        {
            DataMono8Bit d(2);
            d.resize(20);
            datas.push_back(std::move(d));
        }
        CHECK(DataOp::getFormat(datas[0]) == Format::Stereo16);
        CHECK(DataOp::getFormat(datas[1]) == Format::Mono8);
        CHECK(DataOp::getSamplingFrequency(datas[0]) == 1);
        CHECK(DataOp::getSamplingFrequency(datas[1]) == 2);
        CHECK(DataOp::getSampleSize(datas[0]) == sizeof(DataFormat::Stereo16));
        CHECK(DataOp::getSampleSize(datas[1]) == sizeof(DataFormat::Mono8));
        CHECK(DataOp::getNumberOfSamples(datas[0]) == 10);
        CHECK(DataOp::getNumberOfSamples(datas[1]) == 20);
        CHECK(DataOp::getTotalLength(datas[0]) == std::chrono::seconds(10));
        CHECK(DataOp::getTotalLength(datas[1]) == std::chrono::seconds(10));
        CHECK(DataOp::getRawData(datas[0]) != nullptr);
        CHECK(DataOp::getRawData(datas[1]) != nullptr);
    }

}
