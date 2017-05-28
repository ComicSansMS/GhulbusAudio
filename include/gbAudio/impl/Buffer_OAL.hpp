#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_IMPL_BUFFER_OAL_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_IMPL_BUFFER_OAL_HPP

/** @file
*
* @brief Audio buffer implementation OpenAL.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/

#include <gbAudio/config.hpp>
#include <gbAudio/Buffer.hpp>

#include <al.h>

namespace GHULBUS_AUDIO_NAMESPACE
{
    namespace impl
    {
        class Buffer_OAL : public Buffer {
        private:
            ALuint m_buffer;
        public:
            Buffer_OAL(ALuint buffer_id);

            ~Buffer_OAL() override;

            /** @name Implementation of Buffer
            * @{
            */
            void setData(DataVariant const& data) override;
            /// @}
        };
    }
}
#endif
