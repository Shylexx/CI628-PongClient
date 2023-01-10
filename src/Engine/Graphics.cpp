#include "Graphics.h"
#include "../ECS/ECS.h"
#include <iostream>

void Graphics::Init() {
	// Window
	m_Window = SDL_CreateWindow(
		"Multiplayer Pong Client",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		800, 600,
		SDL_WINDOW_SHOWN
	);

	if (!m_Window) {
		std::cerr << "Failed to create window" << SDL_GetError() << std::endl;
	}

	// Renderer
	m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED);

	if (!m_Renderer) {
		std::cerr << "Failed to create renderer" << SDL_GetError() << std::endl;
	}
}

void Graphics::Cleanup() {
	SDL_DestroyWindow(m_Window);
}


void Graphics::ClearScreen() {
	SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_Renderer);
}

void Graphics::Present() {
	SDL_RenderPresent(m_Renderer);
}

void Graphics::DrawScene(ECS::Scene* scene) {
	//for (Entity e = 0; e < MAX_ENTITIES; e++) {
	//    if (!scene->HasComponents(e, CompTags::Transform)) { continue; }

	//    //SDL_SetRenderDrawColor(m_Renderer, 255, 255, 255, 255);
	//    //SDL_RenderDrawRect(m_Renderer, &scene->m_Transforms[e].m_Rect);
	//}


	//// Render all sprites
	//for (Entity e = 0; e < MAX_ENTITIES; e++) {
	//	// Draw the level first!
	//	if (scene->HasComponents(e, CompTags::Tilemap | CompTags::Transform)) {
	//		DrawLevel(&scene->m_Tilemaps[e], &scene->m_Transforms[e]);
	//		continue;
	//	}
	//	if (scene->HasComponents(e, CompTags::Sprite | CompTags::Transform)) {
	//		// drawsprites
	//		if (&scene->m_SpriteRenders[e].m_Visible) {
	//			DrawSprite(&scene->m_SpriteRenders[e], &scene->m_Transforms[e]);
	//			continue;
	//		}
	//	}
	//	if (scene->HasComponents(e, CompTags::Text | CompTags::Transform)) {
	//		// Draw all text components
	//		DrawText(&scene->m_Texts[e], &scene->m_Transforms[e]);
	//		continue;
	//	}
	//}

		// Draw the level first!
		for (Entity e = 0; e < MAX_ENTITIES; e++) {
			if (!scene->HasComponents(e, CompTags::Tilemap | CompTags::Transform)) { continue; }

			DrawLevel(&scene->m_Tilemaps[e], &scene->m_Transforms[e]);
		}

		// Render all sprites
		for (Entity e = 0; e < MAX_ENTITIES; e++) {
			if (!scene->HasComponents(e, CompTags::Transform | CompTags::Sprite)) { continue; }

			// drawsprites
			if (&scene->m_SpriteRenders[e].m_Visible) {
				if (scene->HasComponents(e, CompTags::Bullet)) {
					//std::cout << "rendered bullet" << std::endl;
				}
				DrawSprite(&scene->m_SpriteRenders[e], &scene->m_Transforms[e]);
			}
		}
		// Draw all text components
		for (Entity e = 0; e < MAX_ENTITIES; e++) {
			if (!scene->HasComponents(e, CompTags::Text | CompTags::Transform)) { continue; }

			DrawText(&scene->m_Texts[e], &scene->m_Transforms[e]);
		}
	
}

void Graphics::DrawSprite(ECS::SpriteRender* sprite, ECS::Transform* transform, SDL_Rect* clip, SDL_Point* center) {
	if (sprite->m_Visible) {
		SDL_Rect renderQuad = { transform->m_Position.x, transform->m_Position.y, transform->m_Scale.x, transform->m_Scale.y };
		SDL_RenderCopyEx(m_Renderer, sprite->m_Sprite, clip, &renderQuad, transform->m_Rotation, center, sprite->m_Flip);
	}
}

void Graphics::DrawText(ECS::Text* text, ECS::Transform* transform) {
	SDL_Texture* textTexture = TextureFromString(text->m_Text, text->m_Font, text->m_Color);
	int w, h;
	SDL_QueryTexture(textTexture, NULL, NULL, &w, &h);
	transform->m_Scale.x = w;
	transform->m_Scale.y = h;
	SDL_Rect renderQuad = { transform->m_Position.x, transform->m_Position.y, transform->m_Scale.x, transform->m_Scale.y };
	SDL_RenderCopyEx(m_Renderer, textTexture, NULL, &renderQuad, transform->m_Rotation, NULL, SDL_FLIP_NONE);
	SDL_DestroyTexture(textTexture);
}

void Graphics::DrawLevel(ECS::Tilemap* map, ECS::Transform* transform) {
	for (int y = 0; y < map->m_Tiles.size(); y++) {
		for (int x = 0; x < map->m_Tiles[y].size(); x++) {
			SDL_Texture* sprite = nullptr;
			switch (map->m_Tiles[y][x]) {
			case ECS::EMPTY:
				sprite = map->m_EmptyTexture;
				break;
			case ECS::WALL:
				sprite = map->m_WallTexture;
				break;
			default:
				break;
			}
			DrawTile(sprite, x, y);
		}
	}
}

void Graphics::DrawTile(SDL_Texture* sprite, int x, int y) {
	SDL_Rect renderQuad = { x * ECS::TILE_WIDTH, y * ECS::TILE_HEIGHT, ECS::TILE_WIDTH, ECS::TILE_HEIGHT };
	SDL_RenderCopyEx(m_Renderer, sprite, NULL, &renderQuad, 0, NULL, SDL_FLIP_NONE);
}

SDL_Texture* Graphics::TextureFromSurface(SDL_Surface* surface) {
	return SDL_CreateTextureFromSurface(m_Renderer, surface);
}

SDL_Texture* Graphics::TextureFromString(const std::string& string, TTF_Font* font, SDL_Color color) {
	SDL_Texture* textTexture = nullptr;
	SDL_Surface* textSurface = TTF_RenderText_Solid(font, string.c_str(), color);
	if (textSurface != nullptr) {
		textTexture = SDL_CreateTextureFromSurface(m_Renderer, textSurface);
		SDL_FreeSurface(textSurface);
	}
	else {
		std::cout << "Failed to create texture from string: " << string << std::endl;
		std::cout << TTF_GetError() << std::endl;
	}

	if (textTexture == nullptr)
		std::cerr << "Text Texture was not made!" << std::endl;
	return textTexture;
}
