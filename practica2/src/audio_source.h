#pragma once

#include <stdint.h>

#include <al.h>

class AudioBuffer;

class AudioSource {
public:
	AudioSource(AudioBuffer* buffer);
	~AudioSource();

	uint32_t getAlSource() const                { return m_alSource; }

	void setPitch   (float pitch)               { alSourcef    (m_alSource, AL_PITCH, pitch); }
	void setGain    (float gain)                { alSourcef    (m_alSource, AL_GAIN, gain); }
	void setLooping (bool loop)                 { alSourcei    (m_alSource, AL_LOOPING, loop); }
	void setPosition(float x, float y, float z) { alSource3f   (m_alSource, AL_POSITION, x, y, z); }
	void setVelocity(float x, float y, float z) { alSource3f   (m_alSource, AL_VELOCITY, x, y, z); }
	void play       ()                          { alSourcePlay (m_alSource); }
	void stop       ()                          { alSourceStop (m_alSource); }
	void pause      ()                          { alSourcePause(m_alSource); }
	bool isPlaying  () const                    { ALint sourceState;  alGetSourcei(m_alSource, AL_SOURCE_STATE, &sourceState); return (sourceState == AL_PLAYING) ? true: false; }

private:
	uint32_t m_alSource;
};