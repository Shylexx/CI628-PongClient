#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "SDL.h"
#include "../ECS/ECS.h"
#include <unordered_map>

class Graphics{
private:
	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window;
public:
	SDL_Texture* TextureFromSurface(SDL_Surface* surface);
	SDL_Texture* TextureFromString(const std::string& string, TTF_Font* font, SDL_Color color);
	void DrawSprite(ECS::SpriteRender* sprite, ECS::Transform* transform, SDL_Rect* clip = NULL, SDL_Point* center = NULL);
	void DrawText(ECS::Text* text, ECS::Transform* transform);
	void DrawLevel(ECS::Tilemap* text, ECS::Transform* transform);

	void Init();
	void Cleanup();

	void ClearScreen();

	void DrawScene(ECS::Scene* scene);
	void Present();

};

#endif