#include "audio_buffer.h"

#include <fstream>
#include <string>
#include <vector>

#include <al.h>

AudioBuffer::~AudioBuffer() {
	alDeleteBuffers(1, &m_alBuffer);
}

AudioBuffer* AudioBuffer::load(const char* filename) {
	// Leemos contenido del fichero
	std::ifstream file(filename, std::ios::binary);
	if (!file.is_open()) return nullptr;
	
	// Comprobamos que es un fichero WAV valido
	std::vector<char> fileBuffer(4);
	file.read(fileBuffer.data(), 4);
	if(strncmp(fileBuffer.data(), "RIFF", 4) != 0) return nullptr;
	file.read(fileBuffer.data(), 4);
	uint32_t riffChunkSize = *reinterpret_cast<uint32_t*>(fileBuffer.data());
	file.read(fileBuffer.data(), 4);
	if (strncmp(fileBuffer.data(), "WAVE", 4) != 0) return nullptr;
	file.read(fileBuffer.data(), 4);
	if (strncmp(fileBuffer.data(), "fmt ", 4) != 0) return nullptr;
	
	// Leemos los parametros del fichero WAV
	file.read(fileBuffer.data(), 4);
	uint32_t fmtChunkSize = *reinterpret_cast<uint32_t*>(fileBuffer.data());
	fileBuffer.resize(fmtChunkSize);
	file.read(fileBuffer.data(), fmtChunkSize);
	uint16_t audioFormat   = *reinterpret_cast<uint16_t*>(fileBuffer.data());
	uint16_t channels      = *reinterpret_cast<uint16_t*>(fileBuffer.data() + 2);
	uint32_t sampleRate    = *reinterpret_cast<uint32_t*>(fileBuffer.data() + 4);
	uint32_t byteRate      = *reinterpret_cast<uint32_t*>(fileBuffer.data() + 8);
	uint16_t blockAlign    = *reinterpret_cast<uint16_t*>(fileBuffer.data() + 12);
	uint16_t bitsPerSample = *reinterpret_cast<uint16_t*>(fileBuffer.data() + 14);

	ALenum format;
	switch (channels + bitsPerSample) {
		case  9:	format = AL_FORMAT_MONO8   ; break;
		case 10:	format = AL_FORMAT_STEREO8 ; break;
		case 17:	format = AL_FORMAT_MONO16  ; break;
		case 18:	format = AL_FORMAT_STEREO16; break;
		default:	return nullptr;
	}

	// Buscamos el bloque data
	char chunkId[4];
	uint32_t chunkSize;
	do {
		file.read(chunkId, 4);
		fileBuffer.resize(4);
		file.read(fileBuffer.data(), 4);
		chunkSize = *reinterpret_cast<uint32_t*>(fileBuffer.data());
		fileBuffer.resize(chunkSize);
		file.read(fileBuffer.data(), chunkSize);
	} while (strncmp(chunkId, "data", 4) != 0);

	// Creamos el buffer de audio con la informacion obtenida
	AudioBuffer* audioBuffer = new AudioBuffer();
	alGenBuffers(1, &audioBuffer->m_alBuffer);
	alBufferData(audioBuffer->m_alBuffer, format, fileBuffer.data(), chunkSize, sampleRate);

	// Cerramos el fichero
	file.close();

	return audioBuffer;
}