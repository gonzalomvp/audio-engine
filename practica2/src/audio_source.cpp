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
	alSourcei(m_alSource, AL_BUFFER, buffer->getAlBuffer());
}

AudioSource::~AudioSource() {
	alDeleteSources(1, &m_alSource);
}
