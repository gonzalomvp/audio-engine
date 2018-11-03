#include "font.h"

#include <fstream>

#include <vector>

#define STB_TRUETYPE_IMPLEMENTATION
#include <stb_truetype.h>

Font* Font::load(const char* filename, float height)
{
	Font* pFont = nullptr;
	std::ifstream file(filename, std::ifstream::binary | std::ifstream::ate);

	if (file.is_open()) {

		int size = static_cast<int>(file.tellg());

		if (size > 0) {
			pFont = new Font();
			pFont->m_height = height;

			file.seekg(0, file.beg);
			unsigned char* fileBuffer = new unsigned char[size];

			for (int i = 0; i < size; i++)
			{
				fileBuffer[i] = file.get();
			}

			int textureWidth = 1024;
			int textureHeight = 512;

			unsigned char* alphaBuffer = new unsigned char[textureWidth * textureHeight];

			while (stbtt_BakeFontBitmap(fileBuffer, 0, height, alphaBuffer, textureWidth, textureHeight, 0, 256, pFont->m_chardata.data()) <= 0) {
				textureWidth = textureWidth * 2;
				textureHeight = textureHeight * 2;
				delete[] alphaBuffer;
				alphaBuffer = new unsigned char[textureWidth * textureHeight];
			}
			delete[] fileBuffer;

			int colorBufferSize = textureWidth * textureHeight * 4;
			unsigned char* colorBuffer = new unsigned char[colorBufferSize];

			for (int i = 0; i < colorBufferSize; i++)
			{
				if (i % 4 == 3) {
					colorBuffer[i] = alphaBuffer[i / 4];
				}
				else {
					colorBuffer[i] = 255;
				}
			}
			delete[] alphaBuffer;

			pFont->m_texture = ltex_alloc(textureWidth, textureHeight, 0);
			ltex_setpixels(pFont->m_texture, colorBuffer);
			delete[] colorBuffer;
		}

		file.close();

	}
	return pFont;
}

Font::Font()
{
	m_texture = nullptr;
	m_height = 0;
}

Font::~Font()
{
	ltex_free(m_texture);
}

float Font::getHeight() const
{
	return m_height;
}

void Font::draw(const char* text, const Vec2& pos) const
{
	int length = strlen(text);
	float xpos = pos.x;
	float ypos = pos.y;
	stbtt_aligned_quad q;

	for (int i = 0; i < length; i++)
	{
		stbtt_GetBakedQuad(m_chardata.data(), m_texture->width, m_texture->height, text[i], &xpos, &ypos, &q, true);
		ltex_drawrotsized(m_texture, q.x0, q.y0, 0, 0, 0, q.x1 - q.x0, q.y1 - q.y0, q.s0, q.t0, q.s1, q.t1);
	}
}

Vec2 Font::getTextSize(const char* text) const
{
	Vec2 size;

	int   length = strlen(text);
	float minY = FLT_MAX;
	float maxY = -FLT_MAX;
	float xpos = 0;
	float ypos = 0;
	float width = 0;

	stbtt_aligned_quad q;

	for (int i = 0; i < length; i++)
	{
		width = xpos;
		stbtt_GetBakedQuad(m_chardata.data(), m_texture->width, m_texture->height, text[i], &xpos, &ypos, &q, true);

		if (q.y0 < minY) {
			minY = q.y0;
		}
		if (q.y1 > maxY) {
			maxY = q.y1;
		}

	}
	size.x = width + (q.x1 - q.x0);
	size.y = maxY - minY;

	return size;
}