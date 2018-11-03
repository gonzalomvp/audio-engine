#ifdef _MSC_VER
//#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
#endif

#include <iostream>
#include <vector>

#include <alc.h>
#include <glfw3.h>

#include "engine.h"

using namespace std;

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

	// Creamos un buffer de audio
	AudioBuffer* audioBuffer = AudioBuffer::load("data/music.wav");
	if (!audioBuffer) {
		cout << "Error: No se ha podido abrir el fichero de audio" << endl;
		return -1;
	}

	// Creamos una fuente de audio con el buffer
	AudioSource audioSource(audioBuffer);
	float pitch = 1.0f;
	float xPos  = 0.0f;
	audioSource.setPitch(pitch);
	audioSource.setPosition(xPos, 0.1f, 0.0f);

	// Reproducimos la fuente de audio
	audioSource.play();

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

		// Evaluamos el input
		if (glfwGetKey(window, GLFW_KEY_UP)) {
			pitch += 0.1f * deltaTime;
			audioSource.setPitch(pitch);
		}
		else if (glfwGetKey(window, GLFW_KEY_DOWN)) {
			pitch -= 0.1f * deltaTime;
			audioSource.setPitch(pitch);
		}
		else if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			xPos += 0.1f * deltaTime;
			audioSource.setPosition(xPos, 0.1f, 0.0f);
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			xPos -= 0.1f * deltaTime;
			audioSource.setPosition(xPos, 0.1f, 0.0f);
		}

		// Actualizacion de logica del programa
		
		// Pintado

		// Actualizamos ventana y eventos
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Eliminamos el contexto y el dispositivo de OpenAL
	alcDestroyContext(alcContext);
	alcCloseDevice(alcDevice);

	return 0;
}
