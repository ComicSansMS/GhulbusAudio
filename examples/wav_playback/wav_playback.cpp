#include <gbAudio/Audio.hpp>
#include <gbAudio/AudioDevice.hpp>
#include <gbAudio/Buffer.hpp>
#include <gbAudio/Data.hpp>
#include <gbAudio/Exceptions.hpp>
#include <gbAudio/Source.hpp>
#include <gbAudio/WavLoader.hpp>

#include <gbBase/Log.hpp>
#include <gbBase/LogHandlers.hpp>

#include <chrono>
#include <thread>

std::chrono::milliseconds load_wav_file(GhulbusAudio::Buffer& buffer)
{
    char const* filename = "andromeda.wav";
    GHULBUS_LOG(Info, "Reading wav file \"" << filename << "\".");
    GhulbusAudio::WavLoader wav_loader;
    wav_loader.openWav(filename);
    GHULBUS_LOG(Info, "Wav file is " << wav_loader.getFormat() << ", " << wav_loader.getSamplingFrequency() << "kHz.");
    GhulbusAudio::DataVariant const wav_data = wav_loader.readAll();
    GHULBUS_LOG(Info, "Read " << GhulbusAudio::DataOp::getNumberOfSamples(wav_data) << " samples; "
        << GhulbusAudio::DataOp::getTotalLength(wav_data).count() << "ms.");
    buffer.setData(wav_data);
    return GhulbusAudio::DataOp::getTotalLength(wav_data);
}

void wav_playback()
{
    GHULBUS_LOG(Info, "Initializing Audio...");
    GhulbusAudio::initializeAudio();
    std::vector<GhulbusAudio::AudioDevice::DeviceIdentifier> const device_list =
        GhulbusAudio::AudioDevice::enumerateDevices();
    GHULBUS_LOG(Info, "Found " << device_list.size() << " audio device" << (device_list.size() == 1 ? "" : "s") << ".");

    GHULBUS_LOG(Info, "Creating Audio Device...");
    GhulbusAudio::AudioDevicePtr audio_device = GhulbusAudio::AudioDevice::create();
    GHULBUS_LOG(Info, "Audio Device is \"" << audio_device->getDeviceIdentifier().name << "\".");

    GHULBUS_LOG(Info, "Creating Audio Buffer...");
    GhulbusAudio::BufferPtr buffer = audio_device->createBuffer();
    std::chrono::milliseconds const buffer_length = load_wav_file(*buffer);

    GHULBUS_LOG(Info, "Creating Audio Source...");
    GhulbusAudio::SourcePtr source = audio_device->createSource();

    GHULBUS_LOG(Info, "Binding Audio Buffer...");
    source->bindBuffer(*buffer);

    GHULBUS_LOG(Info, "Playback...");
    source->play();
    std::this_thread::sleep_for(buffer_length);

    GhulbusAudio::shutdownAudio();
}

int main()
{
    Ghulbus::Log::initializeLogging();
    Ghulbus::Log::setLogHandler(Ghulbus::Log::Handlers::logToCout);
    Ghulbus::Log::setLogLevel(Ghulbus::LogLevel::Trace);

    try {
        wav_playback();
    } catch (Ghulbus::Exception const& e) {
        GHULBUS_LOG(Error, "Exception: " << e.what());
    }

    Ghulbus::Log::shutdownLogging();
}
