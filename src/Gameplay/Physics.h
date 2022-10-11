#ifndef __PHYSICS_H__
#define __PHYSICS_H__

#include "../ECS/ECS.h"

void update_kinematics(ECS::Scene* scene, float deltaTime);
bool Overlapping(Entity a, Entity b);

#endif