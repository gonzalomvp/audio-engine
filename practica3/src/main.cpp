#ifdef _MSC_VER
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <iostream>
#include <vector>

#include <alc.h>
#include <glfw3.h>

#include "engine.h"

using namespace std;

#define AUDIO_FACTOR 0.1f

int main() {
	// Inicializamos GLFW
	if (!glfwInit()) {
		cout << "Error: No se ha podido inicializar GLFW" << endl;
		return -1;
	}
	atexit(glfwTerminate);

	// Creamos la ventana
	glfwWindowHint(GLFW_RESIZABLE, true);
	GLFWwindow* window = glfwCreateWindow(800, 600, "Programacion Audio", nullptr, nullptr);
	if (!window) {
		cout << "Error: No se ha podido crear la ventana" << endl;
		return -1;
	}

	// Activamos contexto de OpenGL
	glfwMakeContextCurrent(window);

	// Inicializamos LiteGFX
	lgfx_setup2d(800, 600);

	// Inicializamos OpenAL
	ALCdevice* alcDevice = alcOpenDevice(nullptr);

	// Activamos contexto de OpenAL
	ALCcontext* alcContext = alcCreateContext(alcDevice, nullptr);
	alcMakeContextCurrent(alcContext);

	// Version sin streaming
	//AudioBuffer* audioBuffer = AudioBuffer::load("data/mutant.ogg");
	//if (!audioBuffer) {
	//	cout << "Error: No se ha podido cargar el fichero de audio" << endl;
	//	return -1;
	//}
	//AudioSource* audioSource = new AudioSource(audioBuffer);

	// Version con streaming
	AudioSource* audioSource = AudioSource::load("data/mutant.ogg");

	if (!audioSource) {
		cout << "Error: No se ha podido cargar el fichero de audio" << endl;
		return -1;
	}

	// Reproducimos la fuente de audio
	audioSource->setLooping(true);
	audioSource->play();

	// Bucle principal
	double lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(window) && !glfwGetKey(window, GLFW_KEY_ESCAPE)) {
		// Actualizamos delta
		float deltaTime = static_cast<float>(glfwGetTime() - lastTime);
		lastTime = glfwGetTime();

		// Actualizamos tamaño de ventana
		int screenWidth, screenHeight;
		glfwGetWindowSize(window, &screenWidth, &screenHeight);
		lgfx_setviewport(0, 0, screenWidth, screenHeight);
		Vec2 screenSize(static_cast<float>(screenWidth), static_cast<float>(screenHeight));

		// Actualizacion de logica del programa
		AudioStream::updateAll();

		// Actualizamos ventana y eventos
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Eliminamos el buffer y la fuente de audio
	delete audioSource;

	// Descomentar en la version sin streaming
	//delete audioBuffer;

	// Eliminamos el contexto y el dispositivo de OpenAL
	alcDestroyContext(alcContext);
	alcCloseDevice(alcDevice);

	return 0;
}
