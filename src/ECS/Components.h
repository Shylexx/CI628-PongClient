#ifndef __COMPS_H__
#define __COMPS_H__

#include "SDL.h"
#include "SDL_ttf.h"
#include "../Maths.h"
#include <memory>

namespace ECS {
	struct Transform
	{
		// Position in 2D Space
		Vec2f m_Position = { 0, 0 };
		// Rotation in degrees
		int m_Rotation = 0;
		// Scale on X and Y Axis
		Vec2f m_Scale = { 40, 40};
	};

	struct SpriteRender
	{
		SDL_Texture* m_Sprite = nullptr;
		SDL_RendererFlip m_Flip = SDL_FLIP_NONE;
		bool m_Visible = true;
	};

	struct Text {
		std::string m_Text = "None";
		TTF_Font* m_Font;
		SDL_Color m_Color = { 0xFF, 0xFF, 0xFF, 0xFF };
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

	enum TileType {
		EMPTY = 0,
		WALL = 1,
	};

	struct Tilemap {
		TileType m_Tiles[10][10];

		void setEmpty() {
			for (auto& row : m_Tiles) {
				for (auto& tile : row) {
					tile = EMPTY;
				}
			}
		}

		void setTile(TileType type, int x, int y) {
			m_Tiles[y][x] = type;
		}
	};
}
#endif