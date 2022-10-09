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

	using Velocity = Vec2;

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
}
#endif