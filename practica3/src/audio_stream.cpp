#include "audio_stream.h"

#include <al.h>

#include "audio_source.h"

#define BLOCK_SIZE 32768

std::vector<AudioStream*> AudioStream::m_streams;

AudioStream* AudioStream::load(const char* filename, AudioSource* source) {
	// Abrimos el fichero
	stb_vorbis* oggFile = stb_vorbis_open_filename(filename, nullptr, nullptr);
	if (!oggFile) return nullptr;

	AudioStream* audioStream = new AudioStream();
	audioStream->m_stream = oggFile;
	audioStream->m_source = source;
	// Leemos los parametros del fichero OGG
	audioStream->m_info = stb_vorbis_get_info(oggFile);
	audioStream->m_samplesLeft = stb_vorbis_stream_length_in_samples(oggFile) * audioStream->m_info.channels;

	// Creamos dos buffers de audio
	alGenBuffers(2, audioStream->m_buffers.data());

	// Volcamos los datos a los buffers
	audioStream->stream(audioStream->m_buffers[0]);
	audioStream->stream(audioStream->m_buffers[1]);

	// Añadimos los buffers a la cola de reproduccion
	alSourceQueueBuffers(source->getAlSource(), 2, audioStream->m_buffers.data());
	
	// Registramos el stream creado
	m_streams.push_back(audioStream);

	return audioStream;
}

AudioStream::~AudioStream() {

	// Liberamos los buffers de audio
	alDeleteBuffers(2, m_buffers.data());

	m_streams.erase(
		std::remove_if(m_streams.begin(), m_streams.end(), [this](const AudioStream* streamInVector) -> bool { return this == streamInVector; })
		, m_streams.end());

	// Cerramos el fichero
	stb_vorbis_close(m_stream);
}

void AudioStream::updateAll() {
	for (size_t i = 0; i < m_streams.size(); ++i) {
		m_streams[i]->update();
	}
}

void AudioStream::update() {
	
	// Comprobamos si se ha terminado de procesar algun buffer
	int buffersProcessed;
	alGetSourcei(m_source->getAlSource(), AL_BUFFERS_PROCESSED, &buffersProcessed);

	for (int i = 0; i < buffersProcessed; ++i)
	{
		alSourceUnqueueBuffers(m_source->getAlSource(), 1, &m_buffers[i]);
		if (stream(m_buffers[i])) {
			alSourceQueueBuffers(m_source->getAlSource(), 1, &m_buffers[i]);
		}
		else if (m_shouldLoop) {
			stb_vorbis_seek_start(m_stream);
			m_samplesLeft = stb_vorbis_stream_length_in_samples(m_stream) * m_info.channels;
			if (stream(m_buffers[i])) {
				alSourceQueueBuffers(m_source->getAlSource(), 1, &m_buffers[i]);
			}
		}
	}

}

bool AudioStream::stream(uint32_t buffer) {
	std::array<short, BLOCK_SIZE> pcm;
	int size = stb_vorbis_get_samples_short_interleaved(m_stream, m_info.channels, pcm.data(), BLOCK_SIZE);
	if (size == 0) return false;

	ALenum format;
		switch (m_info.channels) {
		case 1:	format = AL_FORMAT_MONO16  ; break;
		case 2:	format = AL_FORMAT_STEREO16; break;
		default:return false;
	}

	alBufferData(buffer, format, pcm.data(), size * m_info.channels * sizeof(short), m_info.sample_rate);
	m_samplesLeft -= size;
	return true;
}