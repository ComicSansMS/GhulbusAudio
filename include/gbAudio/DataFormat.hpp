#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_DATA_FORMAT_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_DATA_FORMAT_HPP

/** @file
*
* @brief Audio data formats.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/
#include <gbAudio/config.hpp>

#include <cstdint>

namespace GHULBUS_AUDIO_NAMESPACE
{
enum class Format {
    Unknown = 0,                ///< Unknown format; Used to indicate absence of a valid format
    Mono8,                      ///< 8-bit Mono
    Stereo8,                    ///< 8-bit Stereo
    Mono16,                     ///< 16-bit Mono
    Stereo16                    ///< 16-bit Stereo
};

namespace DataFormat
{
    /** Retrieves the corresponding sound data descriptor value for a given layout type
     * @tparam T Format type; This may be any of the layout type from the SoundDataFormat namespace
     * @return Sound data format value for the given layout type
     */
    template<typename T>
    inline constexpr Format getDataFormat() { static_assert(sizeof(T) == 0, "Unsupported sound format"); }

    /** Layout type for 8-bit Mono sound data
     * 8-bit data is expressed as an unsigned value over the range 0 to 255,
     * 128 being an audio output level of zero.
     */
    struct Mono8 {
        std::uint8_t sample;
    };
    template<> inline constexpr Format getDataFormat<Mono8>() { return Format::Mono8; }

    /** Layout type for 8-bit Stereo sound data
     * 8-bit data is expressed as an unsigned value over the range 0 to 255,
     * 128 being an audio output level of zero.
     */
    struct Stereo8 {
        std::uint8_t left;
        std::uint8_t right;
    };
    template<> inline constexpr Format getDataFormat<Stereo8>() { return Format::Stereo8; }

    /* Layout type for 16-bit Mono sound data
     * 16-bit data is expressed as a signed value over the range -32768 to 32767,
     * 0 being an audio output level of zero.
     */
    struct Mono16 {
        std::int16_t sample;
    };
    template<> inline constexpr Format getDataFormat<Mono16>() { return Format::Mono16; }

    /** Layout type for 16-bit Stereo sound data
     * 16-bit data is expressed as a signed value over the range -32768 to 32767,
     * 0 being an audio output level of zero.
     */
    struct Stereo16 {
        std::int16_t left;
        std::int16_t right;
    };
    template<> inline constexpr Format getDataFormat<Stereo16>() { return Format::Stereo16; }
}
}
#endif
