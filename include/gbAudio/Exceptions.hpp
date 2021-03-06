#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_EXCEPTIONS_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_EXCEPTIONS_HPP

/** @file
*
* @brief Exceptions.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/

#include <gbAudio/config.hpp>

#include <gbBase/Exception.hpp>

namespace GHULBUS_AUDIO_NAMESPACE
{
/** Exception decorators.
 */
namespace Exception_Info {
    using namespace GHULBUS_BASE_NAMESPACE::Exception_Info;

    /** Decorator Tags.
     * Tags are empty types used to uniquely identify a decoratory type.
     */
    namespace Tags
    {
        struct GHULBUS_AUDIO_API invalid_enum_value { };
    }
    /** Decorator record types.
     */
    namespace Records
    {
    }

    /** @name Decorators
     * @{
     */
    /** An int representation of the invalid enum value.
     */
    using invalid_enum_value = ::GHULBUS_BASE_NAMESPACE::ErrorInfo<Tags::invalid_enum_value, int>;
    /// @}
}
namespace Exceptions
{
    using namespace GHULBUS_BASE_NAMESPACE::Exceptions;

    struct OpenALError : public GHULBUS_BASE_NAMESPACE::Exceptions::impl::ExceptionImpl
    {};
}
}
#endif
