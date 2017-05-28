#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_WAV_LOADER_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_WAV_LOADER_HPP

/** @file
*
* @brief Loader for WAV Files.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/
#include <gbAudio/config.hpp>
#include <gbAudio/Data.hpp>
#include <gbAudio/DataFormat.hpp>

#include <cstdint>
#include <memory>
#include <iosfwd>

namespace GHULBUS_AUDIO_NAMESPACE
{

class WavLoader
{
private:
    std::uint32_t m_samplingFrequency;
    Format m_dataFormat;
    std::size_t m_size;
    std::size_t m_offset;

    std::istream* m_loaderStream;
    std::unique_ptr<std::istream> m_loaderStreamStorage;

public:
    WavLoader();

    void openWav(char const* filename);
    void openWav(std::istream& is);

    DataVariant readAll();
    DataVariant readChunk(std::size_t size);

    std::uint32_t getSamplingFrequency() const;
    Format getFormat() const;
    std::size_t getDataSize() const;
    std::size_t getCurrentReadPosition() const;
};

}
#endif
