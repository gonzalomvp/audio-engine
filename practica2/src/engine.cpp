#include "engine.h"
#include "Vec2.h"

#include <fstream>
#include <vector>

#define LITE_GFX_IMPLEMENTATION
#include <litegfx.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

ltex_t* loadTexture(const char* filename)
{
	int width, height;
	unsigned char* buffer;
	buffer = stbi_load(filename, &width, &height, nullptr, 4);
	if (!buffer) return nullptr;

	ltex_t* texture = ltex_alloc(width, height, 0);
	ltex_setpixels(texture, buffer);

	stbi_image_free(buffer);
	return texture;
}
