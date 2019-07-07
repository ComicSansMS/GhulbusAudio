#include <gbAudio/Audio.hpp>
#include <gbAudio/AudioDevice.hpp>
#include <gbAudio/Buffer.hpp>
#include <gbAudio/Data.hpp>
#include <gbAudio/Exceptions.hpp>
#include <gbAudio/QueuedSource.hpp>
#include <gbAudio/WavLoader.hpp>

#include <gbBase/Log.hpp>
#include <gbBase/LogHandlers.hpp>

#include <algorithm>
#include <chrono>
#include <thread>

void queued_playback()
{
    char const* filename = "nearer.wav";

    GHULBUS_LOG(Info, "Initializing Audio...");
    GhulbusAudio::initializeAudio();
    std::vector<GhulbusAudio::AudioDevice::DeviceIdentifier> const device_list =
        GhulbusAudio::AudioDevice::enumerateDevices();
    GHULBUS_LOG(Info, "Found " << device_list.size() << " audio device" << (device_list.size() == 1 ? "" : "s") << ".");

    GHULBUS_LOG(Info, "Creating Audio Device...");
    GhulbusAudio::AudioDevicePtr audio_device = GhulbusAudio::AudioDevice::create();
    GHULBUS_LOG(Info, "Audio Device is \"" << audio_device->getDeviceIdentifier().name << "\".");

    GHULBUS_LOG(Info, "Creating Audio Buffers...");
    std::vector<GhulbusAudio::BufferPtr> buffers;
    std::generate_n(std::back_inserter(buffers), 5, [&]() { return audio_device->createBuffer(); });

    GHULBUS_LOG(Info, "Creating Queued Audio Source...");
    GhulbusAudio::QueuedSourcePtr queued_source = audio_device->createQueuedSource();

    GHULBUS_LOG(Info, "Reading wav file \"" << filename << "\".");
    GhulbusAudio::WavLoader wav_loader;
    wav_loader.openWav(filename);
    GHULBUS_LOG(Info, "Wav file is " << wav_loader.getFormat() << ", " << wav_loader.getSamplingFrequency() << "kHz.");

    std::size_t const chunk_size = wav_loader.getSamplingFrequency();
    std::size_t buffers_dropped = 0;
    auto const playback_cb = [&wav_loader, chunk_size, &buffers_dropped](GhulbusAudio::Buffer& buffer) {
        std::size_t const read_size = std::min(chunk_size, wav_loader.getDataSize() - wav_loader.getCurrentReadPosition());
        if (read_size == 0) {
            GHULBUS_LOG(Info, "End of file - Dropping buffer.");
            ++buffers_dropped;
            return GhulbusAudio::QueuedSource::BufferAction::Drop;
        }
        buffer.setData(wav_loader.readChunk(read_size));
        GHULBUS_LOG(Info, "Read " << (wav_loader.getCurrentReadPosition() * 100) / wav_loader.getDataSize() << "%.");
        return GhulbusAudio::QueuedSource::BufferAction::Reinsert;
    };

    GHULBUS_LOG(Info, "Queuing buffers....");
    for (auto& b : buffers) {
        queued_source->enqueueBuffer(*b, playback_cb);
    }

    GHULBUS_LOG(Info, "Begin playback....");
    queued_source->play();
    while (buffers_dropped < buffers.size()) {
        queued_source->pump();
        std::this_thread::sleep_for(std::chrono::milliseconds(125));
    }

    GhulbusAudio::shutdownAudio();
}

int main()
{
    Ghulbus::Log::initializeLogging();
    Ghulbus::Log::setLogHandler(Ghulbus::Log::Handlers::logToCout);
    Ghulbus::Log::setLogLevel(Ghulbus::LogLevel::Trace);

    try {
        queued_playback();
    } catch (Ghulbus::Exception const& e) {
        GHULBUS_LOG(Error, "Exception: " << e.what());
    }

    Ghulbus::Log::shutdownLogging();
}
