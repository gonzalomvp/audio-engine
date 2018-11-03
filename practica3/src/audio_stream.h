#pragma once

#include <array>
#include <vector>

#include <stb_vorbis.h>

class AudioSource;

class AudioStream {
public:
	static AudioStream* load(const char* filename, AudioSource* source);
	~AudioStream();

	void setLooping(bool looping) { m_shouldLoop = looping; }

	static void updateAll();
protected:
	AudioStream() : m_source(nullptr), m_stream(nullptr), m_samplesLeft(0), m_shouldLoop(false) {}
	void update();
	bool stream(uint32_t buffer);
private:
	static std::vector<AudioStream*> m_streams;

	AudioSource*		    m_source;
	stb_vorbis*			    m_stream;
	stb_vorbis_info		    m_info;
	std::array<uint32_t, 2>	m_buffers;
	size_t			        m_samplesLeft;
	bool				    m_shouldLoop;
};
