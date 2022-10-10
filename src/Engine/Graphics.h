#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

#include "SDL.h"
#include "../ECS/ECS.h"

class Graphics{
private:
	SDL_Renderer* m_Renderer;
	SDL_Window* m_Window;
public:
	SDL_Texture* TextureFromSurface(SDL_Surface* surface);

	void Init();
	void Cleanup();

	void ClearScreen();

	void DrawScene(ECS::Scene* scene);
	void Present();

};

#endif