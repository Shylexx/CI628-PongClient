#include "Physics.h"

void update_kinematics(ECS::Scene* scene, float deltaTime) {
	//EntitySig query = CompTags::Transform | CompTags::Kinematic;
	//for (Entity e = 0; e < MAX_ENTITIES; e++) {
	//	if (!scene->HasComponents(e, query)) { continue; }

	//	ECS::Transform& t = scene->m_Transforms[e];
	//	ECS::Kinematic& k = scene->m_Kinematics[e];

	//	t.m_Rect.x = k.m_Velocity.x * deltaTime;
	//	t.m_Rect.y = k.m_Velocity.y * deltaTime;
	//}

	//// Update velocities based on collision

}

bool Overlapping(Entity a, Entity b) {

}