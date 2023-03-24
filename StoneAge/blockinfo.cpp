#include "blockinfo.h"
#include <iostream>

BlockInfo::BlockInfo(BlockType type, const std::string& name) : m_type(type), m_name(name), m_durability(1)
{
}

BlockInfo::~BlockInfo()
{
}

BlockType BlockInfo::GetType() const
{
	return m_type;
}

void BlockInfo::SetDurability(int durability)
{
	m_durability = durability;
}

int BlockInfo::GetDurability() const
{
	return m_durability;
}

void BlockInfo::Show() const
{
	std::cout << "Type: " << m_type << std::endl;
	std::cout << "Nom: " << m_name << std::endl;
	std::cout << "Durabilite: " << m_durability << std::endl;
}

void BlockInfo::SetUVW(float u, float v, float w)
{
	// L'équivalent de l'axe des x sur une texture
	m_u = u;
	// L'équivalent de l'axe des y sur une texture
	m_v = v;
	// L'axe des x et y sur le TextureAtlas (ce qui regroupe les 4 textures)
	// Vu que c'est un carré, x = y et y = x donc pas besoin de deux variables
	m_w = w;
}

float BlockInfo::GetU() const
{
	return m_u;
}

float BlockInfo::GetV() const
{
	return m_v;
}

float BlockInfo::GetW() const
{
	return m_w;
}