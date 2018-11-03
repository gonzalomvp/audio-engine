#include "audio_source.h"

#include <al.h>

#include "audio_buffer.h"

AudioSource::AudioSource(AudioBuffer* buffer) {
	alGenSources(1, &m_alSource);
	setPitch(1.0f);
	setGain(1.0f);
	setLooping(false);
	setPosition(0.0f, 0.0f, 0.0f);
	setVelocity(0.0f, 0.0f, 0.0f);
	if (buffer) {
		alSourcei(m_alSource, AL_BUFFER, buffer->getAlBuffer());
	}
}

AudioSource::~AudioSource() {
	alDeleteSources(1, &m_alSource);
	if (m_stream) {
		delete m_stream;
	}
}

AudioSource* AudioSource::load(const char* filename) {
	AudioSource* audioSource = new AudioSource(nullptr);
	audioSource->m_stream = AudioStream::load(filename, audioSource);

	// Liberamos recursos si el stream no se ha creado correctamente
	if (!audioSource->m_stream) {
		delete audioSource;
		audioSource = nullptr;
	}

	return audioSource;
}