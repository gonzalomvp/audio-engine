#pragma once

#include <stdint.h>

class AudioBuffer {
public:
	~AudioBuffer();

	static AudioBuffer* load(const char* filename);
	uint32_t            getAlBuffer() const { return m_alBuffer; }

private:
	AudioBuffer() {}

	uint32_t m_alBuffer;
};