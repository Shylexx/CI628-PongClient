#include <cstdint>
#include <bitset>
#include <array>
#include <vector>
#include "Components.h"
#ifndef __ECS_H__
#define __ECS_H__

// EntityTags array is 20kb with 5000 entries
constexpr int MAX_ENTITIES = 50;
constexpr int MAX_COMPS = 32;

// Entity ID is used to index inside the scenes components arrays
using Entity = uint32_t;

// An entities components are represented by a bitfield
// This allows each possible combination of components to have a unique signature
using EntitySig = std::bitset<MAX_COMPS>;

namespace CompTags
{
	// Valid component does not exist as data structure
	// This bits presence in a signature just indicates the entity exists
	constexpr EntitySig Valid = 0x1 << 0;
	constexpr EntitySig Transform = 0x1 << 1;
	constexpr EntitySig Name = 0x1 << 2;
}

namespace ECS {
	class Scene {
	private:
		// Entities are held inside an array of bitfields
		// Each bitfield represents the signature of an entity
		std::array<EntitySig, MAX_ENTITIES> m_Entities;

	public:
		std::array<Transform, MAX_ENTITIES> m_Transforms;
		std::array<Name, MAX_ENTITIES> m_Names;
		std::array<SpriteRender, MAX_ENTITIES> m_SpriteRenders;

		bool IsValid(Entity e);
		bool HasComponents(Entity e, EntitySig s);
		void AddComponents(Entity e, EntitySig c);
		Entity NewEntity();




	};
}

#endif