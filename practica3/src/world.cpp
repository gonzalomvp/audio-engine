#include "World.h"

#include "pugixml.hpp"

#include "engine.h"
#include "string_utils.h"

World::~World()
{
	for (size_t i = 0; i < m_mapSprites.size(); i++) {
		delete m_mapSprites[i];
	}
	ltex_free(m_mapTexture);
}

void World::addSprite(Sprite& sprite)
{
	m_sprites.push_back(&sprite);
}

void World::removeSprite(Sprite& sprite)
{
	m_sprites.erase(
		std::remove_if(m_sprites.begin(), m_sprites.end(), [&sprite](const Sprite* spriteInVector) -> bool { return &sprite == spriteInVector; })
		, m_sprites.end());
}

bool World::moveSprite(Sprite& sprite, const Vec2& amount)
{
	Vec2 currentPos = sprite.getPosition();
	bool collisionX = false;
	sprite.setPosition(Vec2(currentPos.x + amount.x, currentPos.y));
	for (size_t i = 0; i < m_mapSprites.size(); i++) {
		if (sprite.collides(*m_mapSprites[i])) {
			sprite.setPosition(currentPos);
			collisionX = true;
			break;
		}
	}

	currentPos = sprite.getPosition();
	bool collisionY = false;
	sprite.setPosition(Vec2(currentPos.x , currentPos.y + amount.y));
	for (size_t i = 0; i < m_mapSprites.size(); i++) {
		if (sprite.collides(*m_mapSprites[i])) {
			sprite.setPosition(currentPos);
			collisionY = true;
			break;
		}
	}

	return collisionX || collisionY;
}

bool World::loadMap(const char* filename, uint16_t firstColId)
{
	pugi::xml_document doc;
	pugi::xml_parse_result result = doc.load_file(filename);
	if(!result) return false;

	// Procesamos el nodo map
	pugi::xml_node mapNode = doc.child("map");
	int width = mapNode.attribute("width").as_int();
	int height = mapNode.attribute("height").as_int();
	int tileWidth = mapNode.attribute("tilewidth").as_int();
	int tileHeight = mapNode.attribute("tileheight").as_int();
	m_mapSize= Vec2(static_cast<float>(width*tileWidth), static_cast<float>(height*tileHeight));

	// Procesamos el nodo tileset
	pugi::xml_node tilesetNode = mapNode.child("tileset");
	int firstgid = tilesetNode.attribute("firstgid").as_int();
	int tilecount = tilesetNode.attribute("tilecount").as_int();
	int columns = tilesetNode.attribute("columns").as_int();

	// Procesamos el nodo image
	pugi::xml_node imageNode = tilesetNode.child("image");
	const pugi::char_t* imageSource = imageNode.attribute("source").as_string();
	std::string imageFile = extractPath(std::string(filename)) + std::string(imageSource);
	m_mapTexture = loadTexture(imageFile.c_str());

	// Procesamos el nodo data
	pugi::xml_node dataNode = mapNode.child("layer").child("data");

	// Si el mapa tiene una codificacion diferente a la de por defecto no lo cargamos
	if (!dataNode.attribute("encoding").empty()) return false;

	// Procesamos cada uno de los tiles
	int tileCount = 0;
	for (pugi::xml_node tileNode = dataNode.child("tile"); tileNode; tileNode = tileNode.next_sibling("tile")) {
		int row = tileCount / width;
		int column = tileCount % width;
			
		int gid = tileNode.attribute("gid").as_int();
		if (gid >= firstgid) {
			Sprite* tile = new Sprite(m_mapTexture, columns, tilecount / columns);
			tile->setPosition(Vec2(static_cast<float>(tileWidth*column), static_cast<float>(tileHeight*row)));
			tile->setCurrentFrame(gid - firstgid);
			// Asignamos un collider en funcion del firstColId
			if (gid >= firstColId) {
				tile->setCollisionType(Sprite::COLLISION_RECT);
			}
			m_mapSprites.push_back(tile);
		}
		++tileCount;
	}
	return true;
}

void World::update(float deltaTime)
{
	// Calculamos el scroll automatico para cada fondo
	for (size_t i = 0; i < m_backgrounds.size(); i++) {
		if (m_backgrounds[i]) {
			m_scrollOffsets[i] = m_scrollOffsets[i] + m_scrollSpeeds[i] * deltaTime;

			// Ajustamos para evitar overflow
			while (m_scrollOffsets[i].x >= m_backgrounds[i]->width) {
				m_scrollOffsets[i].x -= m_backgrounds[i]->width;
			}
			while (m_scrollOffsets[i].y >= m_backgrounds[i]->height) {
				m_scrollOffsets[i].y -= m_backgrounds[i]->height;
			}
			while (m_scrollOffsets[i].x <= -m_backgrounds[i]->width) {
				m_scrollOffsets[i].x += m_backgrounds[i]->width;
			}
			while (m_scrollOffsets[i].y <= -m_backgrounds[i]->height) {
				m_scrollOffsets[i].y += m_backgrounds[i]->height;
			}
		}
	}

	// Actualizamos los sprites
	for (size_t i = 0; i < m_sprites.size(); i++) {
		m_sprites[i]->update(deltaTime);
	}
}

void World::draw(const Vec2& screenSize)
{
	// Limpiado de pantalla
	lgfx_clearcolorbuffer(m_clearRed, m_clearGreen, m_clearBlue);
	lgfx_setcolor(1, 1, 1, 1);
	lgfx_setblend(BLEND_ALPHA);

	// Pintamos los fondos teniendo en cuenta el scroll automatico y de la camara
	lgfx_setorigin(0, 0);
	for (int i = m_backgrounds.size() - 1; i >= 0; i--) {
		if (m_backgrounds[i]) {
			Vec2 totalScroll = m_cameraPosition * m_scrollRatios[i] - m_scrollOffsets[i];
			Vec2 uBackground(totalScroll.x / m_backgrounds[i]->width, totalScroll.y / m_backgrounds[i]->height);
			Vec2 vBackground = uBackground + Vec2(m_mapSize.x / m_backgrounds[i]->width, m_mapSize.y / m_backgrounds[i]->height);
			ltex_drawrotsized(m_backgrounds[i], 0, 0, 0, 0, 0, m_mapSize.x, m_mapSize.y, uBackground.x, uBackground.y, vBackground.x, vBackground.y);
		}
	}

	lgfx_setorigin(m_cameraPosition.x, m_cameraPosition.y);

	// Pintamos el mapa
	for (size_t i = 0; i < m_mapSprites.size(); i++)
	{
		m_mapSprites[i]->draw();
	}

	// Pintamos los sprites
	for (size_t i = 0; i < m_sprites.size(); i++)
	{
		m_sprites[i]->draw();
	}
}
