#include "collider.h"

#include "sprite.h"

//-----------------------------------------------------------------------------
// CircleCollider

CircleCollider::CircleCollider(const Sprite* sprite) {
	m_sprite = sprite;
}

bool CircleCollider::collides(const Collider& other) const {
	return other.collides(m_sprite->getCenterPos(), m_sprite->getRadius());
}

bool CircleCollider::collides(const Vec2& circlePos, float circleRadius) const {
	return checkCircleCircle(m_sprite->getCenterPos(), m_sprite->getRadius(), circlePos, circleRadius);
}

bool CircleCollider::collides(const Vec2& rectPos, const Vec2& rectSize) const {
	return checkCircleRect(m_sprite->getCenterPos(), m_sprite->getRadius(), rectPos, rectSize);
}

bool CircleCollider::collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const {
	return checkCirclePixels(m_sprite->getCenterPos(), m_sprite->getRadius(), pixelsPos, pixelsSize, pixels);
}

//-----------------------------------------------------------------------------
// RectCollider

RectCollider::RectCollider(const Sprite* sprite) {
	m_sprite = sprite;
}

bool RectCollider::collides(const Collider& other) const {
	return other.collides(m_sprite->getTopLeftPos(), m_sprite->getSize());
}

bool RectCollider::collides(const Vec2& circlePos, float circleRadius) const {
	return checkCircleRect(circlePos, circleRadius, m_sprite->getTopLeftPos(), m_sprite->getSize());
}

bool RectCollider::collides(const Vec2& rectPos, const Vec2& rectSize) const {
	return checkRectRect(m_sprite->getTopLeftPos(), m_sprite->getSize(), rectPos, rectSize);
}

bool RectCollider::collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const {
	return checkPixelsRect(pixelsPos, pixelsSize, pixels, m_sprite->getTopLeftPos(), m_sprite->getSize());
}

//-----------------------------------------------------------------------------
// PixelsCollider

PixelsCollider::PixelsCollider(const Sprite* sprite) {
	m_sprite = sprite;
}

bool PixelsCollider::collides(const Collider& other) const {
	return other.collides(m_sprite->getTopLeftPos(), m_sprite->getSize(), m_sprite->getAlphaPixels());
}

bool PixelsCollider::collides(const Vec2& circlePos, float circleRadius) const {
	return checkCirclePixels(circlePos, circleRadius, m_sprite->getTopLeftPos(), m_sprite->getSize(), m_sprite->getAlphaPixels());
}

bool PixelsCollider::collides(const Vec2& rectPos, const Vec2& rectSize) const {
	return checkPixelsRect(m_sprite->getTopLeftPos(), m_sprite->getSize(), m_sprite->getAlphaPixels(), rectPos, rectSize);
}

bool PixelsCollider::collides(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) const {
	return checkPixelsPixels(m_sprite->getTopLeftPos(), m_sprite->getSize(), m_sprite->getAlphaPixels(), pixelsPos, pixelsSize, pixels);
}

//-----------------------------------------------------------------------------
// Helper funcs

bool checkCircleCircle(const Vec2& pos1, float radius1, const Vec2& pos2, float radius2) {
	return pos1.distance(pos2) < radius1 + radius2;
}

bool checkCircleRect(const Vec2& circlePos, float circleRadius, const Vec2& rectPos, const Vec2& rectSize) {
	Vec2 closestPoint(clamp(circlePos.x, rectPos.x, rectPos.x + rectSize.x), clamp(circlePos.y, rectPos.y, rectPos.y + rectSize.y));
	return circlePos.distance(closestPoint) < circleRadius;
}

bool checkRectRect(const Vec2& rectPos1, const Vec2& rectSize1, const Vec2& rectPos2, const Vec2& rectSize2) {
	return ((isInBounds(rectPos1.x, rectPos2.x, rectPos2.x + rectSize2.x) || isInBounds(rectPos2.x, rectPos1.x, rectPos1.x + rectSize1.x) || rectPos1.x == rectPos2.x) &&
		    (isInBounds(rectPos1.y, rectPos2.y, rectPos2.y + rectSize2.y) || isInBounds(rectPos2.y, rectPos1.y, rectPos1.y + rectSize1.y) || rectPos1.y == rectPos2.y));
}

