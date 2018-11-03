#pragma once

#include <vector>

#include <litegfx.h>

#include "vec2.h"

class Collider;

class Sprite {
public:
	enum CollisionType {
		COLLISION_NONE,
		COLLISION_CIRCLE,
		COLLISION_RECT,
		COLLISION_PIXELS
	};

	typedef void(*CallbackFunc)(Sprite&, float);

	Sprite(const ltex_t* tex, int hframes = 1, int vframes = 1);
	~Sprite();

	const ltex_t*    getTexture() const;
	void             setTexture(const ltex_t* tex);

	lblend_t         getBlend() const;
	void             setBlend(lblend_t mode);

	float            getRed() const;
	float            getGreen() const;
	float            getBlue() const;
	float            getAlpha() const;
	void             setColor(float r, float g, float b, float a);

	const Vec2&      getPosition() const;
	void             setPosition(const Vec2& pos);

	float            getAngle() const;
	void             setAngle(float angle);

	const Vec2&      getScale() const;
	void             setScale(const Vec2& scale);

	Vec2             getSize() const;

	const Vec2&      getPivot() const;
	void             setPivot(const Vec2& pivot);

	int              getHframes() const;
	int              getVframes() const;
	void             setFrames(int hframes, int vframes);

	int              getFps() const;
	void             setFps(int fps);

	float            getCurrentFrame() const;
	void             setCurrentFrame(int frame);

	void             setCollisionType(CollisionType type);
	CollisionType    getCollisionType() const;
	const Collider*  getCollider() const;
	bool             collides(const Sprite& other) const;

	const float      getRadius() const;
	const Vec2       getCenterPos() const;
	const Vec2       getTopLeftPos() const;
	const uint8_t*   getAlphaPixels() const;
	void             updateAlphaPixels();

	CallbackFunc     getCallback();
	void             setCallback(CallbackFunc callback);

	void*            getUserData();
	void             setUserData(void* userData);

	void             update(float deltaTime);
	void             draw() const;

private:
	const ltex_t*         m_tex;
	int                   m_hframes;
	int                   m_vframes;
	lblend_t              m_blend;
	float                 m_red;
	float                 m_green;
	float                 m_blue;
	float                 m_alpha;
	Vec2                  m_pos;
	float                 m_angle;
	Vec2                  m_scale;
	Vec2                  m_pivot;
	int                   m_fps;
	float                 m_currentFrame;
	Vec2                  m_size;
	Vec2                  m_frameOffset;
	CollisionType         m_collisionType;
	Collider*             m_collider;
	std::vector<uint8_t>  m_alphaPixels;
	CallbackFunc          m_callback;
	void*                 m_userData;

	void                  updateSize();
	void                  updateFrameOffset();
};