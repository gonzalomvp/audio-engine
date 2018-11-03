#pragma once

#include <array>
#include <vector>

#include "sprite.h"

class World {
public:
	World(
		float clearRed = 0.15f, float clearGreen = 0.15f, float clearBlue = 0.15f,
		const ltex_t* back0 = nullptr, const ltex_t* back1 = nullptr, const ltex_t* back2 = nullptr, const ltex_t* back3 = nullptr)
		: m_clearRed(clearRed), m_clearGreen(clearGreen), m_clearBlue(clearBlue), m_backgrounds{ back0 ,back1, back2, back3 }
		, m_scrollRatios{}, m_scrollSpeeds{}, m_scrollOffsets{}, m_cameraPosition(0, 0), m_mapTexture(nullptr), m_mapSize(0, 0) {}

	~World();

	float         getClearRed() const                                               { return m_clearRed; }
	float         getClearGreen() const                                             { return m_clearGreen; }
	float         getClearBlue() const                                              { return m_clearBlue; }
	void          setClearColors(float clearRed, float clearGreen, float clearBlue) { m_clearRed = clearRed; m_clearGreen = clearGreen; m_clearBlue = clearBlue; }

	const ltex_t* getBackground(size_t layer) const                                 { return m_backgrounds[layer]; }
	void          setBackground(size_t layer, const ltex_t* back)                   { m_backgrounds[layer] = back; }
																                    
	float         getScrollRatio(size_t layer) const                                { return m_scrollRatios[layer]; }
	void          setScrollRatio(size_t layer, float ratio)                         { m_scrollRatios[layer] = ratio; }
																                    
	const Vec2&   getScrollSpeed(size_t layer) const                                { return m_scrollSpeeds[layer]; }
	void          setScrollSpeed(size_t layer, const Vec2& speed)                   { m_scrollSpeeds[layer] = speed; }
																                    
	const Vec2&   getCameraPosition() const                                         { return m_cameraPosition; }
	void          setCameraPosition(const Vec2& pos)                                { m_cameraPosition = pos; }

	Vec2          getMapSize() const                                                { return m_mapSize; }

	void          addSprite(Sprite& sprite);
	void          removeSprite(Sprite& sprite);
	bool          moveSprite(Sprite& sprite, const Vec2& amount);

	bool          loadMap(const char* filename, uint16_t firstColId);
	void          update(float deltaTime);
	void          draw(const Vec2& screenSize);

private:
	float                        m_clearRed;
	float                        m_clearGreen;
	float                        m_clearBlue;
	std::array<const ltex_t*, 4> m_backgrounds;
	std::array<float, 4>         m_scrollRatios;
	std::array<Vec2, 4>          m_scrollSpeeds;
	std::array<Vec2, 4>          m_scrollOffsets;
	Vec2                         m_cameraPosition;
	std::vector<Sprite*>         m_sprites;
	std::vector<Sprite*>         m_mapSprites;
	ltex_t*                      m_mapTexture;
	Vec2                         m_mapSize;
};