#include "ECS.h"
#include <iostream>
#include <stdexcept>

namespace ECS {
	bool Scene::IsValid(Entity e) {
		return (m_Entities[e].test(0));
	}

	bool Scene::HasComponents(Entity e, EntitySig s) {
		// Return false is entity is not created/existing yet
		if (!IsValid(e)) return false;

		// Return true if entity has all components in signature
		return (m_Entities[e] & s) == s;
	}

	Entity Scene::NewEntity() {
		for (Entity e = 0; e < MAX_ENTITIES; e++)
		{
			if (m_Entities[e] == 0x0) {
				SDL_Log("New Entity Spawned");
				// Set the valid flag for the entity
				m_Entities[e].set(0, true);
				return e;
			}
		}
	}

	void Scene::AddComponents(Entity e, EntitySig s) {
		// Ensure entity is valid first
		if (!IsValid(e)) {
			std::cerr << "Couldn't add components to invalid entity " << e << std::endl;
			return;
		}

		m_Entities[e] = m_Entities[e] | s;
	}

	Transform Scene::GetTransform(Entity e) {
		if (!IsValid(e) && !HasComponents(e, CompTags::Transform))
		{
			throw std::runtime_error("Can't get transform of entity " + e);
		}

		return m_Transforms[e];
	}

	Name Scene::GetName(Entity e) {
		if (!IsValid(e) && !HasComponents(e, CompTags::Name)) {
			throw std::runtime_error("Can't get name of entity " + e);
		}

		return m_Names[e];
	}

	SpriteRender Scene::GetSpriteRender(Entity e) {
		if (!IsValid(e) && !HasComponents(e, CompTags::Transform))
		{
			throw std::runtime_error("Can't get Sprite render of entity " + e);
		}

		return m_SpriteRenders[e];
	}

	Entity Scene::FindEntityByName(const std::string& name) {
		for (Entity e = 0; e < MAX_ENTITIES; e++)
		{
			if (!IsValid(e) && !HasComponents(e, CompTags::Name)) { continue; }

			if (m_Names[e] == name) {
				return e;
			}
		}

		// If no entity with name found, error
		throw std::runtime_error("No Entity found with name " + name);
	}
}