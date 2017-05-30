#ifndef GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_IMPL_SOURCE_OAL_HPP
#define GHULBUS_LIBRARY_INCLUDE_GUARD_AUDIO_IMPL_SOURCE_OAL_HPP

/** @file
*
* @brief Audio source implementation OpenAL.
* @author Andreas Weis (der_ghulbus@ghulbus-inc.de)
*/

#include <gbAudio/config.hpp>
#include <gbAudio/Source.hpp>

#include <al.h>

namespace GHULBUS_AUDIO_NAMESPACE
{
namespace impl
{

class Source_OAL : public Source {
private:
    ALuint m_source;
public:
    Source_OAL(ALuint source_id);

    ~Source_OAL() override;

    /** @name Implementation of Source
    * @{
    */

    /** @name Implementation of Playable
    * @{
    */
    void play() override;

    void pause() override;

    void stop() override;

    void rewind() override;

    State getPlayState() const override;

    void loop(bool do_loop) override;

    bool isLooping() const override;
    /// @}

    void bindBuffer(Buffer& buffer) override;

    void detachBuffer() override;
    /// @}
};

}
}
#endif