bool checkPixelsRect(const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels, const Vec2& rectPos, const Vec2& rectSize) {
	bool collision = false;

	if (checkRectRect(pixelsPos, pixelsSize, rectPos, rectSize)) {
		Vec2 overlapPos;
		Vec2 overlapSize;
		getOverlap(pixelsPos, pixelsSize, rectPos, rectSize, overlapPos, overlapSize);

		for (float posX = overlapPos.x; posX < overlapPos.x + overlapSize.x && !collision; posX++)
		{
			for (float posY = overlapPos.y; posY < overlapPos.y + overlapSize.y && !collision; posY++)
			{
				Vec2 pixelToCheck(posX, posY);
				Vec2 relativePixel = pixelToCheck - pixelsPos;
				int bufferIndex = static_cast<int>(relativePixel.y) * static_cast<int>(pixelsSize.x) + static_cast<int>(relativePixel.x);
				if (pixels[bufferIndex] > 127) {
					collision = true;
				}
			}
		}
	}
	return collision;
}

bool checkCirclePixels(const Vec2& circlePos, float circleRadius, const Vec2& pixelsPos, const Vec2& pixelsSize, const uint8_t* pixels) {
	bool collision = false;
	Vec2 rectPos = circlePos - circleRadius;
	Vec2 rectSize(circleRadius * 2, circleRadius * 2);

	if (checkRectRect(pixelsPos, pixelsSize, rectPos, rectSize)) {
		Vec2 overlapPos;
		Vec2 overlapSize;
		getOverlap(pixelsPos, pixelsSize, rectPos, rectSize, overlapPos, overlapSize);

		for (float posX = overlapPos.x; posX < overlapPos.x + overlapSize.x && !collision; posX++)
		{
			for (float posY = overlapPos.y; posY < overlapPos.y + overlapSize.y && !collision; posY++)
			{
				Vec2 pixelToCheck(posX, posY);
				Vec2 relativePixel = pixelToCheck - pixelsPos;
				int bufferIndex = static_cast<int>(relativePixel.y) * static_cast<int>(pixelsSize.x) + static_cast<int>(relativePixel.x);
				if (pixels[bufferIndex] > 127 && pixelToCheck.distance(circlePos)< circleRadius) {
					collision = true;
				}
			}
		}
	}
	return collision;
}

bool checkPixelsPixels(const Vec2& pixelsPos1, const Vec2& pixelsSize1, const uint8_t* pixels1, const Vec2& pixelsPos2, const Vec2& pixelsSize2, const uint8_t* pixels2) {
	bool collision = false;
	
	if (checkRectRect(pixelsPos1, pixelsSize1, pixelsPos2, pixelsSize2)) {
		Vec2 overlapPos;
		Vec2 overlapSize;
		getOverlap(pixelsPos1, pixelsSize1, pixelsPos2, pixelsSize2, overlapPos, overlapSize);

		for (float posX = overlapPos.x; posX < overlapPos.x + overlapSize.x && !collision; posX++)
		{
			for (float posY = overlapPos.y; posY < overlapPos.y + overlapSize.y && !collision; posY++)
			{
				Vec2 pixelToCheck(posX, posY);
				Vec2 relativePixel1 = pixelToCheck - pixelsPos1;
				Vec2 relativePixel2 = pixelToCheck - pixelsPos2;
				int bufferIndex1 = static_cast<int>(relativePixel1.y) * static_cast<int>(pixelsSize1.x) + static_cast<int>(relativePixel1.x);
				int bufferIndex2 = static_cast<int>(relativePixel2.y) * static_cast<int>(pixelsSize2.x) + static_cast<int>(relativePixel2.x);
				if (pixels1[bufferIndex1] > 127 && pixels2[bufferIndex2] > 127) {
					collision = true;
				}
			}
		}
	}
	return collision;
}

void getOverlap(const Vec2& pos1, const Vec2& size1, const Vec2& pos2, const Vec2& size2, Vec2& overlapPos, Vec2& overlapSize) {
	Vec2 end1 = pos1 + size1;
	Vec2 end2 = pos2 + size2;

	overlapPos.x = (isInBounds(pos1.x, pos2.x, end2.x)) ? pos1.x : pos2.x;
	overlapPos.y = (isInBounds(pos1.y, pos2.y, end2.y)) ? pos1.y : pos2.y;

	overlapSize.x = (isInBounds(end1.x, pos2.x, end2.x)) ? end1.x - overlapPos.x : end2.x - overlapPos.x;
	overlapSize.y = (isInBounds(end1.y, pos2.y, end2.y)) ? end1.y - overlapPos.y : end2.y - overlapPos.y;
}