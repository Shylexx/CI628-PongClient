#include "ECS.h"

namespace ECS {
	bool Scene::IsValid(Entity e) {
		return (m_Entities[e].test(0));
	}

	bool Scene::HasComponents(Entity e, EntitySig s) {
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
			SDL_Log("Can't add components to invalid entity");
			return;
		}

		m_Entities[e] = m_Entities[e] | s;
	}
}