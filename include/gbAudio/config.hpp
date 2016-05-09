#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_GBAUDIO_CONFIG_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_GBAUDIO_CONFIG_HPP

/** @file
 *
 * @brief General configuration for Audio.
 * @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
 */

#include <gbAudio/gbAudio_Export.hpp>

/** Specifies the API for a function declaration.
 * When building as a dynamic library, this is used to mark the functions that will be exported by the library.
 */
#define GHULBUS_AUDIO_API GHULBUS_LIBRARY_GBAUDIO_EXPORT

/** \namespace Ghulbus Audio namespace for the Ghulbus library.
 * The implementation internally always uses this macro `GHULBUS_AUDIO_NAMESPACE` to refer to the namespace.
 * When building GhulbusAudio yourself, you can globally redefine this macro to move to a different namespace.
 */
#ifndef GHULBUS_AUDIO_NAMESPACE
#   define GHULBUS_AUDIO_NAMESPACE GhulbusAudio
#endif

#endif
