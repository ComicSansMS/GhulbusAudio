#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_IMPL_ERROR_OAL_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_IMPL_ERROR_OAL_HPP

/** @file
*
* @brief OpenAL error handling.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/

#include <gbAudio/config.hpp>

#include <gbBase/Log.hpp>

#include <al.h>

namespace GHULBUS_AUDIO_NAMESPACE
{
    namespace impl
    {
        /** Get the corresponding string to an OpenAL error code.
         */
        char const* translateErrorCode_OAL(ALenum err);

        /** Helper for OpenAL error handling.
         * Checks for floating errors on construction and destruction
         * and allows for manual error checking in betweeen.
         */
        class ErrorMonitor_OAL {
        private:
            ALenum error_;            ///< OpenAL error code
        public:
            /** Constructor.
             * Checks for floating errors and logs them to Warning.
             */
            ErrorMonitor_OAL()
            {
                if(checkError()) {
                    GHULBUS_LOG(Warning, "Floating OpenAL error detected: " << translateErrorCode_OAL(error_));
                }
            }

            /** Destructor.
             * Checks for floating errors and logs them to Warning.
             */
            ~ErrorMonitor_OAL()
            {
                if(checkError()) {
                    GHULBUS_LOG(Warning, "Floating OpenAL error detected: " << translateErrorCode_OAL(error_));
                }
            }

            /** Checks for new OpenAL errors by calling alGetError().
             * @return true if an error was found (i.e. alGetError() did not return AL_NO_ERROR).
             * @note Calling this resets the OpenAL's error state to AL_NO_ERROR.
             */
            bool checkError()
            {
                return ((error_ = alGetError()) != AL_NO_ERROR);
            }
        };
    }
}
#endif
