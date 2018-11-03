#pragma once

#include <array>

#include <litegfx.h>
#include <stb_truetype.h>

#include "vec2.h"

class Font {
public:
	~Font();

	static Font* load       (const char* filename, float height);
	float        getHeight  ()                                    const;
	Vec2         getTextSize(const char* text)                    const;
	void         draw       (const char* text, const Vec2& pos)   const;

private:
	Font();

	ltex_t*                          m_texture;
	std::array<stbtt_bakedchar, 256> m_chardata;
	float                            m_height;
};
