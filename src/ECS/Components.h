#ifndef __COMPS_H__
#define __COMPS_H__

#include "SDL.h"
#include "../Maths.h"

namespace ECS {
	struct Transform
	{
		// Position in 2D Space
		Vec2 m_Position;
		// Rotation in degrees
		int m_Rotation;
		// Scale on X and Y Axis
		Vec2 m_Scale;

		// SDL Rect for SDL functions
		SDL_Rect m_Rect;
	};

	struct SpriteRender
	{
		SDL_Texture* m_Sprite;
		SDL_RendererFlip m_Flip = SDL_FLIP_NONE;
		bool m_Visible = true;
	};

	// Allows an Entity to be found by name rather than just tag
	using Name = std::string;

	enum Team {
		RED,
		BLUE,
	};

	// Component for player entity,
	// Contains differentiated data to discern between multiple network players
	struct NetPlayer {
		int m_ID;
		Team m_Team;
	};

	// Actual Data for player character objects
	struct Player 
	{
		// Player Health points
		int m_Health;
	};

	// Collider Object for collision testing
	using Collider = SDL_Rect;

	struct Kinematic {
		Vec2f m_Velocity;
		float m_JumpForce = 5.0f;
		float m_Gravity = 0.2f;
		float m_MaxFallSpeed = -5.0f;
		float m_CurJumpForce = 0.0f;
		bool m_Grounded = true;
	};
}
#endif