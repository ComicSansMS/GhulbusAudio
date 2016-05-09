#include <gbAudio/audio.hpp>
#include <al.h>

void foo()
{
    ALuint buf;
    alGenBuffers(1, &buf);
    alDeleteBuffers(1, &buf);
}

