#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_HPP

/** @file
*
* @brief Audio convenience header.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/

#include <gbAudio/config.hpp>

namespace GHULBUS_AUDIO_NAMESPACE
{

/** Initialize the audio subsystem.
 * This function must be called before any other function in the GhulbusAudio library.
 * It initializes the static data that is used by the audio subsystem and dependent subsystems.
 * To free any allocated data, call shutdownAudio(). When calling this function multiple times,
 * each invocation must have a matching call to shutdownAudio() and only the last call will
 * actually perform the shutdown.
 * @attention It is not safe to call this function concurrently with any other function of any
 *            Ghulbus library, including initializeAudio() itself.
 * @see shutdownAudio()
 */
GHULBUS_AUDIO_API void initializeAudio();

/** Shuts down the audio subsystem.
 * Frees any data allocated by initializeAudio(). In case of multiple calls to initializeAudio(),
 * each call must be matched with a call to shutdownAudio() and only the last call will actually
 * perform the shutdown.
 * @attention It is not safe to call this function concurrently with any other function of any
 *            Ghulbus library, including shutdownAudio() itself.
 * @see initializeAudio()
 */
GHULBUS_AUDIO_API void shutdownAudio();

}
#endif
