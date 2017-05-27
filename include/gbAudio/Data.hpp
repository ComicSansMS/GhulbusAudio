#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_DATA_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_DATA_HPP

/** @file
*
* @brief Audio data.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/
#include <gbAudio/config.hpp>
#include <gbAudio/DataFormat.hpp>

#include <gbBase/Assert.hpp>

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <memory>
#include <variant>
#include <vector>

namespace GHULBUS_AUDIO_NAMESPACE
{

/** Base type for carrying sound data.
 * @tparam Format_T Sound data layout type; This may be any of the layout type from the DataFormat namespace.
 */
template<typename DataFormat_T>
class Data
{
public:
    using FormatType = DataFormat_T;
private:
    std::vector<FormatType> m_data;
    std::uint32_t m_samplingFrequency;
public:
    /** Constructor.
     * Constructs an empty Data object.
     * @param[in] sampling_frequency Sampling frequency of stored sound data.
     */
    Data(std::uint32_t sampling_frequency)
        :m_samplingFrequency(sampling_frequency)
    {
    }

    Data(Data const&) = default;
    Data& operator=(Data const&) = default;
    Data(Data&&) = default;
    Data& operator=(Data&&) = default;

    /** Get the corresponding sound data descriptor value.
     * @return Sound data format value for the data.
     */
    constexpr Format getFormat() const
    {
        return DataFormat::getDataFormat<FormatType>();
    }

    /** Get the current sampling frequency for the sound data.
     * @return Sampling frequency.
     */
    std::uint32_t getSamplingFrequency() const
    {
        return m_samplingFrequency;
    }

    /** Get the size of a single sample in bytes.
     * @return Size of one sample.
     */
    constexpr std::size_t getSampleSize() const
    {
        return sizeof(FormatType);
    }

    /** Get the total number of samples present.
     * @return Number of samples.
     */
    std::size_t getNumberOfSamples() const
    {
        return m_data.size();
    }

    /** Get the total length of the sound data when played.
     * @return Playtime of the sound data.
     * @note This depends on both the number of samples and the sampling frequency.
     */
    std::chrono::milliseconds getTotalLength() const
    {
        auto const sample_size = getSampleSize();
        if(sample_size == 0) { return std::chrono::milliseconds(0); }
        return std::chrono::milliseconds((1000u * m_data.size()) / m_samplingFrequency);
    }

    /** Adjust the number of samples to a new size.
     * @param[in] new_size New number of samples.
     * @note If the new size is smaller than the original one, the data will be truncated.
     *       If the new size is larger, all newly appended entries will be uninitialized.
     *
     */
    void resize(std::size_t new_size)
    {
        m_data.resize(new_size);
    }

    /** Append sound data.
     * @param[in] other Additional sound data that will be inserted after the present data.
     *                  Both data must use the same sampling frequency.
     */
    void append(Data const& other)
    {
        GHULBUS_PRECONDITION(getSamplingFrequency() == other.getSamplingFrequency());
        auto const old_size = m_data.size();
        m_data.resize(old_size + other.m_data.size());
        std::memcpy(m_data.data() + old_size, other.getRawData(),
                    other.getNumberOfSamples() * other.getSampleSize());
    }

    /** Access the actual sound data.
     * @param[in] i Sample index [0..NumberOfSamples).
     * @return Sample at the given index.
     */
    FormatType& operator[](std::size_t i)
    {
        GHULBUS_PRECONDITION(i < m_data.size());
        return m_data[i];
    }

    /** Get a raw pointer to the sound data storage.
     * @return Pointer to the beginning of the sound data storage.
     * @pre NumberOfSamples must not be 0.
     * @note This is mainly used for passing data to the underlying sound API and
     *       should not be called manually.
     * @note The total size of the data storage in bytes is NumberOfSamples*SampleSize.
     */
    char const* getRawData() const
    {
        GHULBUS_PRECONDITION(!m_data.empty());
        return reinterpret_cast<char const*>(m_data.data());
    }
};

/** 8-bit Mono sound data.
 */
using DataMono8Bit = Data<DataFormat::Mono8>;
/** 8-bit Stereo sound data.
 */
using DataStereo8Bit = Data<DataFormat::Stereo8>;
/** 16-bit Mono sound data.
 */
using DataMono16Bit = Data<DataFormat::Mono16>;
/** 16-bit Stereo sound data.
 */
using DataStereo16Bit = Data<DataFormat::Stereo16>;

using DataVariant = std::variant<DataMono8Bit, DataStereo8Bit, DataMono16Bit, DataStereo16Bit>;

namespace DataOp
{
    Format getFormat(DataVariant const& dv)
    {
        return std::visit([](auto const& v) { return v.getFormat(); }, dv);
    }

    std::uint32_t getSamplingFrequency(DataVariant const& dv)
    {
        return std::visit([](auto const& v) { return v.getSamplingFrequency(); }, dv);
    }

    std::size_t getSampleSize(DataVariant const& dv)
    {
        return std::visit([](auto const& v) { return v.getSampleSize(); }, dv);
    }

    std::size_t getNumberOfSamples(DataVariant const& dv)
    {
        return std::visit([](auto const& v) { return v.getNumberOfSamples(); }, dv);
    }

    std::chrono::milliseconds getTotalLength(DataVariant const& dv)
    {
        return std::visit([](auto const& v) { return v.getTotalLength(); }, dv);
    }

    char const* getRawData(DataVariant const& dv)
    {
        return std::visit([](auto const& v) { return v.getRawData(); }, dv);
    }
}

}
#endif
