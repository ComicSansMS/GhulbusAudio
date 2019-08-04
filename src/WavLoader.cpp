
#include <gbAudio/WavLoader.hpp>

#include <gbAudio/DataFormat.hpp>

#include <gbBase/Exception.hpp>

#include <fstream>
#include <istream>
#include <string>

namespace GHULBUS_AUDIO_NAMESPACE
{

namespace {
    struct RIFFHeader {
        std::uint32_t ChunkID;
        std::uint32_t ChunkSize;
        std::uint32_t Format;
    };

    struct FmtChunk {
        std::uint32_t Subchunk1ID;
        std::uint32_t Subchunk1Size;
        std::uint16_t AudioFormat;
        std::uint16_t NumChannels;
        std::uint32_t SampleRate;
        std::uint32_t ByteRate;
        std::uint16_t BlockAlign;
        std::uint16_t BitsPerSample;
    };

    struct DataChunk {
        std::uint32_t Subchunk2ID;
        std::uint32_t Subchunk2Size;
    };

    inline unsigned int string_to_hex(char const str[4]) {
        return ((str[3] << 24) | (str[2] << 16) | (str[1] << 8) | (str[0]));
    }

    inline RIFFHeader parseHeader(std::istream& is)
    {
        RIFFHeader header;
        is.read(reinterpret_cast<char*>(&header), sizeof(header));
        if ((is.fail()) || (header.ChunkID != string_to_hex("RIFF")) || (header.Format != string_to_hex("WAVE"))) {
            GHULBUS_THROW(GHULBUS_BASE_NAMESPACE::Exceptions::IOError(),
                          "Invalid file header.");
        }
        return header;
    }

    inline FmtChunk parseFormatChunk(std::istream& is)
    {
        FmtChunk format_chunk;
        is.read(reinterpret_cast<char*>(&format_chunk), sizeof(format_chunk));
        if ((is.fail()) || (format_chunk.Subchunk1ID != string_to_hex("fmt "))) {
            GHULBUS_THROW(GHULBUS_BASE_NAMESPACE::Exceptions::IOError(),
                          "Invalid format chunk.");
        }
        if (format_chunk.AudioFormat != 1) {
            GHULBUS_THROW(GHULBUS_BASE_NAMESPACE::Exceptions::NotImplemented(),
                          "Only uncompressed PCM files supported.");
        }
        if ((format_chunk.BlockAlign != format_chunk.NumChannels * (format_chunk.BitsPerSample >> 3)) ||
            (format_chunk.ByteRate != format_chunk.SampleRate * format_chunk.BlockAlign))
        {
            GHULBUS_THROW(GHULBUS_BASE_NAMESPACE::Exceptions::IOError(),
                          "Format information corrupted.");
        }
        return format_chunk;
    }

    inline DataChunk parseDataChunk(std::istream& is)
    {
        DataChunk data_chunk;
        is.read(reinterpret_cast<char*>(&data_chunk), sizeof(data_chunk));
        if ((is.fail()) || (data_chunk.Subchunk2ID != string_to_hex("data"))) {
            GHULBUS_THROW(GHULBUS_BASE_NAMESPACE::Exceptions::IOError(),
                          "Invalid data chunk");
        }
        return data_chunk;
    }

    inline Format getDataFormat(FmtChunk const& format_chunk)
    {
        bool is_8_bit;
        switch (format_chunk.BitsPerSample)
        {
        case 8:  is_8_bit = true;  break;
        case 16: is_8_bit = false; break;
        default:
            GHULBUS_THROW(GHULBUS_BASE_NAMESPACE::Exceptions::NotImplemented(),
                          std::string("Unsupported bitrate \'") +
                          std::to_string(format_chunk.BitsPerSample) +
                          "\'.");
        }

        switch (format_chunk.NumChannels)
        {
        case 1: return (is_8_bit) ? (Format::Mono8) : (Format::Mono16);
        case 2: return (is_8_bit) ? (Format::Stereo8) : (Format::Stereo16);
        default:
            GHULBUS_THROW(GHULBUS_BASE_NAMESPACE::Exceptions::NotImplemented(),
                          std::string("Unsupported number of channels \'") +
                          std::to_string(format_chunk.NumChannels) +
                          "\'.");
        }
    }
}


WavLoader::WavLoader()
    :m_samplingFrequency(0), m_dataFormat(Format::Unknown), m_size(0), m_offset(0), m_loaderStream(nullptr)
{
}

void WavLoader::openWav(char const* filename)
{
    auto stream_storage = std::make_unique<std::ifstream>(filename, std::ios_base::binary);
    if (!(*stream_storage)) {
        GHULBUS_THROW(GHULBUS_BASE_NAMESPACE::Exceptions::IOError()
                        << GHULBUS_BASE_NAMESPACE::Exception_Info::filename(filename),
                      "File not found.");
    }
    openWav(*stream_storage);
    m_loaderStreamStorage = std::move(stream_storage);
}

void WavLoader::openWav(std::istream& is)
{
    parseHeader(is);
    auto const format_chunk = parseFormatChunk(is);

    auto const format = getDataFormat(format_chunk);
    auto const data_chunk = parseDataChunk(is);

    m_dataFormat = format;
    m_samplingFrequency = format_chunk.SampleRate;
    m_size = data_chunk.Subchunk2Size;
    m_offset = 0;
    m_loaderStream = &is;
    m_loaderStreamStorage.reset();
}

DataVariant WavLoader::readAll()
{
    return readChunk(m_size - m_offset);
}

DataVariant WavLoader::readChunk(std::size_t size)
{
    GHULBUS_PRECONDITION(size <= m_size - m_offset);
    auto ret = [this]() -> DataVariant {
        switch (m_dataFormat)
        {
        case Format::Mono8:    return DataMono8Bit(m_samplingFrequency);
        case Format::Stereo8:  return DataStereo8Bit(m_samplingFrequency);
        case Format::Mono16:   return DataMono16Bit(m_samplingFrequency);
        case Format::Stereo16: return DataStereo16Bit(m_samplingFrequency);
        default: GHULBUS_THROW(GHULBUS_BASE_NAMESPACE::Exceptions::ProtocolViolation(),
                               "Invalid format.");
        }
    }();
    GHULBUS_PRECONDITION_MESSAGE(size % DataOp::getSampleSize(ret) == 0,
                                 "Chunk size must be a multiple of file sample size.");

    DataOp::resize(ret, size / DataOp::getSampleSize(ret));
    m_loaderStream->read(DataOp::getRawData(ret), size);
    if (!(*m_loaderStream)) {
        GHULBUS_THROW(GHULBUS_BASE_NAMESPACE::Exceptions::IOError(), "Error while reading wav data.");
    }
    m_offset += size;
    return ret;
}

std::uint32_t WavLoader::getSamplingFrequency() const
{
    return m_samplingFrequency;
}

Format WavLoader::getFormat() const
{
    return m_dataFormat;
}

std::size_t WavLoader::getDataSize() const
{
    return m_size;
}

std::size_t WavLoader::getCurrentReadPosition() const
{
    return m_offset;
}
}
