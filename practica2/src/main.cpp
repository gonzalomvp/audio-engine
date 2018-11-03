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

	// Creamos un buffer de audio
	AudioBuffer* audioBuffer = AudioBuffer::load("data/ambulance.wav");
	if (!audioBuffer) {
		cout << "Error: No se ha podido cargar el fichero de audio" << endl;
		return -1;
	}

	// Creamos una fuente de audio con el buffer
	AudioSource audioSource(audioBuffer);

	// Establecemos los valores iniciales
	Vec2 circleSize(20.0f, 20.0f);
	Vec2 halfCircleSize = circleSize * 0.5f;
	float angle = 0.0f;
	float listenerOffset = 0.0f;
	//alDopplerFactor(2);

	// Reproducimos la fuente de audio
	audioSource.setLooping(true);
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

		// Movemos el source alrededor del centro de la pantalla
		Vec2 screenCenter = screenSize * 0.5;
		// Calculamos la posicion inicial del source
		Vec2 dir(cos(angle * DEG2RAD), -sin(angle * DEG2RAD));
		Vec2 sourceOffset = dir * static_cast<float>(screenHeight) * 0.25f;
		Vec2 sourceInitialPos = screenCenter + sourceOffset;

		// Calculamos la posicion final del source
		angle += fmod(45.0f * deltaTime, 360.0f);
		dir = Vec2(cos(angle * DEG2RAD), -sin(angle * DEG2RAD));
		sourceOffset = dir * static_cast<float>(screenHeight) * 0.25f;
		Vec2 sourceEndPos = screenCenter + sourceOffset;

		// Calculamos la velocidad del source
		Vec2 sourceVel = (sourceEndPos - sourceInitialPos) / deltaTime;

		// Calculamos la posicion inicial del listener
		Vec2 listenerInitialPos = Vec2(screenWidth * 0.5f + listenerOffset, screenHeight * 0.75f);

		// Calculamos la posicion final del listener
		if (glfwGetKey(window, GLFW_KEY_RIGHT)) {
			listenerOffset += 100.0f * deltaTime;
		}
		else if (glfwGetKey(window, GLFW_KEY_LEFT)) {
			listenerOffset -= 100.0f * deltaTime;
		}
		Vec2 listenerEndPos = Vec2(screenWidth * 0.5f + listenerOffset, screenHeight * 0.75f);

		// Calculamos la velocidad del listener
		Vec2 listenerVel = (listenerEndPos - listenerInitialPos) / deltaTime;

		// Establecemos las posicion y velocidad del souce y listener aplicando un factor de correccion
		audioSource.setPosition(sourceEndPos.x   * AUDIO_FACTOR, sourceEndPos.y   * AUDIO_FACTOR, 0.0f);
		audioSource.setVelocity(sourceVel.x      * AUDIO_FACTOR, sourceVel.y      * AUDIO_FACTOR, 0.0f);
		setListenerPosition    (listenerEndPos.x * AUDIO_FACTOR, listenerEndPos.y * AUDIO_FACTOR, 0.0f);
		setListenerVelocity    (listenerVel.x    * AUDIO_FACTOR, listenerVel.y    * AUDIO_FACTOR, 0.0f);

		// Pintado
		lgfx_clearcolorbuffer(0, 0, 0);
		lgfx_setcolor(1, 1, 1, 1);
		lgfx_setblend(BLEND_SOLID);

		lgfx_drawoval(sourceEndPos.x   - halfCircleSize.x, sourceEndPos.y   - halfCircleSize.y, circleSize.x, circleSize.y);
		lgfx_drawoval(listenerEndPos.x - halfCircleSize.x, listenerEndPos.y - halfCircleSize.y, circleSize.x, circleSize.y);

		//printf("SOURCE POS (%f, %f)\n", sourceEndPos.x, sourceEndPos.y);
		//printf("SOURCE VEL (%f, %f)\n", sourceVel.x, sourceVel.y);
		//printf("LISTENER POS (%f, %f)\n", listenerEndPos.x, listenerEndPos.y);
		//printf("LISTENER VEL (%f, %f)\n", listenerVel.x, listenerVel.y);

		// Actualizamos ventana y eventos
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Eliminamos el buffer
	delete audioBuffer;

	// Eliminamos el contexto y el dispositivo de OpenAL
	alcDestroyContext(alcContext);
	alcCloseDevice(alcDevice);

	return 0;
}
